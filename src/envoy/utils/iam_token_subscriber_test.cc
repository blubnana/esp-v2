// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/envoy/utils/iam_token_subscriber.h"

#include "common/http/message_impl.h"
#include "common/tracing/http_tracer_impl.h"
#include "test/mocks/init/mocks.h"
#include "test/mocks/server/mocks.h"
#include "test/test_common/utility.h"

#include "gmock/gmock-generated-function-mockers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace Utils {
namespace {

using ::Envoy::Server::Configuration::MockFactoryContext;

using ::testing::_;
using ::testing::Invoke;
using ::testing::MockFunction;
using ::testing::Return;
using ::testing::ReturnRef;

class IamTokenSubscriberTest : public testing::Test {
 protected:
  void SetUp() override {}

  void setUp() {
    Init::TargetHandlePtr init_target_handle;
    EXPECT_CALL(context_.init_manager_, add(_))
        .WillOnce(Invoke([&init_target_handle](const Init::Target& target) {
          init_target_handle = target.createHandle("test");
        }));

    iam_token_sub_.reset(new IamTokenSubscriber(
        context_, access_token_fn_.AsStdFunction(), "token_cluster",
        "http://iam/uri_suffix", IamTokenSubscriber::IdentityToken, delegates_,
        scopes_, id_token_callback_.AsStdFunction()));

    raw_mock_client_ =
        std::make_unique<NiceMock<Envoy::Http::MockAsyncClient>>();
    EXPECT_CALL(context_.cluster_manager_, httpAsyncClientForCluster(_))
        .WillRepeatedly(ReturnRef(*raw_mock_client_));
    EXPECT_CALL(*raw_mock_client_, send_(_, _, _))
        .WillRepeatedly(
            Invoke([this](Envoy::Http::MessagePtr& message,
                          Envoy::Http::AsyncClient::Callbacks& callback,
                          const Envoy::Http::AsyncClient::RequestOptions&) {
              call_count_++;
              message_.swap(message);
              client_callback_ = &callback;
              return nullptr;
            }));

    // TokenSubscriber must call `ready` to signal Init::Manager once it
    // finishes initializing.
    EXPECT_CALL(init_watcher_, ready()).WillRepeatedly(Invoke([this]() {
      init_ready_ = true;
    }));
    // Init::Manager should initialize its targets.
    init_target_handle->initialize(init_watcher_);
  }

  void checkRequestHeaders() {
    EXPECT_EQ(message_->headers()
                  .get(Envoy::Http::Headers::get().Method)
                  ->value()
                  .getStringView(),
              "POST");
    EXPECT_EQ(message_->headers()
                  .get(Envoy::Http::Headers::get().Host)
                  ->value()
                  .getStringView(),
              "iam");
    EXPECT_EQ(message_->headers()
                  .get(Envoy::Http::Headers::get().Path)
                  ->value()
                  .getStringView(),
              "/uri_suffix");
    EXPECT_EQ(
        message_->headers().get(kAuthorizationKey)->value().getStringView(),
        "Bearer access-token");
  }

  void checkRequestBody(const std::string& body_str) {
    EXPECT_EQ(message_->bodyAsString(), body_str);
  }

  bool init_ready_ = false;
  int call_count_ = 0;
  ::google::protobuf::RepeatedPtrField<std::string> scopes_;
  ::google::protobuf::RepeatedPtrField<std::string> delegates_;
  NiceMock<Init::ExpectableWatcherImpl> init_watcher_;
  NiceMock<MockFactoryContext> context_;
  MockFunction<std::string()> access_token_fn_;
  MockFunction<int(std::string)> id_token_callback_;
  Envoy::Http::MessagePtr message_;
  Envoy::Http::AsyncClient::Callbacks* client_callback_{};
  std::unique_ptr<NiceMock<Envoy::Http::MockAsyncClient>> raw_mock_client_;
  IamTokenSubscriberPtr iam_token_sub_;
};

TEST_F(IamTokenSubscriberTest, EmptyAccessToken) {
  EXPECT_CALL(access_token_fn_, Call()).WillRepeatedly(Return(""));
  setUp();

  // the client_callback_ it not called.
  EXPECT_CALL(id_token_callback_, Call(_)).Times(0);
  EXPECT_EQ(call_count_, 0);
  EXPECT_EQ(init_ready_, false);
}

TEST_F(IamTokenSubscriberTest, CallOnTokenUpdateOnSuccess) {
  EXPECT_CALL(access_token_fn_, Call())
      .Times(1)
      .WillRepeatedly(Return("access-token"));
  EXPECT_CALL(id_token_callback_, Call(std::string("id-token")));

  setUp();
  EXPECT_EQ(call_count_, 1);

  Envoy::Http::HeaderMapImplPtr headers{new Envoy::Http::TestHeaderMapImpl{
      {":status", "200"},
  }};

  // Send a good token
  Envoy::Http::MessagePtr response(
      new Envoy::Http::RequestMessageImpl(std::move(headers)));

  std::string str_body(R"({"token":"id-token"
  })");
  response->body().reset(
      new Buffer::OwnedImpl(str_body.data(), str_body.size()));

  client_callback_->onSuccess(std::move(response));
  EXPECT_EQ(init_ready_, true);
  checkRequestHeaders();
}

TEST_F(IamTokenSubscriberTest, DoNotCallOnTokenUpdateOnFailure) {
  EXPECT_CALL(access_token_fn_, Call())
      .Times(1)
      .WillRepeatedly(Return("access-token"));
  // the client_callback_ it not called.
  EXPECT_CALL(id_token_callback_, Call(_)).Times(0);
  setUp();
  EXPECT_EQ(call_count_, 1);

  // Send a bad token
  client_callback_->onFailure(Envoy::Http::AsyncClient::FailureReason::Reset);
  EXPECT_EQ(init_ready_, true);
  checkRequestHeaders();
}

TEST_F(IamTokenSubscriberTest, SetDelegatesAndScopes) {
  scopes_.Add("scope_foo");
  scopes_.Add("scope_bar");

  delegates_.Add("delegate_foo");
  delegates_.Add("delegate_bar");
  EXPECT_CALL(access_token_fn_, Call())
      .Times(1)
      .WillRepeatedly(Return("access-token"));
  // the client_callback_ it not called.
  EXPECT_CALL(id_token_callback_, Call(_)).Times(0);
  setUp();
  EXPECT_EQ(call_count_, 1);

  checkRequestHeaders();
  checkRequestBody(
      R"({"scope":["scope_foo","scope_bar"],"delegates":["projects/-/serviceAccounts/delegate_foo","projects/-/serviceAccounts/delegate_bar"]})");
}

TEST_F(IamTokenSubscriberTest, OnlySetDelegates) {
  delegates_.Add("delegate_foo");
  delegates_.Add("delegate_bar");
  EXPECT_CALL(access_token_fn_, Call())
      .Times(1)
      .WillRepeatedly(Return("access-token"));
  // the client_callback_ it not called.
  EXPECT_CALL(id_token_callback_, Call(_)).Times(0);
  setUp();
  EXPECT_EQ(call_count_, 1);

  checkRequestHeaders();
  checkRequestBody(
      R"({"delegates":["projects/-/serviceAccounts/delegate_foo","projects/-/serviceAccounts/delegate_bar"]})");
}

TEST_F(IamTokenSubscriberTest, OnlySetScopes) {
  scopes_.Add("scope_foo");
  scopes_.Add("scope_bar");

  EXPECT_CALL(access_token_fn_, Call())
      .Times(1)
      .WillRepeatedly(Return("access-token"));
  // the client_callback_ it not called.
  EXPECT_CALL(id_token_callback_, Call(_)).Times(0);
  setUp();
  EXPECT_EQ(call_count_, 1);

  checkRequestHeaders();
  checkRequestBody(R"({"scope":["scope_foo","scope_bar"]})");
}

}  // namespace
}  // namespace Utils
}  // namespace Extensions
}  // namespace Envoy
