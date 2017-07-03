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

#include <netflex/misc/error.hpp>
#include <netflex/parsing/header_fields_parser.hpp>
#include <netflex/parsing/utils.hpp>

namespace netflex {

namespace parsing {

//!
//! ctor & dtor
//!
header_fields_parser::header_fields_parser(void)
: m_state(state::empty_line) {}


//!
//! parser_iface impl
//!
parser_iface&
header_fields_parser::operator<<(std::string& buffer) {
  while (!is_done()) {
    //! try to fetch empty line
    //! return false if not enough bytes in the buffer
    //! return true if was able to determine whether empty line or not was present
    //! successful finding of empty line can be checked through status (switch to done or to header)
    if (!fetch_empty_line(buffer))
      return *this;

    //! try to parse header
    if (!fetch_header(buffer))
      return *this;
  }

  return *this;
}

bool
header_fields_parser::is_done(void) const {
  return m_state == state::done;
}

void
header_fields_parser::apply(http::request& request) const {
  request.set_headers(m_headers);
}


//!
//! parse headers list
//!
bool
header_fields_parser::fetch_empty_line(std::string& buffer) {
  if (m_state > state::empty_line)
    return true;

  //! if we do not have enough bytes, no point to check for CRLF, we need to wait for more bytes
  if (buffer.size() < 2)
    return false;

  //! if we got crlf, then we have no more headers to parse and can switch to done state
  //! otherwise, he still have headers to parse and need to switch in header_field state
  m_state = utils::consume_crlf(buffer) ? state::done : state::header_field;

  //! return true in either case
  //! true only means we had enough bytes to make a decision
  //! not whether or not an empty line was detected or not
  return true;
}

bool
header_fields_parser::fetch_header(std::string& buffer) {
  if (m_state > state::done)
    return true;

  //! feed parser and try to build header
  m_parser << buffer;

  if (!m_parser.is_done())
    return false;

  //! grab header and store it here
  http::header header          = m_parser.get_header();
  m_headers[header.field_name] = header.field_value;

  //! reset parser for reuse
  m_parser.reset();

  //! try to find empty line
  m_state = state::empty_line;

  return true;
}

} // namespace parsing

} // namespace netflex
