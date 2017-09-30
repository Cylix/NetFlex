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
#include <unordered_map>
#include <utility>

namespace netflex {

namespace http {

//!
//! representation of an HTTP header
//!
struct header {
  //! default ctor
  header(void) = default;

  //!
  //! std::string-based ctor
  //!
  //! \param field_name header name
  //! \param field_value header value
  //!
  header(const std::string& field_name, const std::string& field_value);

  //!
  //! char*-based ctor
  //!
  //! \param field_name header name
  //! \param field_value header value
  //!
  header(const std::string& field_name, const char* field_value);

  //!
  //! to_string-based ctor
  //!
  //! \param field_name header name
  //! \param field_value header value
  //!
  template <typename T>
  header(const std::string& field_name, T field_value)
  : field_name(field_name)
  , field_value(std::to_string(field_value)) {}

  //!
  //! header name
  //!
  std::string field_name;

  //!
  //! header value
  //!
  std::string field_value;

  //!
  //! \return printable string
  //!
  std::string to_s(void) const;
};

//!
//! convenience typedef for list of headers
//! stored as hashtable
//!
typedef std::unordered_map<std::string, std::string> header_list_t;

} // namespace http

} // namespace netflex
