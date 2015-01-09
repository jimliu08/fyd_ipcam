#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <string>
#include <deque>

namespace vtcs
{

class Utils
{
public:
	static size_t Tokenize2String(const std::string& delimiter, const std::string& str, std::deque<std::string>& tokens);
};

} // namespace vtcs

#endif

