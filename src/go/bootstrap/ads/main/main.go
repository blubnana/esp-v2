// Copyright 2018 Google Cloud Platform Proxy Authors
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

package main

import (
	"flag"
	"io/ioutil"
	"time"

	"cloudesf.googlesource.com/gcpproxy/src/go/bootstrap"
	"cloudesf.googlesource.com/gcpproxy/src/go/bootstrap/ads"
	"github.com/golang/glog"
	"github.com/golang/protobuf/jsonpb"
	"github.com/golang/protobuf/ptypes"
)

var (
	AdsConnectTimeout = flag.Duration("ads_connect_imeout", 10*time.Second, "ads connect timeout in seconds")
	EnableTracing     = flag.Bool("enable_tracing", false, "Enable stack driver tracing")
)

func main() {
	flag.Parse()
	out_path := flag.Arg(0)
	glog.Infof("Output path: %s", out_path)
	if out_path == "" {
		glog.Exitf("Please specify a path to write bootstrap config file")
	}

	connectTimeoutProto := ptypes.DurationProto(*AdsConnectTimeout)
	bt := ads.CreateBootstrapConfig(connectTimeoutProto)
	if *EnableTracing {
		var err error
		if bt.Tracing, err = bootstrap.CreateTracing(); err != nil {
			glog.Exitf("failed to create tracing config, error: %v", err)
		}
	}

	marshaler := &jsonpb.Marshaler{
		Indent: "  ",
	}
	json_str, _ := marshaler.MarshalToString(bt)
	err := ioutil.WriteFile(out_path, []byte(json_str), 0644)
	if err != nil {
		glog.Exitf("failed to write config to %v, error: %v", out_path, err)
	}
}
