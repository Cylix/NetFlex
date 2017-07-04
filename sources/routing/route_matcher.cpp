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

#include <netflex/routing/route_matcher.hpp>

#include <iostream>

namespace netflex {

namespace routing {

//!
//! ctor & dtor
//!
route_matcher::route_matcher(const std::string& path) {
  build_match_regex(path);
}


//!
//! build matching regex
//!
void
route_matcher::build_match_regex(const std::string& path) {
  //! match var1 in /abc/:var1/def
  std::regex find_url_params_regex("/:([a-zA-Z0-9]*)");
  std::smatch sm;

  auto params_it  = std::sregex_iterator(path.cbegin(), path.cend(), find_url_params_regex);
  auto params_end = std::sregex_iterator();

  while (params_it != params_end) {
    auto param = params_it->str();

    if (!std::regex_match(param, sm, find_url_params_regex))
      continue;

    //! sm is [/:var, var]
    m_url_params.push_back(sm[1]);

    ++params_it;
  }

  //! transform /abc/:var1/def into /abc/([a-zA-Z0-9]*)/def to match url params values
  //! also match trailing slash, get params and #comments
  //!
  //! regex_replace(path, reg, "/([a-zA-Z0-9]*)") ==> transform /abc/:var1/def into /abc/([a-zA-Z0-9]*)/def
  //! "/?((\\?([^=]+)=([^&\\#]*))(&([^=]+)=([^&\\#]*))*)?(\\#.*)?" ==> match trailing slash, get params and #comments
  //!  > /? ==> match trailing slash
  //!  > ((\\?([^=]+)=([^&\\#]*))(&([^=]+)=([^&\\#]*))*)? ==> match get params
  //!    > (\\?([^=]+)=([^&\\#]*)) ==> match first ?var=val
  //!    > (&([^=]+)=([^&\\#]*))*)?(\\#.*)? ==> match subsequent &var=val
  //!  > (\\#.*)? ==> match #comments
  m_match_regex = std::regex(std::regex_replace(path, find_url_params_regex, "/([a-zA-Z0-9]*)") + "/?((\\?([^=]+)=([^&\\#]*))(&([^=]+)=([^&\\#]*))*)?(\\#.*)*");
}


//!
//! matching
//!
bool
route_matcher::match(const std::string& path, params_t& params) const {
  std::smatch sm;

  if (!std::regex_match(path, sm, m_match_regex))
    return false;

  //! expected url params are in sm[1..m_url_params.size()]
  for (size_t i = 1; i <= m_url_params.size(); ++i)
    params[m_url_params[i - 1]] = sm[i];

  //! sm[m_url_params.size() + 1] contains get_params
  match_get_params(sm[m_url_params.size() + 1], params);

  return true;
}

void
route_matcher::match_get_params(const std::string& path, params_t& params) const {
  std::regex params_regex("[\\?&]([^=]+)=([^&]+)");
  std::smatch sm;

  auto params_it  = std::sregex_iterator(path.cbegin(), path.cend(), params_regex);
  auto params_end = std::sregex_iterator();

  while (params_it != params_end) {
    auto param = params_it->str();

    if (!std::regex_match(param, sm, params_regex))
      continue;

    //! sm is [?key1=val1, key1, val1]
    params[sm[1]] = sm[2];

    ++params_it;
  }
}

} // namespace routing

} // namespace netflex
