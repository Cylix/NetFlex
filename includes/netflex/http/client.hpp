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

#include <memory>

#include <tacopie/tacopie>

#include <netflex/http/request.hpp>
#include <netflex/http/response.hpp>
#include <netflex/parsing/request_parser.hpp>

namespace netflex {

namespace http {

//!
//! http client
//! represent one client connected to the HTTP server and notify on new requests
//!
class client {
public:
  //!
  //! ctor
  //!
  //! \param tcp_client underlying tcp connection
  //!
  explicit client(const std::shared_ptr<tacopie::tcp_client>& tcp_client);

  //! default dtor
  ~client(void) = default;

  //! copy ctor
  client(const client&) = delete;
  //! assignment operator
  client& operator=(const client&) = delete;

public:
  //!
  //! \return host of the client
  //!
  const std::string& get_host(void) const;

  //!
  //! \return port of the client
  //!
  std::uint32_t get_port(void) const;

public:
  //!
  //! notify on new http request received
  //! notify on invalid http request received (err while parsing)
  //!
  typedef std::function<void(bool, request&)> request_handler_t;

  //!
  //! notify on client disconnection
  //!
  typedef tacopie::tcp_client::disconnection_handler_t disconnection_handler_t;

  //!
  //! define the callback to be called on new valid or invalid http requests
  //!
  //! \param cb callback to be called
  //!
  void set_request_handler(const request_handler_t& cb);

  //!
  //! define the callback to be called on disconnection
  //!
  //! \param cb callback to be called
  //!
  void set_disconnection_handler(const disconnection_handler_t& cb);

public:
  //!
  //! send http response to the client
  //! this should only be called as a result of receiving a valid http request
  //!
  void send_response(const response& response);

private:
  //!
  //! call the request_handler callback
  //!
  void call_request_received_callback(bool success, request& request);

public:
  //!
  //! tcp_client callback called on async_read operation completion
  //!
  //! \param res read operation result
  //!
  void on_async_read_result(tacopie::tcp_client::read_result& res);

private:
  //!
  //! async read from socket
  //!
  void async_read(void);

private:
  //!
  //! tcp connection
  //!
  std::shared_ptr<tacopie::tcp_client> m_tcp_client;

  //!
  //! callback to be called on new http requests
  //!
  request_handler_t m_request_received_callback;

  //!
  //! request parser used to parse the incoming http requests
  //!
  parsing::request_parser m_parser;
};

} // namespace http

} // namespace netflex
