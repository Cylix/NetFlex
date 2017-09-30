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

#include <list>
#include <string>
#include <vector>

#include <tacopie/tacopie>

#include <netflex/http/client.hpp>
#include <netflex/routing/middleware_chain.hpp>
#include <netflex/routing/route.hpp>

namespace netflex {

namespace http {

//!
//! http server itself
//! listen for incoming connection, detect requests, parse them and dispatch them before sending the appropriate responses
//!
class server {
public:
  //! default ctor
  server(void);
  //! default dtor
  ~server(void) = default;

  //! copy ctor
  server(const server&) = delete;
  //! assignment operator
  server& operator=(const server&) = delete;

public:
  //!
  //! add route to the server
  //!
  //! \param route route to be added
  //! \return reference to the current object
  //!
  server& add_route(const routing::route& route);

  //!
  //! add multiple routes to the server
  //!
  //! \param routes routes to be added
  //! \return reference to the current object
  //!
  server& add_routes(const std::vector<routing::route>& routes);

  //!
  //! set routes for the server
  //!
  //! \param routes routes to be set
  //! \return reference to the current object
  //!
  server& set_route(const std::vector<routing::route>& routes);

public:
  //!
  //! add middleware to the server
  //! added middleware is added at the highest level (on top of all the previously added middleware)
  //! this mean that the added middleware will be the last to be executed
  //!
  //! \param middleware middleware to be added
  //! \return reference to the current object
  //!
  server& add_middleware(const routing::middleware_t& middleware);

  //!
  //! add multiple middlewares to the server
  //! added middlewares are added at the highest level (on top of all the previously added middleware)
  //! this mean that the added middlewares will be the last to be executed
  //! middleares should be ranged from the lowest to the highest level
  //!
  //! \param middlewares middlewares to be added
  //! \return reference to the current object
  //!
  server& add_middlewares(const std::list<routing::middleware_t>& middlewares);

  //!
  //! set middlewares for the server
  //! middleares should be ranged from the lowest to the highest level
  //!
  //! \param middlewares middlewares to be added
  //! \return reference to the current object
  //!
  server& set_middlewares(const std::list<routing::middleware_t>& middlewares);

public:
  //!
  //! start the server at the given host and port
  //!
  //! \param host host to bind
  //! \param port port to bind
  //!
  void start(const std::string& host = "0.0.0.0", unsigned int port = 3000);

  //!
  //! stop the server
  //!
  void stop(void);

  //!
  //! \return whether the server is currently running or not
  //!
  bool is_running(void) const;

private:
  //!
  //! tacopie::tcp_server callback
  //! called whenever the tcp server receives a new connection
  //!
  //! \return whether the server takes charge of the management of the connection or not (always true in our case)
  //!
  bool on_connection_received(const std::shared_ptr<tacopie::tcp_client>& client);

  //!
  //! convenience typedef
  //!
  typedef std::list<client>::iterator client_iterator_t;

  //!
  //! client callback
  //! called whenever a client receives a new http request (valid or invalid)
  //!
  //! \param success whether the received request is valid or not
  //! \param request the received request
  //! \param client iterator to the client in m_clients that trigerred the callback
  //!
  void on_http_request_received(bool success, request& request, client_iterator_t client);

  //!
  //! client callback
  //! called whenever a client disconnected from the server
  //!
  //! \param client iterator to the client in m_clients that trigerred the callback
  //!
  void on_client_disconnected(client_iterator_t client);

  //!
  //! dispatch the request and its response by using the specified middleware chain
  //!
  //! \param chain chain of middlewares to be executed
  //! \param request received http request
  //! \param response response to be sent
  //!
  void dispatch(routing::middleware_chain& chain, http::request& request, http::response& response);

private:
  //!
  //! underlying tcp server
  //!
  tacopie::tcp_server m_tcp_server;

  //!
  //! server routes
  //!
  std::vector<routing::route> m_routes;

  //!
  //! server middlewares
  //!
  std::list<routing::middleware_t> m_middlewares;

  //!
  //! clients
  //!
  std::list<client> m_clients;
};

} // namespace http

} // namespace netflex
