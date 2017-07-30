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

#include <cstdlib>
#include <vector>

#include <netflex/misc/error.hpp>
#include <netflex/parsing/message_body_chuncked_parser.hpp>
#include <netflex/parsing/message_body_compress_parser.hpp>
#include <netflex/parsing/message_body_content_length_parser.hpp>
#include <netflex/parsing/message_body_deflate_parser.hpp>
#include <netflex/parsing/message_body_gzip_parser.hpp>
#include <netflex/parsing/message_body_parser.hpp>
#include <netflex/parsing/utils.hpp>

namespace netflex {

namespace parsing {

//!
//! ctor
//!
message_body_parser::message_body_parser(http::request& request)
: parser_iface(request)
, m_states(build_states_from_request_headers())
, m_current_parser(create_parser_from_state(m_states.front()))
, m_content_length(fetch_content_length()) {}


//!
//! parser_iface impl
//!
parser_iface&
message_body_parser::operator<<(std::string& str) {
  while (!is_done() && parse_body(str))
    ;

  return *this;
}

bool
message_body_parser::is_done(void) const {
  return m_states.front() == state::done;
}


//!
//! parse body by delegating to other appropriate parsers
//!
bool
message_body_parser::parse_body(std::string& str) {
  //! feed current parser
  *m_current_parser << str;

  if (m_current_parser->is_done()) {
    //! switch to next state
    m_states.pop_front();
    m_current_parser = create_parser_from_state(m_states.front());

    return true;
  }

  return false;
}

//!
//! build states list from request headers
//!
std::list<message_body_parser::state>
message_body_parser::build_states_from_request_headers(void) const {
  std::list<state> states;

  //! if both content length and encoding are provided, content length should be discarded
  if (m_request.has_header("Content-Length") && m_request.has_header("Transfer-Encoding")) {
    m_request.remove_header("Content-Length");
  }

  //! content length header
  if (m_request.has_header("Content-Length")) {
    states.push_back(state::content_length);
  }

  if (m_request.has_header("Transfer-Encoding")) {
    std::vector<std::string> encodings = utils::split(m_request.get_header("Transfer-Encoding"), ',');

    for (auto& encoding : encodings) {
      utils::trim(encoding);
      utils::to_lower(encoding);

      if (encoding == "chuncked") {
        states.push_back(state::chuncked);
      }
      else if (encoding == "compress" || encoding == "x-compress") {
        states.push_back(state::compress);
      }
      else if (encoding == "deflate") {
        states.push_back(state::deflate);
      }
      else if (encoding == "gzip" || encoding == "x-gzip") {
        states.push_back(state::gzip);
      }
      else {
        __NETFLEX_THROW(error, "unsupported transfer encoding: " + encoding);
      }
    }
  }

  states.push_back(state::done);

  return states;
}


//!
//! fetch content length
//!
unsigned int
message_body_parser::fetch_content_length(void) const {
  if (m_request.has_header("Content-Length")) {
    return std::strtoul(m_request.get_header("Content-Length").c_str(), nullptr, 10);
  }

  return 0;
}


//!
//! create parser from given state
//!
std::unique_ptr<parser_iface>
message_body_parser::create_parser_from_state(state s) const {
  switch (s) {
  case state::content_length:
    return std::unique_ptr<parser_iface>(new message_body_content_length_parser(m_request));
  case state::chuncked:
    return std::unique_ptr<parser_iface>(new message_body_chuncked_parser(m_request));
  case state::compress:
    return std::unique_ptr<parser_iface>(new message_body_compress_parser(m_request));
  case state::deflate:
    return std::unique_ptr<parser_iface>(new message_body_deflate_parser(m_request));
  case state::gzip:
    return std::unique_ptr<parser_iface>(new message_body_gzip_parser(m_request));
  case state::done:
    return std::unique_ptr<parser_iface>(nullptr);
  default:
    __NETFLEX_THROW(error, "create_parser received invalid encoding type");
  }
}

} // namespace parsing

} // namespace netflex
