#ifndef _PHP_H_
#define _PHP_H_

/*
 * cpplibphp - C++ PHP library.
 * A single header C++ library containing useful functions based on those from the PHP standard library.
 * https://github.com/rmarder/cpplibphp
 *
 * Copyright (C) 2021 Robert Alex Marder (ram@robertmarder.com)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <climits>
#include <vector>
#include <sstream>
#include <utility>

// php constants
// these can be set to anything as long as they are unique
// we only test == we never test >< etc
const size_t STR_PAD_RIGHT = 1;
const size_t STR_PAD_LEFT = 2;
const size_t STR_PAD_BOTH = 3;

namespace php
{

/********************/
/* string functions */
/********************/

	std::string ltrim(const std::string &str, const std::string &whitespace = " \n\r\t\f\v")
	{
		size_t start = str.find_first_not_of(whitespace);
		if(start == std::string::npos)
		{
			return "";
		}
		return str.substr(start);
	}
 
	std::string rtrim(const std::string &str, const std::string &whitespace = " \n\r\t\f\v")
	{
		size_t end = str.find_last_not_of(whitespace);
		if(end == std::string::npos)
		{
			return "";
		}
		return str.substr(0, end + 1);
	}

	std::string trim(const std::string &str, const std::string &whitespace = " \n\r\t\f\v")
	{
		return rtrim(ltrim(str, whitespace), whitespace);
	}

	// C++24 supposedly will have it's own contains. This matches str_contains() from PHP 8
	bool str_contains(const std::string &haystack, const std::string &needle)
	{
		if(haystack.find(needle) != std::string::npos)
		{
			return true;
		}
		return false;
	}

	std::string strrev(std::string str)
	{
		reverse(str.begin(), str.end());
		return str;
	}

	std::string str_rot13(const std::string &str)
	{
		std::string result;
		for(size_t i = 0; i < str.size(); i++)
		{
			if(isalpha(str[i]))
			{
				if((tolower(str[i]) - 'a') < 14)
				{
					result.append(1, str[i] + 13);
				}
				else
				{
					result.append(1, str[i] - 13);
				}
			}
			else
			{
				result.append(1, str[i]);
			}
		}
		return result;
	}

	namespace {
		// offset is for our internal recursion, we don't want anyone to call using that argument
		// by sticking this inside an anonymous namespace, we guarentee it.
		std::string _str_replace(const std::string &search, const std::string &replace, const std::string &subject, size_t offset = 0)
		{
			std::string result;
			size_t found;

			//printf("search = [%s] replace = [%s] subject = [%s]\n", search.c_str(), replace.c_str(), subject.c_str());

			// find the starting position of search in subject
			// offset ensures that if we are calling this function recursively, we don't match what we just replaced again.
			found = subject.find(search, offset);
			if(found == std::string::npos)
			{
				// search not found in subject. returning subject untouched.
				return subject;
			}

			// we found a match, do the replacement
			result = subject;
			result.replace(found, search.size(), replace);

			// recursive function. are there more instances of search in subject to replace?
			found = result.find(search, offset + search.size());
			if(found == std::string::npos)
			{
				// no more matches
				return result;
			}
			return _str_replace(search, replace, result, found + search.size() + 1);
		}
	}

	std::string str_replace(const std::string &search, const std::string &replace, const std::string &subject)
	{
		return _str_replace(search, replace, subject);
	}

	std::string str_repeat(const std::string &str, const size_t times)
	{
		size_t loop;
		std::string result;

		loop = 0;
		while(loop < times)
		{
			result.append(str);
			loop++;
		}
		return result;
	}

	std::string str_pad(const std::string &input, size_t length, const std::string pad_str = " ", size_t pad_type = STR_PAD_RIGHT)
	{
		std::string result;

		// no length available to do any padding. return input untouched.
		if(input.size() >= length) { return input; }

		if(pad_type == STR_PAD_RIGHT || pad_type == STR_PAD_LEFT || pad_type == STR_PAD_BOTH)
		{
			for(size_t i = input.size(); i < length; i = i + pad_str.size())
			{
				if(input.size() + result.size() + pad_str.size() < length)
				{
					result.append(pad_str);
				}
				else
				{
					result.append(pad_str.substr(0, length - i));
				}
			}
		}

		if(pad_type == STR_PAD_RIGHT)
		{
			result.insert(0, input);
		}
		if(pad_type == STR_PAD_LEFT)
		{
			result.append(input);
		}
		if(pad_type == STR_PAD_BOTH)
		{
			std::cout << "pad both" <<std::endl;
			size_t half = (length - input.size()) / 2;
			std::string tmp = result;
			result = "";

			// this prefers padding right, just like PHP does
			result.append(tmp.substr(0, half));
			result.append(input);
			result.append(tmp.substr(half, std::string::npos));
		}
		return result;
	}

	std::string ucfirst(std::string str)
	{
		str[0]=toupper(str[0]);
		return str;
	}

	std::string lcfirst(std::string str)
	{
		str[0]=tolower(str[0]);
		return str;
	}


/******************
* array functions *
*******************
*/
	namespace
	{
		// explode implementation helper. we don't want people caling this, thus the anonymous namespace.
		// given a search delimited string subject, return the segment requested.
		// eg:  _explode_segment("||", "this||is||a||string", result, 2) // result == "a"
		// if segment was found return true, if segment was not found return false
		bool _explode_segment(const std::string &search, const std::string &subject, std::string &result, size_t segment = 0)
		{
			//std::string result;
			std::string looptest;
			size_t found_segment;

			// segment 0 is a special case because we won't find search immediately before it.
			// instead, just return from the start of string until we encounter search, not including search itself.
			if(segment == 0)
			{
				found_segment = subject.find(search, 0);
				result = subject.substr(0, found_segment);
				//std::cout << "found segment: 0 " << " result: " << result << std::endl;
				// we have a good result
				return true;
			}

			found_segment = 0;
			result = "";

			// loop until we find the segment we want
			for(size_t i = 0; i < subject.size(); i++)
			{
				// have we found a segment?
				if(subject.substr(i, search.size()) == search)
				{
					found_segment++;
					result = subject.substr(i + search.size(), std::string::npos);
					//std::cout << "found segment: [" << result << "] | ";
					result = result.substr(0, result.find(search, 0));
					//std::cout << "found segment: [" << result << "]" << std::endl;
					if(found_segment == segment)
					{
						// we have a good result.
						return true;
					}
					i = i + search.size() - 1;
				}
			}
			// we didn't find any segments. signal to caller that result is garbage.
			return false;
		}
	}

	// This does not handle limit like php's explode() does.
	// 1) Negative limit is undefined behavior.
	// 2) If limit is set and positive, the returned array will contain a maximum of limit elements. The last element will NOT contain the rest of string.
	std::vector<std::string> explode(std::string const &search, std::string const &subject, int limit = INT_MAX)
	{
		std::vector<std::string> result_v;
		std::string result;
		int loop = 0;
		bool found = false;
		if(limit == 0)
		{
			// PHP: if the limit parameter is zero, then this is treated as 1.
			limit = 1;
		}
		while(1)
		{
			found = _explode_segment(search, subject, result, loop);
			if(found == false || limit == loop)
			{
				// no more segments to get
				break;
			}
			result_v.push_back(result);
			loop++;
		}
		return result_v;
	}

	std::vector<std::string> str_split(const std::string &str, size_t length = 1)
	{
		std::vector<std::string> result_v;
		std::string result;
		size_t loop;

		if(length > str.size())
		{
			length = str.size();
		}

		loop = 0;
		while(loop < str.size())
		{
			result = str.substr(loop, length);
			result_v.push_back(result);
			loop = loop + length;
		}

		return result_v;
	}

	std::string implode(const std::string &separator, const std::vector<std::string> &array)
	{
		std::string result;
		for(size_t i = 0; i < array.size(); i++)
		{
			result.append(array[i]);
			if(i < array.size() - 1)
			{
				// this prevents a tailing seperator going on the end during the last loop cycle
				result.append(separator);
			}
		}
		return result;
	}

	std::string implode(const std::vector<std::string> &array)
	{
		return implode("", array);
	}
	
}
#endif
