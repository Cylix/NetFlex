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

#include <string>

namespace netflex {

namespace parsing {

namespace utils {

//! whitespace tokens
extern const char SP;
extern const char HTAB;
extern const char VT;
extern const char FF;
extern const char CR;
extern const char LF;

//! parsing helper
bool is_space_delimiter(char c);
bool is_whitespace_delimiter(char c);
bool is_crlf(const std::string& buffer);

//! consumers
char consume_whitespaces(std::string& buffer);
std::string consume_word(std::string& buffer, char ending = 0);
std::string consume_words(std::string& buffer);
std::string consume_word_with_ending(std::string& buffer, char ending);
bool consume_crlf(std::string& buffer);

//! parsing wrapper
bool parse_words(std::string& buffer, std::string& out);
bool parse_next_word(std::string& buffer, std::string& out);
bool parse_next_word_with_ending(std::string& buffer, std::string& word, char ending);

} // namespace utils

} // namespace parsing

} // namespace netflex
