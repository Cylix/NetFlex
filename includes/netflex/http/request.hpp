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
#include <netflex/http/method.hpp>
#include <netflex/routing/params.hpp>

namespace netflex {

namespace http {

//!
//! http request representation
//! contains all information related to the received http request
//!
class request {
public:
  //! default ctor
  request(void) = default;
  //! default dtor
  ~request(void) = default;

  //! copy ctor
  request(const request&) = default;
  //! assignment operator
  request& operator=(const request&) = default;

public:
  //!
  //! \return request http verb
  //!
  method get_method(void) const;

  //!
  //! \return request http verb (string version)
  //!
  const std::string& get_raw_method(void) const;

  //!
  //! \return status line target
  //!
  const std::string& get_target(void) const;

  //!
  //! \return request http version
  //!
  const std::string& get_http_version(void) const;

  //!
  //! set new http verb
  //!
  //! \param method new http verb
  //!
  void set_method(method method);

  //!
  //! set new http verb (string version)
  //!
  //! \param method new http verb
  //!
  void set_raw_method(const std::string& method);

  //!
  //! set new status line target
  //!
  //! \param target new status line target
  //!
  void set_target(const std::string& target);

  //!
  //! set new http version
  //!
  //! \param http_version new http version
  //!
  void set_http_version(const std::string& http_version);

public:
  //!
  //! return specific header
  //! throws an exception if header does not exist
  //!
  //! \param name header name to get
  //! \return requested header value
  //!
  const std::string& get_header(const std::string& name) const;

  //!
  //! \return all headers for request
  //!
  const header_list_t& get_headers(void) const;

  //!
  //! set request headers
  //!
  //! \param headers new headers for request
  //!
  void set_headers(const header_list_t& headers);

  //!
  //! add a new header to the request header
  //! if header already exists, override its value
  //!
  //! \param header header to be added
  //!
  void add_header(const header& header);

  //!
  //! return whether the request contains a specific header
  //!
  //! \param name header name to check
  //! \return whether the requested header is present or not
  //!
  bool has_header(const std::string& name) const;

  //!
  //! remove a header from the request
  //! does nothing if header does not exist
  //!
  //! \param name name of the header to remove
  //!
  void remove_header(const std::string& name);

public:
  //!
  //! \return requested path
  //!
  const std::string& get_path(void) const;

  //!
  //! \return request params (route params and GET params)
  //!
  const routing::params_t& get_params(void) const;

  //!
  //! set requested path
  //!
  //! \param path new path for request
  //!
  void set_path(const std::string& path);

  //!
  //! set request params
  //!
  //! \param params new request params
  //!
  void set_params(const routing::params_t& params);

public:
  //!
  //! \return request body
  //!
  const std::string& get_body(void) const;

  //!
  //! set request body
  //!
  //! \param body new request body
  //!
  void set_body(const std::string& body);

public:
  //!
  //! \return printable version of the request (for logging purpose)
  //!
  std::string to_string(void) const;

private:
  //!
  //! request http verb
  //!
  method m_method;

  //!
  //! request http verb (raw string)
  //!
  std::string m_raw_method;

  //!
  //! status line target
  //!
  std::string m_target;

  //!
  //! requested http version
  //!
  std::string m_http_version;

  //!
  //! headers
  //!
  header_list_t m_headers;

  //!
  //! requested path
  //!
  std::string m_path;

  //!
  //! request params
  //!
  routing::params_t m_params;

  //!
  //! request body
  //!
  std::string m_body;
};

} // namespace http

} // namespace netflex
