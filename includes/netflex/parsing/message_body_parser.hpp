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

#pragma once

#include <list>
#include <memory>

#include <netflex/parsing/parser_iface.hpp>

namespace netflex {

namespace parsing {

//!
//! parser for body
//!
class message_body_parser : public parser_iface {
public:
  //!
  //! default ctor
  //!
  //! \param request request to be initialized
  //!
  explicit message_body_parser(http::request& request);

  //! default dtor
  ~message_body_parser(void) = default;

  //! copy ctor
  message_body_parser(const message_body_parser&) = delete;
  //! assignment operator
  message_body_parser& operator=(const message_body_parser&) = delete;

public:
  //!
  //! consume input data to parse it and init the request
  //! if not enough data is passed in, this method would need to be called again later
  //! input data is modified whenever a token is consumed by parsing, even if parsing is incomplete or invalid
  //! invalid data would lead to a raised exception
  //!
  //! \param data input data to be parsed
  //! \return reference to the current object
  //!
  parser_iface& operator<<(std::string& data);

  //!
  //! \return whether the parsing is done or not
  //!
  bool is_done(void) const;

private:
  //!
  //! parse body by delegating to other appropriate parsers
  //!
  //! \param data input data
  //! \return whether the body has been fully parsed or not
  //!
  bool parse_body(std::string& data);

private:
  //!
  //! parsing state
  //! specify which part of the header field is being parsed
  //!
  enum class state {
    content_length,
    chuncked,
    compress,
    deflate,
    gzip,
    done
  };

  //!
  //! build states list from request headers
  //!
  //! \return list of states
  //!
  std::list<state> build_states_from_request_headers(void) const;

  //!
  //! create parser from given state
  //!
  //! \param s state to be used to build the appropriate parser
  //! \return appropriate parser
  //!
  std::unique_ptr<parser_iface> create_parser_from_state(state s) const;

private:
  //!
  //! list of states to process, current state is head of list
  //!
  std::list<state> m_states;

  //!
  //! current parser
  //!
  std::unique_ptr<parser_iface> m_current_parser;
};

} // namespace parsing

} // namespace netflex
