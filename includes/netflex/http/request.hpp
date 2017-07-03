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

class request {
public:
  //! ctor & dtor
  request(void)  = default;
  ~request(void) = default;

  //! copy ctor & assignment operator
  request(const request&) = default;
  request& operator=(const request&) = default;

public:
  //! start line information
  const std::string& get_method(void) const;
  const std::string& get_target(void) const;
  const std::string& get_http_version(void) const;

  void set_method(const std::string& method);
  void set_target(const std::string& target);
  void set_http_version(const std::string& http_version);

public:
  //! headers information
  const header_list_t& get_headers(void) const;

  void set_headers(const header_list_t& headers);
  void add_header(const header& header);

public:
  //! misc
  std::string to_string(void) const;

private:
  //! start line information
  std::string m_method;
  std::string m_target;
  std::string m_http_version;
  //! headers
  header_list_t m_headers;
};

} // namespace http

} // namespace netflex
