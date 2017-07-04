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

#include <netflex/http/request.hpp>
#include <netflex/misc/output.hpp>

namespace netflex {

namespace http {

//!
//! start line information
//!
const std::string&
request::get_method(void) const {
  return m_method;
}

const std::string&
request::get_target(void) const {
  return m_target;
}

const std::string&
request::get_http_version(void) const {
  return m_http_version;
}

void
request::set_method(const std::string& method) {
  m_method = method;
}

void
request::set_target(const std::string& target) {
  m_target = target;
}

void
request::set_http_version(const std::string& http_version) {
  m_http_version = http_version;
}


//!
//! headers information
//!
const header_list_t&
request::get_headers(void) const {
  return m_headers;
}

void
request::set_headers(const header_list_t& headers) {
  m_headers = headers;
}

void
request::add_header(const header& header) {
  m_headers[header.field_name] = header.field_value;
}


//!
//! path & params
//!
const std::string&
request::get_path(void) const {
  return m_path;
}

const routing::params_t&
request::get_params(void) const {
  return m_params;
}

void
request::set_path(const std::string& path) {
  m_path = path;
}

void
request::set_params(const routing::params_t& params) {
  m_params = params;
}


//!
//! misc
//!
std::string
request::to_string(void) const {
  return m_method + " " + m_target + " " + m_http_version + " " + misc::printable_header_list(m_headers);
}

} // namespace http

} // namespace netflex
