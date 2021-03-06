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

#pragma once

#include "common/common/logger.h"
#include "common/init/target_impl.h"
#include "envoy/common/pure.h"
#include "envoy/common/time.h"
#include "envoy/event/dispatcher.h"
#include "envoy/http/async_client.h"
#include "envoy/http/message.h"
#include "envoy/server/filter_config.h"
#include "envoy/upstream/cluster_manager.h"
#include "src/envoy/utils/json_struct.h"

namespace Envoy {
namespace Extensions {
namespace Utils {

// Required header when fetching from the iam server
extern const Envoy::Http::LowerCaseString kAuthorizationKey;

// `IamTokenSubscriber` class fetches id token from IAM server, and it depends
// on access_token_.
class IamTokenSubscriber
    : public Envoy::Http::AsyncClient::Callbacks,
      public Envoy::Logger::Loggable<Envoy::Logger::Id::filter> {
 public:
  using TokenUpdateFunc = std::function<void(const std::string& token)>;
  using TokenGetFunc = std::function<std::string()>;

  enum TokenType { AccessToken, IdentityToken };

  IamTokenSubscriber(
      Envoy::Server::Configuration::FactoryContext& context,
      TokenGetFunc access_token_fn, const std::string& iam_service_cluster,
      const std::string& iam_service_uri, TokenType token_type,
      const ::google::protobuf::RepeatedPtrField<std::string>& delegates,
      const ::google::protobuf::RepeatedPtrField<std::string>& scopes,
      TokenUpdateFunc callback);
  virtual ~IamTokenSubscriber();

 private:
  // Envoy::Http::AsyncClient::Callbacks
  void onSuccess(Envoy::Http::MessagePtr&& response) override;
  void onFailure(Envoy::Http::AsyncClient::FailureReason reason) override;

  void refresh();
  void processResponse(Envoy::Http::MessagePtr&& response);
  void processAccessTokenResp(JsonStruct& json_struct);
  void processIdentityTokenResp(JsonStruct& json_struct);
  void resetTimer(const std::chrono::milliseconds& ms);

  Upstream::ClusterManager& cm_;
  TokenGetFunc access_token_fn_;
  const std::string& iam_service_cluster_;
  const std::string iam_service_uri_;

  TokenType token_type_;
  const std::string request_name_;
  const ::google::protobuf::RepeatedPtrField<std::string>& delegates_;
  const ::google::protobuf::RepeatedPtrField<std::string> scopes_;

  TokenUpdateFunc callback_;
  Envoy::Http::AsyncClient::Request* active_request_{};

  Envoy::Event::TimerPtr refresh_timer_;
  // init_target_.ready() need be called at the end of request callbacks.
  Envoy::Init::TargetImpl init_target_;
};
typedef std::unique_ptr<IamTokenSubscriber> IamTokenSubscriberPtr;

}  // namespace Utils
}  // namespace Extensions
}  // namespace Envoy
