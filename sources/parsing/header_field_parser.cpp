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

#include <netflex/parsing/header_field_parser.hpp>

namespace netflex {

namespace parsing {

//!
//! parser_iface impl
//!
parser_iface&
header_field_parser::operator<<(std::string&) {
  return *this;
}

bool
header_field_parser::is_done(void) const {
  return false;
}

void
header_field_parser::apply(http::request& request) const {
  request.add_header(m_header);
}


//!
//! retrieve informations
//!
const http::header&
header_field_parser::get_header(void) const {
  return m_header;
}


//!
//! reset state
//!
void
header_field_parser::reset(void) {
  //! reset parsed data
  m_header = http::header();
}


} // namespace parsing

} // namespace netflex
