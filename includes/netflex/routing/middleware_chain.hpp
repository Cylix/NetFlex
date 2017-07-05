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

#include <functional>
#include <list>

#include <netflex/http/request.hpp>
#include <netflex/http/response.hpp>

namespace netflex {

namespace routing {

//! middleware_chain forward declaration
class middleware_chain;

//! middleware
//! take middleware middleware_chain as parameter and should call .proceed() to go to next step
typedef std::function<void(middleware_chain&, http::request&, http::response&)> middleware_t;

class middleware_chain {
public:
  //! ctor & dtor
  middleware_chain(const std::list<middleware_t>& middlewares, http::request& request, http::response& response);
  ~middleware_chain(void) = default;

  //! copy ctor & assignment operator
  middleware_chain(const middleware_chain&) = default;
  middleware_chain& operator=(const middleware_chain&) = default;

public:
  //! proceed to next middleware
  void proceed(void);

private:
  //! middlewares
  std::list<middleware_t> m_middlewares;
  //! request & response
  http::request& m_request;
  http::response& m_response;
  //! current middleware to execute
  std::list<middleware_t>::iterator m_current_middleware;
};

} // namespace routing

} // namespace netflex
