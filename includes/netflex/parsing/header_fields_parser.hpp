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

#include <unordered_map>

#include <netflex/parsing/header_field_parser.hpp>
#include <netflex/parsing/parser_iface.hpp>

namespace netflex {

namespace parsing {

//!
//! parser for all header fields
//!
class header_fields_parser : public parser_iface {
public:
  //!
  //! default ctor
  //!
  //! \param request request to be initialized
  //!
  explicit header_fields_parser(http::request& request);

  //! default dtor
  ~header_fields_parser(void) = default;

  //! copy ctor
  header_fields_parser(const header_fields_parser&) = delete;
  //! assignment operator
  header_fields_parser& operator=(const header_fields_parser&) = delete;

private:
  //!
  //! parsing state
  //! specify which part of the headers fields is being parsed
  //!
  enum class state {
    empty_line,
    header_field,
    done
  };

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
  //! try to fetch the empty line (end of header list)
  //!
  //! \param buffer input data
  //! \return true if empty line is parsed, false otherwise
  //!
  bool fetch_empty_line(std::string& buffer);

  //!
  //! parse header
  //!
  //! \param buffer input data
  //! \return true if header is parsed, false otherwise
  //!
  bool fetch_header(std::string& buffer);

private:
  //!
  //! current state
  //!
  state m_state;

  //!
  //! parser
  //!
  header_field_parser m_parser;
};

} // namespace parsing

} // namespace netflex
