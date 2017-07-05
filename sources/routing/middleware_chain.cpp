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

#include <netflex/http/response.hpp>
#include <netflex/routing/middleware_chain.hpp>

namespace netflex {

namespace routing {

//!
//! ctor & dtor
//!
middleware_chain::middleware_chain(const std::list<middleware_t>& middlewares, http::request& request, http::response& response)
: m_middlewares(middlewares)
, m_request(request)
, m_response(response)
, m_current_middleware(m_middlewares.begin()) {}


//!
//! proceed to next middleware
//!
void
middleware_chain::proceed(void) {
  //! nothing anymore to proceed
  if (m_current_middleware == m_middlewares.end())
    return;

  //! we increment the index right now because the next middleware will call .proceed before returning
  //! so we need to do it beforehand if we do not want to execute the same middleware over and over again
  ++m_current_middleware;

  //! execute middleware
  (*std::prev(m_current_middleware))(*this, m_request, m_response);
}

} // namespace routing

} // namespace netflex
