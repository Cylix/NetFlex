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

#include <netflex/http/method.hpp>

namespace netflex {

namespace http {

method
method_from_string(const std::string& str) {
  if (str == "OPTIONS")
    return method::OPTIONS;

  if (str == "HEAD")
    return method::HEAD;

  if (str == "GET")
    return method::GET;

  if (str == "POST")
    return method::POST;

  if (str == "PUT")
    return method::PUT;

  if (str == "PATCH")
    return method::PATCH;

  if (str == "DELETE")
    return method::DELETE;

  if (str == "TRACE")
    return method::TRACE;

  if (str == "CONNECT")
    return method::CONNECT;

  return method::unknown;
}

std::string
method_to_string(method m) {
  switch (m) {
  case method::OPTIONS:
    return "OPTIONS";

  case method::HEAD:
    return "HEAD";

  case method::GET:
    return "GET";

  case method::POST:
    return "POST";

  case method::PUT:
    return "PUT";

  case method::PATCH:
    return "PATCH";

  case method::DELETE:
    return "DELETE";

  case method::TRACE:
    return "TRACE";

  case method::CONNECT:
    return "CONNECT";

  default:
    return "";
  }
}

} // namespace http

} // namespace netflex
