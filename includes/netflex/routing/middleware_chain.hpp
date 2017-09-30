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

//!
//! middleware
//! take middleware middleware_chain as parameter and should call .proceed() to go to next step
//!
typedef std::function<void(middleware_chain&, http::request&, http::response&)> middleware_t;

//!
//! contains a chain of middlewares to execute
//! used to manage execution of middlewares in the right order (and possibly stop the execution if necessary)
//!
class middleware_chain {
public:
  //!
  //! ctor
  //!
  //! \param middlewares middlewares to be managed by the middleware chain. middleware should be ordered from lowest level (first executed) to highest level (last to be executed)
  //! \param request request to be passed as parameter to each middleware
  //! \param response response to be passed as parameter to each middleware
  //!
  middleware_chain(const std::list<middleware_t>& middlewares, http::request& request, http::response& response);

  //! default dtor
  ~middleware_chain(void) = default;

  //! copy ctor
  middleware_chain(const middleware_chain&) = default;
  //! assignment operator
  middleware_chain& operator=(const middleware_chain&) = default;

public:
  //!
  //! proceed to next middleware to be executed or return if nothing needs to be executed anymore
  //!
  void proceed(void);

private:
  //!
  //! middlewares
  //!
  std::list<middleware_t> m_middlewares;

  //!
  //! request to propagate
  //!
  http::request& m_request;

  //!
  //! response to propagate
  //!
  http::response& m_response;

  //!
  //! current middleware to execute
  //!
  std::list<middleware_t>::iterator m_current_middleware;
};

} // namespace routing

} // namespace netflex
