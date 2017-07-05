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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <netflex/netflex>

#include <condition_variable>
#include <csignal>
#include <iostream>

#ifdef _WIN32
#include <Winsock2.h>
#endif /* _WIN32 */

//! netflex server
netflex::http::server server;
//! wait condvar
std::condition_variable stop_server_condvar;

//! sigint handler, exit server
void
handle_sigint(int) {
  // clean output
  std::cout << std::endl;

  server.stop();
  stop_server_condvar.notify_all();
}

int
main(void) {
  //! Enable logging
  netflex::active_logger = std::unique_ptr<netflex::logger>(new netflex::logger);

#ifdef _WIN32
  //! Windows netword DLL init
  WORD version = MAKEWORD(2, 2);
  WSADATA data;

  if (WSAStartup(version, &data) != 0) {
    __NETFLEX_LOG(error, "WSAStartup() failure");
    return -1;
  }
#endif /* _WIN32 */

  //! routes
  //! /:var_name provides you a way to define URLs including variables
  server.add_route({"/users/:user_name/articles/:post_id",
    [](const netflex::http::request& request, netflex::http::response& response) {
      __NETFLEX_LOG(info, "/users/:user_name/articles/:post_id callback triggered");
      __NETFLEX_LOG(info, "Headers: " + netflex::misc::printable_header_list(request.get_headers()));
      __NETFLEX_LOG(info, "Params: " + netflex::misc::printable_params_list(request.get_params()));

      response.set_body("What's up?!\n");
      response.add_header({"Content-Length", 12});
    }});

  //! optional middlewares
  server.add_middleware([](netflex::routing::middleware_chain& chain, netflex::http::request& request, netflex::http::response& response) {
    //! alter request
    request.add_header({"MiddleWare-Custom-Header", "MiddleWare custom header value"});

    //! proceed
    chain.proceed();

    //! alter response
    response.set_body(response.get_body() + "Powered by Netflex\n");
    response.add_header({"Content-Length", response.get_body().length()});
  });

  //! run server
  server.start("0.0.0.0", 3001);

  //! wait for sigint to exit server
  std::signal(SIGINT, &handle_sigint);
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);
  stop_server_condvar.wait(lock);

#ifdef _WIN32
  WSACleanup();
#endif /* _WIN32 */

  return 0;
}
