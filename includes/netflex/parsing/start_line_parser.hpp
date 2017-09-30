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

#include <netflex/parsing/parser_iface.hpp>

namespace netflex {

namespace parsing {

//!
//! HTTP Start Line
//! https://tools.ietf.org/html/rfc7230#section-3.1
//!
//! start-line     = request-line / status-line
//! request-line   = method SP request-target SP HTTP-version CRLF
//! method         = token
//!

//!
//! start line parser
//!
class start_line_parser : public parser_iface {
public:
  //!
  //! default ctor
  //!
  //! \param request request to be initialized
  //!
  explicit start_line_parser(http::request& request);

  //! default dtor
  ~start_line_parser(void) = default;

  //! copy ctor
  start_line_parser(const start_line_parser&) = delete;
  //! assignment operator
  start_line_parser& operator=(const start_line_parser&) = delete;

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
  //! parsing state
  //! specify which part of the start-line is being parsed
  //!
  enum class state {
    method,
    target,
    http_version,
    trailing,
    done
  };

private:
  //!
  //! parse method
  //!
  //! \param buffer input data
  //! \return whether the method has been parsed or not
  //!
  bool fetch_method(std::string& buffer);

  //!
  //! parse target
  //!
  //! \param buffer input data
  //! \return whether the target has been parsed or not
  //!
  bool fetch_target(std::string& buffer);

  //!
  //! parse http version
  //!
  //! \param buffer input data
  //! \return whether the http version has been parsed or not
  //!
  bool fetch_http_version(std::string& buffer);

  //!
  //! parse the trailing characters (basically clear buffer)
  //!
  //! \param buffer input data
  //! \return whether the trailing characters have been fully parsed or not
  //!
  bool fetch_trailing(std::string& buffer);

private:
  //!
  //! parsed method
  //!
  std::string m_method;

  //!
  //! parsed target
  //!
  std::string m_target;

  //!
  //! parsed http version
  //!
  std::string m_http_version;

  //!
  //! keep track of last whitespace character erased in fetch trailing
  //!
  char m_last_consumed_whitespace;

  //!
  //! current state
  //!
  state m_state;
};

} // namespace parsing

} // namespace netflex
