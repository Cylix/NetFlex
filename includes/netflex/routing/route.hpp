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
#include <string>

#include <netflex/http/request.hpp>
#include <netflex/http/response.hpp>
#include <netflex/routing/route_matcher.hpp>

namespace netflex {

namespace routing {

class route {
public:
  //! callback associated to the route
  typedef std::function<void(const http::request&, http::response&)> route_callback_t;

public:
  //! ctor & dtor
  route(const std::string& path, const route_callback_t& callback);
  ~route(void) = default;

  //! copy ctor & assignment operator
  route(const route&) = default;
  route& operator=(const route&) = default;

public:
  //! matching
  bool match(http::request& request) const;

public:
  //! dispatch
  void dispatch(const http::request&, http::response&) const;

private:
  //! path
  std::string m_path;
  //! callback
  route_callback_t m_callback;
  //! used to match a route with a requested path
  route_matcher m_matcher;
};

} // namespace routing

} // namespace netflex
