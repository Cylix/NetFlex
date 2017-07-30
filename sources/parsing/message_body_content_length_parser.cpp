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

#include <algorithm>
#include <cstdlib>

#include <netflex/parsing/message_body_content_length_parser.hpp>

namespace netflex {

namespace parsing {

//!
//! ctor
//!
message_body_content_length_parser::message_body_content_length_parser(http::request& request)
: parser_iface(request)
, m_content_length(fetch_content_length()) {}


//!
//! parser_iface impl
//!
parser_iface&
message_body_content_length_parser::operator<<(std::string& buffer) {
  fetch_body(buffer);

  return *this;
}

bool
message_body_content_length_parser::is_done(void) const {
  return m_body.length() == m_content_length;
}


//!
//! fetch body
//!
void
message_body_content_length_parser::fetch_body(std::string& buffer) {
  if (is_done()) {
    return;
  }

  std::size_t remaining        = m_content_length - m_body.length();
  std::size_t nb_bytes_to_read = std::min(remaining, buffer.length());

  //! fetch iterator to last byte to read in buffer
  auto buffer_last_byte_it = buffer.begin();
  std::advance(buffer_last_byte_it, nb_bytes_to_read);

  //! fetch bytes from buffer
  m_body.insert(m_body.end(), buffer.begin(), buffer_last_byte_it);
  //! erase bytes from buffer
  buffer.erase(buffer.begin(), buffer_last_byte_it);

  if (is_done()) {
    //! store body in request
    m_request.set_body(m_body);
  }
}


//!
//! fetch content length
//!
unsigned int
message_body_content_length_parser::fetch_content_length(void) const {
  if (m_request.has_header("Content-Length")) {
    return std::strtoul(m_request.get_header("Content-Length").c_str(), nullptr, 10);
  }

  return 0;
}

} // namespace parsing

} // namespace netflex
