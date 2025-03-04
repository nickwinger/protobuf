// Protocol Buffers - Google's data interchange format
// Copyright 2014 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <memory>

#include "google/protobuf/any.pb.h"
#include "google/protobuf/compiler/command_line_interface.h"
#include <gtest/gtest.h>
#include "google/protobuf/compiler/csharp/csharp_helpers.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/io/printer.h"
#include "google/protobuf/io/zero_copy_stream.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace csharp {
namespace {

TEST(CSharpEnumValue, PascalCasedPrefixStripping) {
  EXPECT_EQ("Bar", GetEnumValueName("Foo", "BAR"));
  EXPECT_EQ("BarBaz", GetEnumValueName("Foo", "BAR_BAZ"));
  EXPECT_EQ("Bar", GetEnumValueName("Foo", "FOO_BAR"));
  EXPECT_EQ("Bar", GetEnumValueName("Foo", "FOO__BAR"));
  EXPECT_EQ("BarBaz", GetEnumValueName("Foo", "FOO_BAR_BAZ"));
  EXPECT_EQ("BarBaz", GetEnumValueName("Foo", "Foo_BarBaz"));
  EXPECT_EQ("Bar", GetEnumValueName("FO_O", "FOO_BAR"));
  EXPECT_EQ("Bar", GetEnumValueName("FOO", "F_O_O_BAR"));
  EXPECT_EQ("Bar", GetEnumValueName("Foo", "BAR"));
  EXPECT_EQ("BarBaz", GetEnumValueName("Foo", "BAR_BAZ"));
  EXPECT_EQ("Foo", GetEnumValueName("Foo", "FOO"));
  EXPECT_EQ("Foo", GetEnumValueName("Foo", "FOO___"));
  // Identifiers can't start with digits
  EXPECT_EQ("_2Bar", GetEnumValueName("Foo", "FOO_2_BAR"));
  EXPECT_EQ("_2", GetEnumValueName("Foo", "FOO___2"));
}

TEST(DescriptorProtoHelpers, IsDescriptorProto) {
  EXPECT_TRUE(IsDescriptorProto(DescriptorProto::descriptor()->file()));
  EXPECT_FALSE(IsDescriptorProto(google::protobuf::Any::descriptor()->file()));
}

TEST(DescriptorProtoHelpers, IsDescriptorOptionMessage) {
  EXPECT_TRUE(IsDescriptorOptionMessage(FileOptions::descriptor()));
  EXPECT_FALSE(IsDescriptorOptionMessage(google::protobuf::Any::descriptor()));
  EXPECT_FALSE(IsDescriptorOptionMessage(DescriptorProto::descriptor()));
}

// https://github.com/protocolbuffers/protobuf/issues/13482
TEST(CSharpIdentifiers2, UnderscoresToCamelCase) {
    EXPECT_EQ("My.Scope._3", UnderscoresToCamelCase("my.scope._3", true, true));
    EXPECT_EQ("My.Scope.Fd3", UnderscoresToCamelCase("my.Scope.fd_3", true, true));
    EXPECT_EQ("My.Scope.Fd", UnderscoresToCamelCase("my._scope.Fd", true, true));
}

TEST(CSharpIdentifiers, UnderscoresToCamelCase) {
	EXPECT_EQ("FooBar", UnderscoresToCamelCase("Foo_Bar", true));
	EXPECT_EQ("fooBar", UnderscoresToCamelCase("FooBar", false));
	EXPECT_EQ("foo123", UnderscoresToCamelCase("foo_123", false));
	// remove leading underscores
	EXPECT_EQ("Foo123", UnderscoresToCamelCase("_Foo_123", true));
	// this one has slight unexpected output as it capitalises the first
	// letter after consuming the underscores, but this was the existing
	// behaviour so I have not changed it
	EXPECT_EQ("FooBar", UnderscoresToCamelCase("___fooBar", false));
	// leave a leading underscore for identifiers that would otherwise
	// be invalid because they would start with a digit
	EXPECT_EQ("_123Foo", UnderscoresToCamelCase("_123_foo", true));
	EXPECT_EQ("_123Foo", UnderscoresToCamelCase("___123_foo", true));
}

}  // namespace
}  // namespace csharp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
