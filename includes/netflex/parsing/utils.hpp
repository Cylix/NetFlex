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
#include <vector>

namespace netflex {

namespace parsing {

namespace utils {

//! whitespace token
extern const char SP;

//! whitespace token
extern const char HTAB;

//! whitespace token
extern const char VT;

//! whitespace token
extern const char FF;

//! whitespace token
extern const char CR;

//! whitespace token
extern const char LF;

//!
//! check if input characted is a space delimiter (space or htab)
//!
//! \param c char to check
//! \return whether c is a space
//!
bool is_space_delimiter(char c);

//!
//! check if input characted is a whitespace delimiter (sp, htab, vt, ff, cr, lf)
//!
//! \param c char to check
//! \return whether c is a whitespace
//!
bool is_whitespace_delimiter(char c);

//!
//! check if the input string is a CRLF sequence
//!
//! \param buffer string to check
//! \return whether buffer is a CRLF sequence
//!
bool is_crlf(const std::string& buffer);

//!
//! consume all characters until a non-whitespace character is met
//!
//! \param buffer buffer in which chars have to be consumed
//! \return last whitespace character
//!
char consume_whitespaces(std::string& buffer);

//!
//! consume one word (until whitespaces are met or ending is met)
//!
//! \param buffer buffer in which chars have to be consumed
//! \param ending ending character
//! \return consumed word
//!
std::string consume_word(std::string& buffer, char ending = 0);

//!
//! consume multiple words at once (char-sequenced splited by space delimiters, SP or HTAB)
//!
//! \param buffer buffer in which chars have to be consumed
//! \return consumed words
//!
std::string consume_words(std::string& buffer);

//!
//! same as consume words, but with a specified ending char
//!
//! \param buffer buffer in which chars have to be consumed
//! \param ending ending character
//! \return consumed words
//!
std::string consume_word_with_ending(std::string& buffer, char ending);

//!
//! consume a CRLF sequence, only if the buffer contains a CRLF sequence
//!
//! \param buffer buffer in which chars have to be consumed
//! \return whether a CRLF sequence was consumed or not
//!
bool consume_crlf(std::string& buffer);

//!
//! wrapper of consume_words & consume_whitespaces
//! dismiss whitespaces before word and fetch word
//! can be called multiple times with same out string in case first call did not provide enough data in input buffer
//!
//! \param buffer buffer in which chars have to be consumed
//! \param out where to store consumed words
//! \return whether words were fully parsed or not
//!
bool parse_words(std::string& buffer, std::string& out);

//!
//! wrapper of consume_word & consume_whitespaces
//! dismiss whitespaces before word and fetch word
//! can be called multiple times with same out string in case first call did not provide enough data in input buffer
//!
//! \param buffer buffer in which chars have to be consumed
//! \param out where to store consumed word
//! \return whether word was fully parsed or not
//!
bool parse_next_word(std::string& buffer, std::string& out);

//!
//! same as parse_next_word, but with ending
//! wrapper of consume_word_with_ending & consume_whitespaces
//! dismiss whitespaces before word and fetch word
//! can be called multiple times with same out string in case first call did not provide enough data in input buffer
//!
//! \param buffer buffer in which chars have to be consumed
//! \param word where to store consumed word
//! \param ending ending character
//! \return whether word was fully parsed or not
//!
bool parse_next_word_with_ending(std::string& buffer, std::string& word, char ending);

//!
//! split string based on given sep
//!
//! \param str string to split
//! \param sep separator to use for split
//! \return splited string
//!
std::vector<std::string> split(const std::string& str, char sep);

//!
//! left and right trim string (in place)
//!
//! \param str string to trim
//!
void trim(std::string& str);

//!
//! left trim string (in place)
//!
//! \param str string to trim
//!
void ltrim(std::string& str);

//!
//! right trim string (in place)
//!
//! \param str string to trim
//!
void rtrim(std::string& str);

//!
//! convert all chars to lower case (in place)
//!
//! \param str string to lower
//!
void to_lower(std::string& str);

//!
//! convert all chars to upper case (in place)
//!
//! \param str string to upper
//!
void to_upper(std::string& str);

} // namespace utils

} // namespace parsing

} // namespace netflex
