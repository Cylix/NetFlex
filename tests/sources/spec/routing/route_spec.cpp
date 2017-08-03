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

//!
//! match
//!
TEST(route, match) {
  netflex::http::request request;
  request.set_target("/");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/");
  EXPECT_EQ(params.size(), 0UL);
}

TEST(route, match_unmatch_http_method) {
  netflex::http::request request;
  request.set_target("/");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::POST, "/", nullptr);
  EXPECT_EQ(route.match(request), false);
}

TEST(route, match_no_variable) {
  netflex::http::request request;
  request.set_target("/users/1/articles");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/1/articles", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/users/1/articles");
  EXPECT_EQ(params.size(), 0UL);
}

TEST(route, match_solo_variable) {
  netflex::http::request request;
  request.set_target("/users/42/articles");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/:user_id/articles", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/users/:user_id/articles");
  EXPECT_EQ(params.size(), 1UL);
  EXPECT_EQ(params["user_id"], "42");
}

TEST(route, match_multi_variable) {
  netflex::http::request request;
  request.set_target("/users/42/articles/84/comments/21/author");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/:user_id/articles/:article_id/comments/:comment_id/author", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/users/:user_id/articles/:article_id/comments/:comment_id/author");
  EXPECT_EQ(params.size(), 3UL);
  EXPECT_EQ(params["user_id"], "42");
  EXPECT_EQ(params["article_id"], "84");
  EXPECT_EQ(params["comment_id"], "21");
}

TEST(route, match_alphanum_variable) {
  netflex::http::request request;
  request.set_target("/users/42/articles");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/:user_id-0/articles", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/users/:user_id-0/articles");
  EXPECT_EQ(params.size(), 1UL);
  EXPECT_EQ(params["user_id-0"], "42");
}

TEST(route, match_solo_url_params) {
  netflex::http::request request;
  request.set_target("/users/articles?user_id=42");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/articles", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/users/articles");
  EXPECT_EQ(params.size(), 1UL);
  EXPECT_EQ(params["user_id"], "42");
}

TEST(route, match_multi_url_params) {
  netflex::http::request request;
  request.set_target("/users/articles?user_id=42&article_id=84&comment_id=21");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/articles", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/users/articles");
  EXPECT_EQ(params.size(), 3UL);
  EXPECT_EQ(params["user_id"], "42");
  EXPECT_EQ(params["article_id"], "84");
  EXPECT_EQ(params["comment_id"], "21");
}

TEST(route, match_multi_empty_url_params) {
  netflex::http::request request;
  request.set_target("/users/articles?user_id=42&article_id=&comment_id=21");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/articles", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/users/articles");
  EXPECT_EQ(params.size(), 3UL);
  EXPECT_EQ(params["user_id"], "42");
  EXPECT_EQ(params["article_id"], "");
  EXPECT_EQ(params["comment_id"], "21");
}

TEST(route, match_combination) {
  netflex::http::request request;
  request.set_target("/users/42/articles/84/?comment_id=21&source=google");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/:user_id/articles/:article_id", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/users/:user_id/articles/:article_id");
  EXPECT_EQ(params.size(), 4UL);
  EXPECT_EQ(params["user_id"], "42");
  EXPECT_EQ(params["article_id"], "84");
  EXPECT_EQ(params["comment_id"], "21");
  EXPECT_EQ(params["source"], "google");
}

TEST(route, match_combination_conflict) {
  netflex::http::request request;
  request.set_target("/users/42/articles/84/?comment_id=21&user_id=1&source=google");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/:user_id/articles/:article_id", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/users/:user_id/articles/:article_id");
  EXPECT_EQ(params.size(), 4UL);
  EXPECT_EQ(params["user_id"], "1");
  EXPECT_EQ(params["article_id"], "84");
  EXPECT_EQ(params["comment_id"], "21");
  EXPECT_EQ(params["source"], "google");
}

TEST(route, match_complex) {
  netflex::http::request request;
  request.set_target("/users/42/articles/84/comments/21/author?user_id=1&source=google&test=#anchor");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/:user_id/articles/:article_id/comments/:comment_id/author", nullptr);
  EXPECT_EQ(route.match(request), true);

  netflex::routing::params_t params = request.get_params();
  EXPECT_EQ(request.get_path(), "/users/:user_id/articles/:article_id/comments/:comment_id/author");
  EXPECT_EQ(params.size(), 5UL);
  EXPECT_EQ(params["user_id"], "1");
  EXPECT_EQ(params["article_id"], "84");
  EXPECT_EQ(params["comment_id"], "21");
  EXPECT_EQ(params["source"], "google");
  EXPECT_EQ(params["test"], "");
}

TEST(route, match_trailing_slash) {
  netflex::http::request request;
  request.set_target("/users/1/articles/");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/1/articles", nullptr);
  EXPECT_EQ(route.match(request), true);
}

TEST(route, match_basic_unmatch) {
  netflex::http::request request;
  request.set_target("/users/2/articles");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/1/articles", nullptr);
  EXPECT_EQ(route.match(request), false);
}

TEST(route, match_empty_variable_unmatch) {
  netflex::http::request request;
  request.set_target("/users//articles");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/:user_id/articles", nullptr);
  EXPECT_EQ(route.match(request), false);
}

TEST(route, match_empty_variable_unmatch_multi_slashes) {
  netflex::http::request request;
  request.set_target("/users///articles");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/:user_id/articles", nullptr);
  EXPECT_EQ(route.match(request), false);
}

TEST(route, match_empty_variable_unmatch_missing_begin) {
  netflex::http::request request;
  request.set_target("/1/articles");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/1/articles", nullptr);
  EXPECT_EQ(route.match(request), false);
}

TEST(route, match_empty_variable_unmatch_missing_end) {
  netflex::http::request request;
  request.set_target("/users/1");
  request.set_method(netflex::http::method::GET);

  netflex::routing::route route(netflex::http::method::GET, "/users/1/articles", nullptr);
  EXPECT_EQ(route.match(request), false);
}


//!
//! dispatch
//!
TEST(route, dispatch) {
  netflex::routing::route route(netflex::http::method::GET, "", [](const netflex::http::request& request, netflex::http::response& response) {
    EXPECT_EQ(request.get_body(), "0");
    EXPECT_EQ(response.get_body(), "0");
  });

  //! initial setup
  netflex::http::request request;
  netflex::http::response response;

  request.set_body("0");
  response.set_body("0");

  //! dispatch
  route.dispatch(request, response);
}

TEST(route, dispatch_null_callback) {
  netflex::routing::route route(netflex::http::method::GET, "", nullptr);

  //! initial setup
  netflex::http::request request;
  netflex::http::response response;

  request.set_body("0");
  response.set_body("0");

  //! dispatch
  route.dispatch(request, response);
  //! should not crash
}
