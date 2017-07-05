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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <netflex/misc/output.hpp>

namespace netflex {

namespace misc {

//!
//! logging
//!
std::string
printable_header_list(const http::header_list_t& headers) {
  std::string headers_str;

  for (const auto& header : headers)
    headers_str += "[" + header.first + "]=" + header.second + " ";

  return headers_str;
}

std::string
printable_params_list(const routing::params_t& params) {
  std::string params_str;

  for (const auto& param : params)
    params_str += "[" + param.first + "]=" + param.second + " ";

  return params_str;
}


//!
//! http response formating
//!
std::string
header_list_to_http_packet(const http::header_list_t& headers) {
  std::string headers_str;

  for (const auto& header : headers)
    headers_str += header.first + ": " + header.second + "\r\n";
  headers_str += "\r\n";

  return headers_str;
}

std::string
status_line_to_http_packet(const std::string& http_version, unsigned int status_code, const std::string& reason_phrase) {
  return http_version + " " + std::to_string(status_code) + " " + reason_phrase + "\r\n";
}

} // namespace misc

} // namespace netflex
