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
#include <netflex/misc/error.hpp>
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
  async_read();
}

void
client::set_disconnection_handler(const disconnection_handler_t& disco_callback) {
  m_tcp_client->set_on_disconnection_handler(disco_callback);
}


//!
//! send http response
//!
void
client::send_response(const response& response) {
  std::string http_response = response.to_http_packet();

  m_tcp_client->async_write({{http_response.begin(), http_response.end()}, nullptr});
}


//!
//! call callbacks
//!
void
client::call_request_received_callback(bool success, const request& request) {
  if (m_request_received_callback) {
    m_request_received_callback(success, request);
  }
}


//!
//! tcp_client callback
//!
void
client::on_async_read_result(tacopie::tcp_client::read_result& result) {
  __NETFLEX_LOG(debug, __NETFLEX_CLIENT_LOG_PREFIX(m_tcp_client->get_host(), m_tcp_client->get_port()) + "async_read result");

  //! if request has failed, simply return
  //! disconnection callback will be called by the tcp_client right after
  if (!result.success) {
    __NETFLEX_LOG(warn, __NETFLEX_CLIENT_LOG_PREFIX(m_tcp_client->get_host(), m_tcp_client->get_port()) + "async_read failure");
    return;
  }

  //! try to parse request
  //! in case of failure, notify that the request could not be parsed and stop reading bytes from socket
  try {
    __NETFLEX_LOG(debug, __NETFLEX_CLIENT_LOG_PREFIX(m_tcp_client->get_host(), m_tcp_client->get_port()) + "attempts to parse request");
    m_parser << std::string(result.buffer.begin(), result.buffer.end());
  }
  catch (const netflex_error&) {
    __NETFLEX_LOG(error, __NETFLEX_CLIENT_LOG_PREFIX(m_tcp_client->get_host(), m_tcp_client->get_port()) + "could not parse request (invalid format), disconnecting");

    call_request_received_callback(false, m_parser.get_currently_parsed_request());

    return;
  }

  //! retrieve available requests and forward them
  while (m_parser.request_available()) {
    __NETFLEX_LOG(debug, __NETFLEX_CLIENT_LOG_PREFIX(m_tcp_client->get_host(), m_tcp_client->get_port()) + "request fully parsed");

    call_request_received_callback(true, m_parser.get_front());
    m_parser.pop_front();
  }

  //! Keep reading
  async_read();
} // namespace http


//!
//! async read from socket
//!
void
client::async_read(void) {
  try {
    m_tcp_client->async_read({1024, std::bind(&client::on_async_read_result, this, std::placeholders::_1)});
  }
  catch (const std::exception&) {
    //! Client disconnected in the meantime
  }
}

} // namespace http

} // namespace netflex
