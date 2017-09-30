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

#include <deque>
#include <memory>
#include <string>

#include <netflex/http/request.hpp>
#include <netflex/parsing/parser_iface.hpp>
#include <netflex/parsing/parsers_factory.hpp>

namespace netflex {

namespace parsing {

//!
//! request parser
//! connect all parsers between each other and make sure the right order is followed
//! can handle multiple requests
//!

class request_parser {
public:
  //! default ctor
  request_parser(void);
  //! default dtor
  ~request_parser(void) = default;

  //! copy ctor
  request_parser(const request_parser&) = delete;
  //! assignment operator
  request_parser& operator=(const request_parser&) = delete;

public:
  //!
  //! add data to the parser. This data will be used for parsing.
  //!
  //! \param data data to feed the parser
  //! \return reference to the current object
  //!
  request_parser& operator<<(const std::string& data);

  //!
  //! same as get_front
  //!
  //! \param request object where to store the request
  //!
  void operator>>(http::request& request);

  //!
  //! \return the first available request. Throws if no request is available
  //!
  const http::request& get_front(void) const;

  //!
  //! remove the first available request. Throws if no request is available
  //!
  void pop_front(void);

  //!
  //! \return incomplete request currently being parsed
  //!
  const http::request& get_currently_parsed_request(void) const;

  //!
  //! \return whether a request is available
  //!
  bool request_available(void) const;

private:
  //!
  //! build request
  //!
  //! \return whether the request is fully built
  //!
  bool build_request(void);

private:
  //!
  //! buffer
  //!
  std::string m_buffer;

  //!
  //! request currently being built
  //!
  http::request m_current_request;

  //!
  //! current parsing state
  //!
  parsing_stage m_current_stage;

  //!
  //! current parser
  //!
  std::unique_ptr<parser_iface> m_current_parser;

  //!
  //! parsed requests, ready for dequeing
  //!
  std::deque<http::request> m_available_requests;
};

} // namespace parsing

} // namespace netflex
