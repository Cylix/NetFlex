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

#include <netflex/http/server.hpp>

namespace netflex {

namespace http {

//!
//! add routes to the server
//!
server&
server::add_route(const routing::route& route) {
  m_routes.push_back(route);
  return *this;
}

server&
server::add_routes(const std::vector<routing::route>& routes) {
  m_routes.insert(m_routes.end(), routes.begin(), routes.end());
  return *this;
}

server&
server::set_route(const std::vector<routing::route>& routes) {
  m_routes = routes;
  return *this;
}

//!
//! start & stop the server
//!

void
server::start(const std::string& host, unsigned int port) {
  m_tcp_server.start(host, port);
}

void
server::stop(void) {
  m_tcp_server.stop();
}

//!
//! returns whether the server is currently running or not
//!

bool
server::is_running(void) const {
  return m_tcp_server.is_running();
}

} // namespace http

} // namespace netflex
