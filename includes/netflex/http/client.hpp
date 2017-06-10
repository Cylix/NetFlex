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
  //! on request received callback
  typedef std::function<void(request&)> request_received_callback_t;

  //! listen for incoming requests
  void listen_for_incoming_requests(const request_received_callback_t& callback);

public:
  //! tcp_client callback
  void on_async_read_result(tacopie::tcp_client::read_result&);

private:
  //! tcp_client
  std::shared_ptr<tacopie::tcp_client> m_tcp_client;
  //! on request received callback
  request_received_callback_t m_request_received_callback;
};

} // namespace http

} // namespace netflex
