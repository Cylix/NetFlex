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

#include <netflex/http/request.hpp>

namespace netflex {

namespace parsing {

//!
//! parser interface that must be implemented by all parsers
//!
class parser_iface {
public:
  //!
  //! default ctor
  //!
  //! \param request request to be initialized
  //!
  explicit parser_iface(http::request& request);

  //! default dtor
  virtual ~parser_iface(void) = default;

  //!
  //! consume input data to parse it and init the request
  //! if not enough data is passed in, this method would need to be called again later
  //! input data is modified whenever a token is consumed by parsing, even if parsing is incomplete or invalid
  //! invalid data would lead to a raised exception
  //!
  //! \param data input data to be parsed
  //! \return reference to the current object
  //!
  virtual parser_iface& operator<<(std::string& data) = 0;

  //!
  //! \return whether the parsing is done or not
  //!
  virtual bool is_done(void) const = 0;

protected:
  //!
  //! request
  //!
  http::request& m_request;
};

} // namespace parsing

} // namespace netflex
