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
#include <netflex/misc/logger.hpp>
namespace netflex {

namespace http {

//!
//! ctor & dtor
//!
client::client(const std::shared_ptr<tacopie::tcp_client>& tcp_client)
: m_tcp_client(tcp_client)
, m_request_received_callback(nullptr) {}


//!
//! host & port
//!
const std::string&
client::get_host(void) const {
  return m_tcp_client->get_host();
}

std::uint32_t
client::get_port(void) const {
  return m_tcp_client->get_port();
}


//!
//! callbacks
//!  > notify on new http request received
//!  > notify on invalid http request received (err while parsing)
//!  > notify on client disconnection
//!
void
client::set_request_handler(const request_handler_t& recv_callback) {
  m_request_received_callback = recv_callback;

  m_tcp_client->async_read({1024, std::bind(&client::on_async_read_result, this, std::placeholders::_1)});
}

void
client::set_disconnection_handler(const disconnection_handler_t& disco_callback) {
  m_tcp_client->set_on_disconnection_handler(disco_callback);
}


//!
//! tcp_client callback
//!
void
client::on_async_read_result(tacopie::tcp_client::read_result& result) {
  __NETFLEX_LOG(debug, __NETFLEX_CLIENT_LOG_PREFIX(m_tcp_client->get_host(), m_tcp_client->get_port()) + "async_read result");

  if (!result.success) {
    __NETFLEX_LOG(debug, __NETFLEX_CLIENT_LOG_PREFIX(m_tcp_client->get_host(), m_tcp_client->get_port()) + "async_read failure");
    return;
  }

  //!do something
}

} // namespace http

} // namespace netflex
