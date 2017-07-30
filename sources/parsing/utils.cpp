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

#include <algorithm>
#include <regex>

#include <netflex/misc/error.hpp>
#include <netflex/parsing/utils.hpp>

namespace netflex {

namespace parsing {

namespace utils {

//!
//! whitespace tokens
//!
const char SP   = ' ';
const char HTAB = '\t';
const char VT   = 0x0b;
const char FF   = 0x0c;
const char CR   = 0x0d;
const char LF   = '\n';


//!
//! parsing helper
//!
bool
is_space_delimiter(char c) {
  return c == SP
         || c == HTAB;
}

bool
is_whitespace_delimiter(char c) {
  return c == SP
         || c == HTAB
         || c == VT
         || c == FF
         || c == CR;
}

bool
is_crlf(const std::string& buffer) {
  //! crlf is 2 bytes, CR & LF
  if (buffer.size() < 2)
    return false;

  return buffer[0] == CR && buffer[1] == LF;
}


//!
//! consumers
//!
char
consume_whitespaces(std::string& buffer) {
  size_t i = 0;

  if (buffer.empty())
    return 0;

  while (i < buffer.size() && utils::is_whitespace_delimiter(buffer[i])) {
    ++i;
  }

  char last_consumed_whitespace = buffer[i];
  buffer.erase(0, i);

  return last_consumed_whitespace;
}

std::string
consume_word(std::string& buffer, char ending) {
  size_t i = 0;

  while (i < buffer.size() && !utils::is_whitespace_delimiter(buffer[i]) && buffer[i] != ending) {
    ++i;
  }

  if (i == buffer.size()) {
    return std::move(buffer);
  }
  else {
    std::string word = buffer.substr(0, i);
    buffer.erase(0, i);

    return word;
  }
}

std::string
consume_words(std::string& buffer) {
  size_t i = 0;

  while (i < buffer.size() && !(utils::is_whitespace_delimiter(buffer[i]) && !utils::is_space_delimiter(buffer[i]))) {
    ++i;
  }

  if (i == buffer.size()) {
    return std::move(buffer);
  }
  else {
    std::string word = buffer.substr(0, i);
    buffer.erase(0, i);

    return word;
  }
}

std::string
consume_word_with_ending(std::string& buffer, char ending) {
  std::string word = consume_word(buffer, ending);

  if (!buffer.empty() && buffer[0] != ending)
    __NETFLEX_THROW(error, "expected character '" + std::to_string(ending) + "', got '" + std::to_string(buffer[0]) + "'");

  return word;
}

bool
consume_crlf(std::string& buffer) {
  if (!is_crlf(buffer))
    return false;

  buffer.erase(0, 2);
  return true;
}


//!
//! parsing wrapper
//!
bool
parse_words(std::string& buffer, std::string& out) {
  //! dismiss preceding whitespaces if word has not been started to be consumed
  if (out.empty())
    utils::consume_whitespaces(buffer);

  //! find word
  out += utils::consume_words(buffer);

  //! determine whether we finished to parse the words
  //! if we don't have any more characters in the buffer, we haven't finish
  return !buffer.empty();
}

bool
parse_next_word(std::string& buffer, std::string& word) {
  //! dismiss preceding whitespaces if word has not been started to be consumed
  if (word.empty())
    utils::consume_whitespaces(buffer);

  //! find word
  word += utils::consume_word(buffer);

  //! determine whether we finished to parse the word
  //! if we don't have any more characters in the buffer, then we did not reach any whitespace
  //! thus, we haven't finish
  return !buffer.empty();
}

bool
parse_next_word_with_ending(std::string& buffer, std::string& word, char ending) {
  //! dismiss preceding whitespaces if word has not been started to be consumed
  if (word.empty())
    utils::consume_whitespaces(buffer);

  //! find word
  word += utils::consume_word_with_ending(buffer, ending);

  //! determine whether we finished to parse the word
  //! if we don't have any more characters in the buffer, then we did not reach the ending character
  //! thus, we haven't finish
  return !buffer.empty();
}


//!
//! string manipulation
//!
std::vector<std::string>
split(const std::string& str, char sep) {
  std::regex re(std::string(1, sep));

  //! passing -1 as the submatch index parameter performs splitting
  std::sregex_token_iterator
    first{str.begin(), str.end(), re, -1},
    last;

  return {first, last};
}

void
trim(std::string& str) {
  ltrim(str);
  rtrim(str);
}

void
ltrim(std::string& str) {
  str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
    return !std::isspace(ch);
  }));
}

void
rtrim(std::string& str) {
  str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
    return !std::isspace(ch);
  })
              .base(),
    str.end());
}

void
to_lower(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void
to_upper(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

} // namespace utils

} // namespace parsing

} // namespace netflex
