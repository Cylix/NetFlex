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

#include <string>
#include <vector>

#include <tacopie/tacopie>

#include <netflex/routing/route.hpp>

namespace netflex {

namespace http {

class server {
public:
  //! ctor & dtor
  server(void)  = default;
  ~server(void) = default;

  //! copy ctor & assignment operator
  server(const server&) = delete;
  server& operator=(const server&) = delete;

public:
  //! add routes to the server
  server& add_route(const routing::route& route);
  server& add_routes(const std::vector<routing::route>& routes);
  server& set_route(const std::vector<routing::route>& routes);

public:
  //! start & stop the server
  void start(const std::string& host = "0.0.0.0", unsigned int port = 3000);
  void stop(void);

  //! returns whether the server is currently running or not
  bool is_running(void) const;

private:
  //! underlying tcp server
  tacopie::tcp_server m_tcp_server;
  //! server routes
  std::vector<routing::route> m_routes;
};

} // namespace http

} // namespace netflex
