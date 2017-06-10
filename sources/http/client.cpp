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

#include <netflex/http/client.hpp>

namespace netflex {

namespace http {

//!
//! ctor & dtor
//!
client::client(const std::shared_ptr<tacopie::tcp_client>& tcp_client)
: m_tcp_client(tcp_client) {}


//!
//! listen for incoming requests
//!
void
client::listen_for_incoming_requests(const request_received_callback_t& callback) {
  m_request_received_callback = callback;

  m_tcp_client->async_read({1024, std::bind(&client::on_async_read_result, this, std::placeholders::_1)});
}


//!
//! tcp_client callback
//!
void
client::on_async_read_result(tacopie::tcp_client::read_result&) {
  //!do something
}

} // namespace http

} // namespace netflex
