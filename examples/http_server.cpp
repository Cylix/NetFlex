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
#ifdef _WIN32
  //! Windows netword DLL init
  WORD version = MAKEWORD(2, 2);
  WSADATA data;

  if (WSAStartup(version, &data) != 0) {
    std::cerr << "WSAStartup() failure" << std::endl;
    return -1;
  }
#endif /* _WIN32 */

  //! Enable logging
  netflex::active_logger = std::unique_ptr<netflex::logger>(new netflex::logger(netflex::logger::log_level::debug));

  //! routes
  server.add_route({"/abc/:var1/def/:var2/:var3",
    [](const netflex::http::request& request, netflex::http::response& response) {
      std::cout << "request received for /abc/:var1/def/:var2/:var3 with params:" << std::endl;

      for (const auto& param : request.get_params()) {
        std::cout << param.first << "=" << param.second << std::endl;
      }

      response.set_body("it works!\n");
      response.add_header({"Content-Length", "10"});
    }});

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
