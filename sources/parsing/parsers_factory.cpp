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

#include <netflex/misc/error.hpp>
#include <netflex/parsing/header_fields_parser.hpp>
#include <netflex/parsing/message_body_parser.hpp>
#include <netflex/parsing/parsers_factory.hpp>
#include <netflex/parsing/start_line_parser.hpp>

namespace netflex {

namespace parsing {

//!
//! create the parser corresponding to the given stage
//!
std::unique_ptr<parser_iface>
create_parser(parsing_stage stage, http::request& request) {
  switch (stage) {
  case parsing_stage::start_line:
    return std::unique_ptr<parser_iface>(new start_line_parser(request));
  case parsing_stage::header_fields:
    return std::unique_ptr<parser_iface>(new header_fields_parser(request));
  case parsing_stage::message_body:
    return std::unique_ptr<parser_iface>(new message_body_parser(request));
  default:
    __NETFLEX_THROW(error, "create_parser received invalid parsing_stage");
  }
}


//!
//! switch to the next parsing stage *or go back to initial one if last stage already reached
//! and return the parser corresponding to that next stage
//!
std::unique_ptr<parser_iface>
switch_to_next_stage(parsing_stage& stage, http::request& request) {
  switch (stage) {
  case parsing_stage::start_line:
    stage = parsing_stage::header_fields;
    break;
  case parsing_stage::header_fields:
    stage = parsing_stage::message_body;
    break;
  case parsing_stage::message_body:
    stage = parsing_stage::start_line;
    break;
  default:
    __NETFLEX_THROW(error, "create_parser received invalid parsing_stage");
  }

  return create_parser(stage, request);
}

} // namespace parsing

} // namespace netflex
