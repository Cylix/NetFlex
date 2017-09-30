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

//!
//! http response representation
//! contains all information related to the http response to be sent
//!
class response {
public:
  //! default ctor
  response(void);
  //! default dtor
  ~response(void) = default;

  //! copy ctor
  response(const response&) = default;
  //! assignment operator
  response& operator=(const response&) = default;

public:
  //!
  //! \return http version
  //!
  const std::string& get_http_version(void) const;

  //!
  //! \return status code
  //!
  unsigned int get_status_code(void) const;

  //!
  //! \return reason phrase for status code
  //!
  const std::string& get_reason_phase(void) const;

  //!
  //! set the http version used for the response
  //!
  //! \param version http version
  //!
  void set_http_version(const std::string& version);

  //!
  //! set response status code
  //!
  //! \param code status code to return to the client
  //!
  void set_status_code(unsigned int code);

  //!
  //! set reason phrase
  //!
  //! \param reason reason phrase to return to the client
  //!
  void set_reason_phrase(const std::string& reason);

public:
  //!
  //! \return headers list of the http response
  //!
  const header_list_t& get_headers(void) const;

  //!
  //! add a header to the headers list to be returned to the client
  //! if the header already exists, override it
  //!
  //! \param header header to be added
  //!
  void add_header(const header& header);

  //!
  //! set a headers list to be used for the http response
  //!
  //! \param headers headers list for http response
  //!
  void set_headers(const header_list_t& headers);

public:
  //!
  //! \return response body
  //!
  const std::string& get_body(void) const;

  //!
  //! set response body to be returned to the client
  //!
  //! \param body new body to be returned
  //!
  void set_body(const std::string& body);

public:
  //!
  //! convert response to http packet
  //!
  //! \return conversion
  //!
  std::string to_http_packet(void) const;

private:
  //!
  //! response http version
  //!
  std::string m_http_version;

  //!
  //! response status code
  //!
  unsigned int m_status;

  //!
  //! response reason phrase
  //!
  std::string m_reason;

  //!
  //! response headers
  //!
  header_list_t m_headers;

  //!
  //! response body
  //!
  std::string m_body;
};

} // namespace http

} // namespace netflex
