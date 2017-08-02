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

#include <regex>
#include <string>
#include <vector>

#include <netflex/routing/params.hpp>

namespace netflex {

namespace routing {

class route_matcher {
public:
  //! ctor & dtor
  explicit route_matcher(const std::string& path);
  ~route_matcher(void) = default;

  //! copy ctor & assignment operator
  route_matcher(const route_matcher&) = default;
  route_matcher& operator=(const route_matcher&) = default;

public:
  //! matching
  bool match(const std::string& path, params_t& params) const;

protected:
  //! build matching regex
  void build_match_regex(const std::string& path);

  //! matching
  void match_get_params(const std::string& path, params_t& params) const;

protected:
  //! matching regex
  std::string m_match_regex_str;
  std::regex m_match_regex;
  //! url params to match, in order of appearance
  std::vector<std::string> m_url_params;
};

} // namespace routing

} // namespace netflex
