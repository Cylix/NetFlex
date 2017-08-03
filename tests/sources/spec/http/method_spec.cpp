// The MIT License (MIT)
//
// Copyright (c) 2015-2017 Simon Ninon <simon.ninon@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <gtest/gtest.h>

#include <netflex/netflex>

//!
//! method_from_string
//!
TEST(method, method_from_string) {
  EXPECT_EQ(netflex::http::method_from_string("OPTIONS"), netflex::http::method::OPTIONS);
  EXPECT_EQ(netflex::http::method_from_string("HEAD"), netflex::http::method::HEAD);
  EXPECT_EQ(netflex::http::method_from_string("GET"), netflex::http::method::GET);
  EXPECT_EQ(netflex::http::method_from_string("POST"), netflex::http::method::POST);
  EXPECT_EQ(netflex::http::method_from_string("PUT"), netflex::http::method::PUT);
  EXPECT_EQ(netflex::http::method_from_string("PATCH"), netflex::http::method::PATCH);
  EXPECT_EQ(netflex::http::method_from_string("DELETE"), netflex::http::method::DELETE);
  EXPECT_EQ(netflex::http::method_from_string("TRACE"), netflex::http::method::TRACE);
  EXPECT_EQ(netflex::http::method_from_string("CONNECT"), netflex::http::method::CONNECT);
  EXPECT_EQ(netflex::http::method_from_string("YOLO"), netflex::http::method::unknown);
}


//!
//! method_to_string
//!
TEST(method, method_to_string) {
  EXPECT_EQ(netflex::http::method_to_string(netflex::http::method::OPTIONS), "OPTIONS");
  EXPECT_EQ(netflex::http::method_to_string(netflex::http::method::HEAD), "HEAD");
  EXPECT_EQ(netflex::http::method_to_string(netflex::http::method::GET), "GET");
  EXPECT_EQ(netflex::http::method_to_string(netflex::http::method::POST), "POST");
  EXPECT_EQ(netflex::http::method_to_string(netflex::http::method::PUT), "PUT");
  EXPECT_EQ(netflex::http::method_to_string(netflex::http::method::PATCH), "PATCH");
  EXPECT_EQ(netflex::http::method_to_string(netflex::http::method::DELETE), "DELETE");
  EXPECT_EQ(netflex::http::method_to_string(netflex::http::method::TRACE), "TRACE");
  EXPECT_EQ(netflex::http::method_to_string(netflex::http::method::CONNECT), "CONNECT");
  EXPECT_EQ(netflex::http::method_to_string(netflex::http::method::unknown), "");
  EXPECT_EQ(netflex::http::method_to_string((netflex::http::method) 42), "");
}
