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

#include <netflex/http/header.hpp>
#include <netflex/parsing/parser_iface.hpp>

namespace netflex {

namespace parsing {

class header_field_parser : public parser_iface {
public:
  //! ctor & dtor
  header_field_parser(void);
  ~header_field_parser(void) = default;

  //! copy ctor & assignment operator
  header_field_parser(const header_field_parser&) = delete;
  header_field_parser& operator=(const header_field_parser&) = delete;

public:
  //! retrieve informations
  const http::header& get_header(void) const;

  //! reset state
  void reset(void);

public:
  //! parser_iface impl
  parser_iface&
  operator<<(std::string&);
  bool is_done(void) const;
  void apply(http::request&) const;

private:
  //! parsing state
  enum class state {
    field_name,
    field_value,
    trailing,
    done
  };

private:
  //! parse header
  bool fetch_field_name(std::string& buffer);
  bool fetch_field_value(std::string& buffer);
  bool fetch_trailing(std::string& buffer);

private:
  //! parser header
  http::header m_header;
  //! keep track of last whitespace character erased in fetch trailing
  char m_last_consumed_whitespace;
  //! current state
  state m_state;
};

} // namespace parsing

} // namespace netflex
