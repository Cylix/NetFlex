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

#include <netflex/http/response.hpp>
#include <netflex/misc/output.hpp>

namespace netflex {

namespace http {

//!
//! ctor & dtor
//!
response::response(void)
: m_http_version("HTTP/1.1")
, m_status(200)
, m_reason("OK") {}


//!
//! convert response to http packet
//!
std::string
response::to_http_packet(void) const {
  return m_http_version + " " + std::to_string(m_status) + " " + m_reason + "\r\n" + misc::header_list_to_http_packet(m_headers) + m_body;
}


//!
//! status line
//!
const std::string&
response::get_http_version(void) const {
  return m_http_version;
}

unsigned int
response::get_status_code(void) const {
  return m_status;
}

const std::string&
response::get_reason_phase(void) const {
  return m_reason;
}

void
response::set_http_version(const std::string& version) {
  m_http_version = version;
}

void
response::set_status_code(unsigned int code) {
  m_status = code;
}

void
response::set_reason_phrase(const std::string& reason) {
  m_reason = reason;
}


//!
//! headers
//!
const header_list_t&
response::get_headers(void) const {
  return m_headers;
}

void
response::add_header(const header& header) {
  m_headers[header.field_name] = header.field_value;
}

void
response::set_headers(const header_list_t& headers) {
  m_headers = headers;
}


//!
//! body
//!
const std::string&
response::get_body(void) const {
  return m_body;
}

void
response::set_body(const std::string& body) {
  m_body = body;
}

} // namespace http

} // namespace netflex
