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
#include <netflex/parsing/request_parser.hpp>

namespace netflex {

namespace parsing {

//!
//! ctor & dtor
//!
request_parser::request_parser(void)
: m_current_stage(parsing_stage::start_line)
, m_current_parser(create_parser(m_current_stage)) {}


//!
//! add data to the parser
//!
request_parser&
request_parser::operator<<(const std::string& data) {
  m_buffer += data;

  while (build_request())
    ;

  return *this;
}


//!
//! build request
//!
bool
request_parser::build_request(void) {
  if (!m_buffer.size())
    return false;

  //! feed current parser
  *m_current_parser << m_buffer;

  if (m_current_parser->is_done()) {
    //! apply parser result to current request
    m_current_parser->apply(m_current_request);

    //! request fully built
    if (m_current_stage == parsing_stage::message_body) {
      //! store request as available
      m_available_requests.push_back(m_current_request);
      //! reset current request
      m_current_request = {};
    }

    //! switch to next stage
    m_current_parser = switch_to_next_stage(m_current_stage);

    return true;
  }

  return false;
}


//!
//! get request
//!
void
request_parser::operator>>(http::request& request) {
  request = get_front();
}

const http::request&
request_parser::get_front(void) const {
  if (!request_available())
    __NETFLEX_THROW(error, "No available request");

  return m_available_requests.front();
}

void
request_parser::pop_front(void) {
  if (!request_available())
    __NETFLEX_THROW(error, "No available request");

  m_available_requests.pop_front();
}


//!
//! get incomplete request currently being parsed
//!
const http::request&
request_parser::get_currently_parsed_request(void) const {
  return m_current_request;
}


//!
//! returns whether a request is available
//!
bool
request_parser::request_available(void) const {
  return !m_available_requests.empty();
}

} // namespace parsing

} // namespace netflex
