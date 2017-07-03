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
#include <netflex/parsing/start_line_parser.hpp>
#include <netflex/parsing/utils.hpp>

namespace netflex {

namespace parsing {

//!
//! ctor & dtor
//!
start_line_parser::start_line_parser(void)
: m_last_consumed_whitespace(0)
, m_state(state::method) {}


//!
//! parser_iface impl
//!
parser_iface&
start_line_parser::operator<<(std::string& buffer) {
  if (!buffer.size())
    return *this;

  if (!fetch_method(buffer))
    return *this;

  if (!fetch_target(buffer))
    return *this;

  if (!fetch_http_version(buffer))
    return *this;

  if (!fetch_trailing(buffer))
    return *this;

  return *this;
}

bool
start_line_parser::is_done(void) const {
  return m_state == state::done;
}

void
start_line_parser::apply(http::request& request) const {
  request.set_method(m_method);
  request.set_target(m_target);
  request.set_http_version(m_http_version);
}


//!
//! retrieve start line information
//!
bool
start_line_parser::fetch_method(std::string& buffer) {
  if (m_state > state::method)
    return true;

  if (utils::parse_next_word(buffer, m_method)) {
    //! we can process to next state
    m_state = state::target;
    return true;
  }

  return false;
}

bool
start_line_parser::fetch_target(std::string& buffer) {
  if (m_state > state::target)
    return true;

  if (utils::parse_next_word(buffer, m_target)) {
    //! we can process to next state
    m_state = state::http_version;
    return true;
  }

  return false;
}

bool
start_line_parser::fetch_http_version(std::string& buffer) {
  if (m_state > state::http_version)
    return true;

  if (utils::parse_next_word(buffer, m_http_version)) {
    //! we can process to next state
    m_state = state::trailing;
    return true;
  }

  return false;
}

bool
start_line_parser::fetch_trailing(std::string& buffer) {
  if (m_state > state::trailing)
    return true;

  //! dismiss preceding whitespaces
  if (!buffer.empty() && utils::is_whitespace_delimiter(buffer[0]))
    m_last_consumed_whitespace = utils::consume_whitespaces(buffer);

  //! if buffer has no character, then it can't have the trailing LF character
  if (buffer.empty())
    return false;

  //! if we are here, it is because we consumed all whitespaces (including CR)
  //! and that there are still bytes in the buffer
  //! so, if the first remaining byte is not LF, then the start-line is not ending correctly
  if (m_last_consumed_whitespace == utils::CR && buffer[0] != utils::LF)
    __NETFLEX_THROW(error, "Invalid start-line");

  //! consume LF
  buffer.erase(0, 1);
  //! process to next step
  m_state = state::done;
  return true;
}

} // namespace parsing

} // namespace netflex
