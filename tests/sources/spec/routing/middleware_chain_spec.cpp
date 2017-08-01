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

#include <gtest/gtest.h>

#include <netflex/netflex>

TEST(middleware_chain, proceed) {
  std::list<netflex::routing::middleware_t> middlewares;

  //! 1st middleware (lower level)
  middlewares.push_back([](netflex::routing::middleware_chain& chain, netflex::http::request& request, netflex::http::response& response) {
    //! check previous values have been forwarded
    EXPECT_EQ(request.get_body(), "0");
    EXPECT_EQ(response.get_body(), "0");

    //! change value
    request.set_body("1");
    response.set_body("1");

    //! proceed, no more to proceed technically
    chain.proceed();

    //! check next values have been forwarded
    EXPECT_EQ(request.get_body(), "3");
    EXPECT_EQ(response.get_body(), "3");
  });

  //! 2st middleware (mid level)
  middlewares.push_back([](netflex::routing::middleware_chain& chain, netflex::http::request& request, netflex::http::response& response) {
    //! check previous values have been forwarded
    EXPECT_EQ(request.get_body(), "1");
    EXPECT_EQ(response.get_body(), "1");

    //! change value
    request.set_body("2");
    response.set_body("2");

    //! proceed
    chain.proceed();

    //! check next values have been forwarded
    EXPECT_EQ(request.get_body(), "3");
    EXPECT_EQ(response.get_body(), "3");
  });

  //! 3rd middleware (higher level)
  middlewares.push_back([](netflex::routing::middleware_chain& chain, netflex::http::request& request, netflex::http::response& response) {
    //! check previous values have been forwarded
    EXPECT_EQ(request.get_body(), "2");
    EXPECT_EQ(response.get_body(), "2");

    //! change value
    request.set_body("3");
    response.set_body("3");

    //! proceed
    chain.proceed();

    //! check next values have been forwarded
    EXPECT_EQ(request.get_body(), "3");
    EXPECT_EQ(response.get_body(), "3");
  });

  //! initial setup
  netflex::http::request request;
  netflex::http::response response;

  request.set_body("0");
  response.set_body("0");

  //! build chain
  netflex::routing::middleware_chain chain(middlewares, request, response);

  //! proceed
  chain.proceed();

  //! check chain forwared values
  EXPECT_EQ(request.get_body(), "3");
  EXPECT_EQ(response.get_body(), "3");
}


TEST(middleware_chain, proceed_with_broken_chain) {
  std::list<netflex::routing::middleware_t> middlewares;

  //! 1st middleware (lower level)
  middlewares.push_back([](netflex::routing::middleware_chain& chain, netflex::http::request& request, netflex::http::response& response) {
    //! check previous values have been forwarded
    EXPECT_EQ(request.get_body(), "0");
    EXPECT_EQ(response.get_body(), "0");

    //! change value
    request.set_body("1");
    response.set_body("1");

    //! proceed, no more to proceed technically
    chain.proceed();

    //! check next values have been forwarded
    EXPECT_EQ(request.get_body(), "2");
    EXPECT_EQ(response.get_body(), "2");
  });

  //! 2st middleware (mid level)
  middlewares.push_back([](netflex::routing::middleware_chain&, netflex::http::request& request, netflex::http::response& response) {
    //! check previous values have been forwarded
    EXPECT_EQ(request.get_body(), "1");
    EXPECT_EQ(response.get_body(), "1");

    //! change value
    request.set_body("2");
    response.set_body("2");

    //! notice: no proceed
  });

  //! 3rd middleware (higher level)
  middlewares.push_back([](netflex::routing::middleware_chain&, netflex::http::request&, netflex::http::response&) {
    //! if this middleware is called, test fails: we havent proceed in previous middleware
    FAIL() << "Middleware #3 get called while middleware #2 did not proceed";
  });

  //! initial setup
  netflex::http::request request;
  netflex::http::response response;

  request.set_body("0");
  response.set_body("0");

  //! build chain
  netflex::routing::middleware_chain chain(middlewares, request, response);

  //! proceed
  chain.proceed();

  //! check chain forwared values
  EXPECT_EQ(request.get_body(), "2");
  EXPECT_EQ(response.get_body(), "2");
}


TEST(middleware_chain, proceed_with_empty_chain) {
  std::list<netflex::routing::middleware_t> middlewares;

  //! initial setup
  netflex::http::request request;
  netflex::http::response response;

  request.set_body("0");
  response.set_body("0");

  //! build chain
  netflex::routing::middleware_chain chain(middlewares, request, response);

  //! proceed
  chain.proceed();

  //! check chain forwared values
  EXPECT_EQ(request.get_body(), "0");
  EXPECT_EQ(response.get_body(), "0");
}
