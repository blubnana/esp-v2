// Code generated by protoc-gen-go. DO NOT EDIT.
// source: api/envoy/http/backend_routing/config.proto

package google_api_envoy_http_backend_routing

import (
	fmt "fmt"
	_ "github.com/envoyproxy/protoc-gen-validate/validate"
	proto "github.com/golang/protobuf/proto"
	math "math"
)

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.ProtoPackageIsVersion3 // please upgrade the proto package

type BackendRoutingRule struct {
	Operation            string   `protobuf:"bytes,1,opt,name=operation,proto3" json:"operation,omitempty"`
	IsConstAddress       bool     `protobuf:"varint,2,opt,name=is_const_address,json=isConstAddress,proto3" json:"is_const_address,omitempty"`
	PathPrefix           string   `protobuf:"bytes,3,opt,name=path_prefix,json=pathPrefix,proto3" json:"path_prefix,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *BackendRoutingRule) Reset()         { *m = BackendRoutingRule{} }
func (m *BackendRoutingRule) String() string { return proto.CompactTextString(m) }
func (*BackendRoutingRule) ProtoMessage()    {}
func (*BackendRoutingRule) Descriptor() ([]byte, []int) {
	return fileDescriptor_c7e3bdb01c1a7a25, []int{0}
}

func (m *BackendRoutingRule) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_BackendRoutingRule.Unmarshal(m, b)
}
func (m *BackendRoutingRule) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_BackendRoutingRule.Marshal(b, m, deterministic)
}
func (m *BackendRoutingRule) XXX_Merge(src proto.Message) {
	xxx_messageInfo_BackendRoutingRule.Merge(m, src)
}
func (m *BackendRoutingRule) XXX_Size() int {
	return xxx_messageInfo_BackendRoutingRule.Size(m)
}
func (m *BackendRoutingRule) XXX_DiscardUnknown() {
	xxx_messageInfo_BackendRoutingRule.DiscardUnknown(m)
}

var xxx_messageInfo_BackendRoutingRule proto.InternalMessageInfo

func (m *BackendRoutingRule) GetOperation() string {
	if m != nil {
		return m.Operation
	}
	return ""
}

func (m *BackendRoutingRule) GetIsConstAddress() bool {
	if m != nil {
		return m.IsConstAddress
	}
	return false
}

func (m *BackendRoutingRule) GetPathPrefix() string {
	if m != nil {
		return m.PathPrefix
	}
	return ""
}

type FilterConfig struct {
	Rules                []*BackendRoutingRule `protobuf:"bytes,1,rep,name=rules,proto3" json:"rules,omitempty"`
	XXX_NoUnkeyedLiteral struct{}              `json:"-"`
	XXX_unrecognized     []byte                `json:"-"`
	XXX_sizecache        int32                 `json:"-"`
}

func (m *FilterConfig) Reset()         { *m = FilterConfig{} }
func (m *FilterConfig) String() string { return proto.CompactTextString(m) }
func (*FilterConfig) ProtoMessage()    {}
func (*FilterConfig) Descriptor() ([]byte, []int) {
	return fileDescriptor_c7e3bdb01c1a7a25, []int{1}
}

func (m *FilterConfig) XXX_Unmarshal(b []byte) error {
	return xxx_messageInfo_FilterConfig.Unmarshal(m, b)
}
func (m *FilterConfig) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	return xxx_messageInfo_FilterConfig.Marshal(b, m, deterministic)
}
func (m *FilterConfig) XXX_Merge(src proto.Message) {
	xxx_messageInfo_FilterConfig.Merge(m, src)
}
func (m *FilterConfig) XXX_Size() int {
	return xxx_messageInfo_FilterConfig.Size(m)
}
func (m *FilterConfig) XXX_DiscardUnknown() {
	xxx_messageInfo_FilterConfig.DiscardUnknown(m)
}

var xxx_messageInfo_FilterConfig proto.InternalMessageInfo

func (m *FilterConfig) GetRules() []*BackendRoutingRule {
	if m != nil {
		return m.Rules
	}
	return nil
}

func init() {
	proto.RegisterType((*BackendRoutingRule)(nil), "google.api.envoy.http.backend_routing.BackendRoutingRule")
	proto.RegisterType((*FilterConfig)(nil), "google.api.envoy.http.backend_routing.FilterConfig")
}

func init() {
	proto.RegisterFile("api/envoy/http/backend_routing/config.proto", fileDescriptor_c7e3bdb01c1a7a25)
}

var fileDescriptor_c7e3bdb01c1a7a25 = []byte{
	// 253 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0x8c, 0x8f, 0x41, 0x4a, 0xc4, 0x30,
	0x14, 0x86, 0xc9, 0x54, 0xc5, 0x66, 0x44, 0x24, 0x1b, 0x8b, 0x1b, 0xcb, 0x80, 0x58, 0x10, 0x52,
	0xd0, 0x95, 0x4b, 0x67, 0xc0, 0xad, 0x92, 0x0b, 0x94, 0x4c, 0xfb, 0xa6, 0xf3, 0x30, 0xe4, 0x85,
	0x24, 0x1d, 0xf4, 0x06, 0x9e, 0xc9, 0x95, 0xd7, 0xf1, 0x16, 0xd2, 0x06, 0x11, 0x74, 0x33, 0xbb,
	0xf0, 0xe5, 0x7d, 0x3f, 0x7c, 0xfc, 0x46, 0x3b, 0xac, 0xc1, 0xee, 0xe8, 0xad, 0xde, 0xc6, 0xe8,
	0xea, 0xb5, 0x6e, 0x5f, 0xc0, 0x76, 0x8d, 0xa7, 0x21, 0xa2, 0xed, 0xeb, 0x96, 0xec, 0x06, 0x7b,
	0xe9, 0x3c, 0x45, 0x12, 0x57, 0x3d, 0x51, 0x6f, 0x40, 0x6a, 0x87, 0x72, 0x72, 0xe4, 0xe8, 0xc8,
	0x3f, 0xce, 0xc5, 0xf9, 0x4e, 0x1b, 0xec, 0x74, 0x84, 0xfa, 0xe7, 0x91, 0xfc, 0xc5, 0x3b, 0xe3,
	0x62, 0x99, 0x8e, 0x55, 0xba, 0x55, 0x83, 0x01, 0x71, 0xcd, 0x73, 0x72, 0xe0, 0x75, 0x44, 0xb2,
	0x05, 0x2b, 0x59, 0x95, 0x2f, 0xf3, 0x8f, 0xaf, 0xcf, 0xec, 0xc0, 0xcf, 0x4a, 0xa6, 0x7e, 0xff,
	0x44, 0xc5, 0xcf, 0x30, 0x34, 0x2d, 0xd9, 0x10, 0x1b, 0xdd, 0x75, 0x1e, 0x42, 0x28, 0x66, 0x25,
	0xab, 0x8e, 0xd5, 0x29, 0x86, 0xd5, 0x88, 0x1f, 0x12, 0x15, 0x97, 0x7c, 0xee, 0x74, 0xdc, 0x36,
	0xce, 0xc3, 0x06, 0x5f, 0x8b, 0x6c, 0x1c, 0x55, 0x7c, 0x44, 0xcf, 0x13, 0x59, 0x34, 0xfc, 0xe4,
	0x11, 0x4d, 0x04, 0xbf, 0x9a, 0x02, 0xc5, 0x13, 0x3f, 0xf4, 0x83, 0x81, 0x50, 0xb0, 0x32, 0xab,
	0xe6, 0xb7, 0xf7, 0x72, 0xaf, 0x54, 0xf9, 0xbf, 0x46, 0xa5, 0x9d, 0xf5, 0xd1, 0x94, 0x7c, 0xf7,
	0x1d, 0x00, 0x00, 0xff, 0xff, 0x8f, 0x8c, 0x28, 0x92, 0x61, 0x01, 0x00, 0x00,
}