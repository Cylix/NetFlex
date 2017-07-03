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

#include <string>

#include <netflex/http/header.hpp>

namespace netflex {

namespace http {

class response {
public:
  //! ctor & dtor
  response(void);
  ~response(void) = default;

  //! copy ctor & assignment operator
  response(const response&) = default;
  response& operator=(const response&) = default;

public:
  //! status line
  const std::string& get_http_version(void) const;
  unsigned int get_status_code(void) const;
  const std::string& get_reason_phase(void) const;

  void set_http_version(const std::string& version);
  void set_status_code(unsigned int code);
  void set_reason_phrase(const std::string& reason);

public:
  //! headers
  const header_list_t& get_headers(void) const;
  void add_header(const header& header);

  void set_headers(const header_list_t& headers);

public:
  //! body
  const std::string& get_body(void) const;

  void set_body(const std::string& body);

public:
  //! convert response to http packet
  std::string to_http_packet(void) const;

private:
  //! start line
  std::string m_http_version;
  unsigned int m_status;
  std::string m_reason;
  //! headers
  header_list_t m_headers;
  //! body
  std::string m_body;
};

} // namespace http

} // namespace netflex
