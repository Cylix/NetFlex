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
#include <netflex/misc/logger.hpp>

namespace netflex {

namespace http {

//!
//! ctor & dtor
//!
server::server(void)
//! insert first middleware (dispatch)
: m_middlewares({1, std::bind(&server::dispatch, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)}) {}


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
//! add middlewares
//!
server&
server::add_middleware(const routing::middleware_t& middleware) {
  //! insert all middlewares after the user defined middlewares, but always before the dispatch middleware
  m_middlewares.insert(std::prev(m_middlewares.end()), middleware);

  return *this;
}

server&
server::add_middlewares(const std::list<routing::middleware_t>& middlewares) {
  //! insert all middlewares after the user defined middlewares, but always before the dispatch middleware
  m_middlewares.insert(std::prev(m_middlewares.end()), middlewares.begin(), middlewares.end());

  return *this;
}

server&
server::set_middlewares(const std::list<routing::middleware_t>& middlewares) {
  //! erase all middlewares except the dispatch one
  m_middlewares.erase(m_middlewares.begin(), std::prev(m_middlewares.end()));

  //! insert all middlewares after the user defined middlewares, but always before the dispatch middleware
  m_middlewares.insert(std::prev(m_middlewares.end()), middlewares.begin(), middlewares.end());

  return *this;
}


//!
//! start & stop the server
//!
void
server::start(const std::string& host, unsigned int port) {
  __NETFLEX_LOG(info, "starting server on " + __NETFLEX_HOST_PORT_LOG(host, port));
  //! TODO: debug log of loaded routes.

  m_tcp_server.start(host, port, std::bind(&server::on_connection_received, this, std::placeholders::_1));

  __NETFLEX_LOG(info, "server running on " + __NETFLEX_HOST_PORT_LOG(host, port));
}

void
server::stop(void) {
  __NETFLEX_LOG(info, "stopping server");
  m_tcp_server.stop();
  __NETFLEX_LOG(info, "server stopped");
}


//!
//! tacopie::tcp_server callback
//!
bool
server::on_connection_received(const std::shared_ptr<tacopie::tcp_client>& client) {
  __NETFLEX_LOG(debug, __NETFLEX_CLIENT_LOG_PREFIX(client->get_host(), client->get_port()) + "receiving connection");

  //! store client
  m_clients.emplace_back(client);

  //! start listening for incoming requests
  client_iterator_t http_client = std::prev(m_clients.end());
  http_client->set_disconnection_handler(std::bind(&server::on_client_disconnected, this, http_client));
  http_client->set_request_handler(std::bind(&server::on_http_request_received, this, std::placeholders::_1, std::placeholders::_2, http_client));

  __NETFLEX_LOG(info, __NETFLEX_CLIENT_LOG_PREFIX(client->get_host(), client->get_port()) + "connection accepted");

  //! mark connection as handled by ourselves
  return true;
}


//!
//! client callback
//!
void
server::on_http_request_received(bool success, request& request, client_iterator_t client) {
  if (!success) {
    __NETFLEX_LOG(warn, __NETFLEX_CLIENT_LOG_PREFIX(client->get_host(), client->get_port()) + "invalid request");

    m_clients.erase(client);
    return;
  }

  __NETFLEX_LOG(info, __NETFLEX_CLIENT_LOG_PREFIX(client->get_host(), client->get_port()) + "receive request " + request.to_string());

  http::response response;
  //! status line
  response.set_http_version("HTTP/1.1");
  response.set_status_code(200);
  response.set_reason_phrase("OK");
  //! header with body information
  response.add_header({"Content-Type", "text/html"});

  //! middleware chain, including dispatch
  routing::middleware_chain chain(m_middlewares, request, response);
  chain.proceed();

  client->send_response(response);
}

void
server::on_client_disconnected(client_iterator_t client) {
  __NETFLEX_LOG(info, __NETFLEX_CLIENT_LOG_PREFIX(client->get_host(), client->get_port()) + "client disconnected");

  m_clients.erase(client);
}


//!
//! dispatch
//!
void
server::dispatch(routing::middleware_chain&, http::request& request, http::response& response) {
  //! find route matching
  for (const auto& route : m_routes) {
    if (route.match(request)) {
      route.dispatch(request, response);
      return;
    }
  }

  //! 404 not found status
  response.set_status_code(404);
  response.set_reason_phrase("Not Found");
  //! 4040 not found body
  response.set_body("Page not found\n");
  //! 404 not found headers
  response.add_header({"Content-Length", response.get_body().length()});
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
