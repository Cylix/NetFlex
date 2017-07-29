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

//! HTTP Start Line
//! https://tools.ietf.org/html/rfc7230#section-3.1
//!
//! start-line     = request-line / status-line
//! request-line   = method SP request-target SP HTTP-version CRLF
//! method         = token

class start_line_parser : public parser_iface {
public:
  //! ctor & dtor
  start_line_parser(http::request& request);
  ~start_line_parser(void) = default;

  //! copy ctor & assignment operator
  start_line_parser(const start_line_parser&) = delete;
  start_line_parser& operator=(const start_line_parser&) = delete;

public:
  //! parser_iface impl
  parser_iface& operator<<(std::string&);
  bool is_done(void) const;

private:
  //! parsing state
  enum class state {
    method,
    target,
    http_version,
    trailing,
    done
  };

private:
  //! retrieve start line information
  bool fetch_method(std::string& buffer);
  bool fetch_target(std::string& buffer);
  bool fetch_http_version(std::string& buffer);
  bool fetch_trailing(std::string& buffer);

private:
  //! information retrieved
  std::string m_method;
  std::string m_target;
  std::string m_http_version;
  //! keep track of last whitespace character erased in fetch trailing
  char m_last_consumed_whitespace;
  //! current state
  state m_state;
};

} // namespace parsing

} // namespace netflex
