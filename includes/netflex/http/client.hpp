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
#include <netflex/parsing/request_parser.hpp>

namespace netflex {

namespace http {

class client {
public:
  //! ctor & dtor
  client(const std::shared_ptr<tacopie::tcp_client>& tcp_client);
  ~client(void) = default;

  //! copy ctor & assignment operator
  client(const client&) = delete;
  client& operator=(const client&) = delete;

public:
  //! host & port
  const std::string& get_host(void) const;
  std::uint32_t get_port(void) const;

public:
  //! callbacks
  //!  > notify on new http request received
  //!  > notify on invalid http request received (err while parsing)
  //!  > notify on client disconnection
  typedef std::function<void(bool, const request&)> request_handler_t;
  typedef tacopie::tcp_client::disconnection_handler_t disconnection_handler_t;

  void set_request_handler(const request_handler_t&);
  void set_disconnection_handler(const disconnection_handler_t&);

private:
  //! call callbacks
  void call_request_received_callback(bool success, const request& request);

public:
  //! tcp_client callback
  void on_async_read_result(tacopie::tcp_client::read_result&);

private:
  //! async read from socket
  void async_read(void);

private:
  //! tcp_client
  std::shared_ptr<tacopie::tcp_client> m_tcp_client;
  //! callback
  request_handler_t m_request_received_callback;
  //! request parser
  parsing::request_parser m_parser;
};

} // namespace http

} // namespace netflex
