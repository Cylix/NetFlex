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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
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
#include <netflex/routing/params.hpp>

namespace netflex {

namespace misc {

//!
//! headers formatting for logging
//!
//! \param headers headers to format
//! \return formatted string ready for output
//!
std::string printable_header_list(const http::header_list_t& headers);

//!
//! params formatting for logging
//!
//! \param params params to format
//! \return formatted string ready for output
//!
std::string printable_params_list(const routing::params_t& params);

//!
//! headers formatting for http response
//!
//! \param headers headers to format
//! \return formatted string ready to be used for an http response
//!
std::string header_list_to_http_packet(const http::header_list_t& headers);

//!
//! status line formatting for http response
//!
//! \param http_version http version used by the http server
//! \param status_code status code returned by the http server
//! \reason_phrase reason phrase associated to the status code returned by the http server
//! \return formatted string ready to be used for an http response
//!
std::string status_line_to_http_packet(const std::string& http_version, unsigned int status_code, const std::string& reason_phrase);

} // namespace misc

} // namespace netflex
