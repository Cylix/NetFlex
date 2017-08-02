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
//! test_route_matcher
//! inherit from route_matcher to access private fields / methods
//!
class test_route_matcher : public netflex::routing::route_matcher {
public:
  //! ctor
  test_route_matcher(const std::string& path)
  : netflex::routing::route_matcher(path) {}

  //! getters, mostly for testing purpose
  const std::string&
  get_match_regex_str(void) const {
    return m_match_regex_str;
  }

  const std::vector<std::string>&
  get_url_params(void) const {
    return m_url_params;
  }

  //! build matching regex
  void
  test_build_match_regex(const std::string& path) {
    build_match_regex(path);
  }

  //! matching
  void
  test_match_get_params(const std::string& path, netflex::routing::params_t& params) const {
    match_get_params(path, params);
  }
};

//! regex constants
static const std::string PATH_REGEX_MATCH_SUFIX = "/?((\\?([^=]+)=([^&\\#]*))(&([^=]+)=([^&\\#]*))*)?(\\#.*)*";
static const std::string PATH_REGEX_VAR_MATCH   = "/([a-zA-Z0-9_\\-]+)";

//!
//! match
//!
TEST(match, match) {
  test_route_matcher matcher("/");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/", params), true);

  EXPECT_EQ(params.size(), 0UL);
}

TEST(match, match_no_variable) {
  test_route_matcher matcher("/users/1/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/1/articles", params), true);

  EXPECT_EQ(params.size(), 0UL);
}

TEST(match, match_solo_variable) {
  test_route_matcher matcher("/users/:user_id/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/42/articles", params), true);

  EXPECT_EQ(params.size(), 1UL);
  EXPECT_EQ(params["user_id"], "42");
}

TEST(match, match_multi_variable) {
  test_route_matcher matcher("/users/:user_id/articles/:article_id/comments/:comment_id/author");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/42/articles/84/comments/21/author", params), true);

  EXPECT_EQ(params.size(), 3UL);
  EXPECT_EQ(params["user_id"], "42");
  EXPECT_EQ(params["article_id"], "84");
  EXPECT_EQ(params["comment_id"], "21");
}

TEST(match, match_alphanum_variable) {
  test_route_matcher matcher("/users/:user_id-0/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/42/articles", params), true);

  EXPECT_EQ(params.size(), 1UL);
  EXPECT_EQ(params["user_id-0"], "42");
}

TEST(match, match_solo_url_params) {
  test_route_matcher matcher("/users/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/articles?user_id=42", params), true);

  EXPECT_EQ(params.size(), 1UL);
  EXPECT_EQ(params["user_id"], "42");
}

TEST(match, match_multi_url_params) {
  test_route_matcher matcher("/users/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/articles?user_id=42&article_id=84&comment_id=21", params), true);

  EXPECT_EQ(params.size(), 3UL);
  EXPECT_EQ(params["user_id"], "42");
  EXPECT_EQ(params["article_id"], "84");
  EXPECT_EQ(params["comment_id"], "21");
}

TEST(match, match_multi_empty_url_params) {
  test_route_matcher matcher("/users/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/articles?user_id=42&article_id=&comment_id=21", params), true);

  EXPECT_EQ(params.size(), 3UL);
  EXPECT_EQ(params["user_id"], "42");
  EXPECT_EQ(params["article_id"], "");
  EXPECT_EQ(params["comment_id"], "21");
}

TEST(match, match_combination) {
  test_route_matcher matcher("/users/:user_id/articles/:article_id");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/42/articles/84/?comment_id=21&source=google", params), true);

  EXPECT_EQ(params.size(), 4UL);
  EXPECT_EQ(params["user_id"], "42");
  EXPECT_EQ(params["article_id"], "84");
  EXPECT_EQ(params["comment_id"], "21");
  EXPECT_EQ(params["source"], "google");
}

TEST(match, match_combination_conflict) {
  test_route_matcher matcher("/users/:user_id/articles/:article_id");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/42/articles/84/?comment_id=21&user_id=1&source=google", params), true);

  EXPECT_EQ(params.size(), 4UL);
  EXPECT_EQ(params["user_id"], "1");
  EXPECT_EQ(params["article_id"], "84");
  EXPECT_EQ(params["comment_id"], "21");
  EXPECT_EQ(params["source"], "google");
}

TEST(match, match_complex) {
  test_route_matcher matcher("/users/:user_id/articles/:article_id/comments/:comment_id/author");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/42/articles/84/comments/21/author?user_id=1&source=google&test=#anchor", params), true);

  EXPECT_EQ(params.size(), 5UL);
  EXPECT_EQ(params["user_id"], "1");
  EXPECT_EQ(params["article_id"], "84");
  EXPECT_EQ(params["comment_id"], "21");
  EXPECT_EQ(params["source"], "google");
  EXPECT_EQ(params["test"], "");
}

TEST(match, match_trailing_slash) {
  test_route_matcher matcher("/users/1/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/1/articles/", params), true);
}

TEST(match, match_basic_unmatch) {
  test_route_matcher matcher("/users/1/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/2/articles", params), false);
}

TEST(match, match_empty_variable_unmatch) {
  test_route_matcher matcher("/users/:user_id/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users//articles", params), false);
}

TEST(match, match_empty_variable_unmatch_multi_slashes) {
  test_route_matcher matcher("/users/:user_id/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users///articles", params), false);
}

TEST(match, match_empty_variable_unmatch_missing_begin) {
  test_route_matcher matcher("/users/1/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/1/articles", params), false);
}

TEST(match, match_empty_variable_unmatch_missing_end) {
  test_route_matcher matcher("/users/1/articles");

  netflex::routing::params_t params;
  EXPECT_EQ(matcher.match("/users/1", params), false);
}

//!
//! build_match_regex specs
//!
TEST(route_matcher, build_match_regex) {
  test_route_matcher matcher("");

  matcher.test_build_match_regex("/");
  EXPECT_EQ(matcher.get_match_regex_str(), "/" + PATH_REGEX_MATCH_SUFIX);
  EXPECT_EQ(matcher.get_url_params().size(), 0UL);
}

TEST(route_matcher, build_match_regex_empty_path) {
  test_route_matcher matcher("");

  matcher.test_build_match_regex("");
  EXPECT_EQ(matcher.get_match_regex_str(), PATH_REGEX_MATCH_SUFIX);
  EXPECT_EQ(matcher.get_url_params().size(), 0UL);
}


TEST(route_matcher, build_match_regex_multi_path) {
  test_route_matcher matcher("");

  matcher.test_build_match_regex("/abc/def/ghijk");
  EXPECT_EQ(matcher.get_match_regex_str(), "/abc/def/ghijk" + PATH_REGEX_MATCH_SUFIX);
  EXPECT_EQ(matcher.get_url_params().size(), 0UL);
}

TEST(route_matcher, build_match_regex_solo_variable) {
  test_route_matcher matcher("");

  matcher.test_build_match_regex("/users/:user_id/articles");
  EXPECT_EQ(matcher.get_match_regex_str(), "/users" + PATH_REGEX_VAR_MATCH + "/articles" + PATH_REGEX_MATCH_SUFIX);
  EXPECT_EQ(matcher.get_url_params().size(), 1UL);
  EXPECT_EQ(matcher.get_url_params()[0], "user_id");
}

TEST(route_matcher, build_match_regex_multi_variables) {
  test_route_matcher matcher("");

  matcher.test_build_match_regex("/users/:user_id/articles/:article_id/comments/:comment_id/author");
  EXPECT_EQ(matcher.get_match_regex_str(), "/users" + PATH_REGEX_VAR_MATCH + "/articles" + PATH_REGEX_VAR_MATCH + "/comments" + PATH_REGEX_VAR_MATCH + "/author" + PATH_REGEX_MATCH_SUFIX);
  EXPECT_EQ(matcher.get_url_params().size(), 3UL);
  EXPECT_EQ(matcher.get_url_params()[0], "user_id");
  EXPECT_EQ(matcher.get_url_params()[1], "article_id");
  EXPECT_EQ(matcher.get_url_params()[2], "comment_id");
}

TEST(route_matcher, build_match_regex_alphanum_variable) {
  test_route_matcher matcher("");

  matcher.test_build_match_regex("/users/:user_id-0/articles");
  EXPECT_EQ(matcher.get_match_regex_str(), "/users" + PATH_REGEX_VAR_MATCH + "/articles" + PATH_REGEX_MATCH_SUFIX);
  EXPECT_EQ(matcher.get_url_params().size(), 1UL);
  EXPECT_EQ(matcher.get_url_params()[0], "user_id-0");
}

TEST(route_matcher, build_match_regex_incomplete_variable) {
  test_route_matcher matcher("");

  matcher.test_build_match_regex("/users/:/articles");
  EXPECT_EQ(matcher.get_match_regex_str(), "/users/:/articles" + PATH_REGEX_MATCH_SUFIX);
  EXPECT_EQ(matcher.get_url_params().size(), 0UL);
}

TEST(route_matcher, build_match_regex_subsequent_slashes) {
  test_route_matcher matcher("");

  matcher.test_build_match_regex("//users///1//articles///");
  EXPECT_EQ(matcher.get_match_regex_str(), "//users///1//articles///" + PATH_REGEX_MATCH_SUFIX);
  EXPECT_EQ(matcher.get_url_params().size(), 0UL);
}

//!
//! match_get_params
//!
TEST(route_matcher, match_get_params) {
  test_route_matcher matcher("");

  netflex::routing::params_t params;
  matcher.test_match_get_params("", params);

  EXPECT_EQ(params.size(), 0UL);
}

TEST(route_matcher, match_get_params_solo_var) {
  test_route_matcher matcher("");

  netflex::routing::params_t params;
  matcher.test_match_get_params("?var=val", params);

  EXPECT_EQ(params.size(), 1UL);
  EXPECT_EQ(params["var"], "val");
}

TEST(route_matcher, match_get_params_multi_var) {
  test_route_matcher matcher("");

  netflex::routing::params_t params;
  matcher.test_match_get_params("?var_1=val_1&var_2=val_2&var_3=val_3", params);

  EXPECT_EQ(params.size(), 3UL);
  EXPECT_EQ(params["var_1"], "val_1");
  EXPECT_EQ(params["var_2"], "val_2");
  EXPECT_EQ(params["var_3"], "val_3");
}

TEST(route_matcher, match_get_params_multi_var_conflict) {
  test_route_matcher matcher("");

  netflex::routing::params_t params;
  matcher.test_match_get_params("?var_1=val_1&var_2=val_2&var_3=val_3&var_2=val_4", params);

  EXPECT_EQ(params.size(), 3UL);
  EXPECT_EQ(params["var_1"], "val_1");
  EXPECT_EQ(params["var_2"], "val_4");
  EXPECT_EQ(params["var_3"], "val_3");
}

TEST(route_matcher, match_get_params_multi_var_empty) {
  test_route_matcher matcher("");

  netflex::routing::params_t params;
  matcher.test_match_get_params("?var_1=val_1&var_2=&var_3=val_3", params);

  EXPECT_EQ(params.size(), 3UL);
  EXPECT_EQ(params["var_1"], "val_1");
  EXPECT_EQ(params["var_2"], "");
  EXPECT_EQ(params["var_3"], "val_3");
}

TEST(route_matcher, match_get_params_multi_var_special) {
  test_route_matcher matcher("");

  netflex::routing::params_t params;
  matcher.test_match_get_params("?var_1==&var_2=&var_3=val_3", params);

  EXPECT_EQ(params.size(), 3UL);
  EXPECT_EQ(params["var_1"], "=");
  EXPECT_EQ(params["var_2"], "");
  EXPECT_EQ(params["var_3"], "val_3");
}
