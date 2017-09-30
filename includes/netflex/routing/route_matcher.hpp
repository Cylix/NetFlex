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

//!
//! hande the route matching logic
//! build regex for a given route of the server and match requested path by http clients
//!
class route_matcher {
public:
  //!
  //! ctor
  //!
  //! \param path path associated to the route. It will be used to build an regex representation of the route to be used for matching
  //!
  explicit route_matcher(const std::string& path);

  //! default dtor
  ~route_matcher(void) = default;

  //! copy ctor
  route_matcher(const route_matcher&) = default;
  //! assignment operator
  route_matcher& operator=(const route_matcher&) = default;

public:
  //!
  //! match the given path with the underlying route
  //!
  //! \param path path to match
  //! \param params place where to store params of the requested path (for example /articles/1?author=simon) will
  //!        store {article:1, author: simon} for underlying route /articles/:id.
  //!        store nothing if mismatch
  //! \return whether the path matched or not
  //!
  bool match(const std::string& path, params_t& params) const;

protected:
  //!
  //! build matching regex
  //! that is, given a path, construct a regex that can be used by match() and store it in m_match_regex
  //!
  //! \param path path to be used for regex construction
  //!
  void build_match_regex(const std::string& path);

  //!
  //! match GET request params given a requested path
  //!
  //! \param path path to match
  //! \param params place where to store params of the requested path (for example /articles/1?author=simon) will
  //!        store {author: simon}.
  //!
  void match_get_params(const std::string& path, params_t& params) const;

protected:
  //!
  //! matching regex (string object)
  //!
  std::string m_match_regex_str;

  //!
  //! matching regex (regex object)
  //!
  std::regex m_match_regex;

  //!
  //! url params to match, in order of appearance
  //! for example, for route /articles/:id/author/:name, this would be {id, name}
  //!
  std::vector<std::string> m_url_params;
};

} // namespace routing

} // namespace netflex
