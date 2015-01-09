#include <fd-http-server/utils.hpp>

namespace vtcs
{

size_t Utils::Tokenize2String(const std::string& delimiter, const std::string& str, std::deque<std::string>& tokens)
{
        // Find the character which is not the delimiters. 
        size_t start_pos = str.find_first_not_of(delimiter, 0);
        // Find the delimiter.
        size_t end_pos = str.find_first_of(delimiter, start_pos);

        while (std::string::npos != end_pos || std::string::npos != start_pos)
        {
                tokens.push_back(str.substr(start_pos, end_pos - start_pos));
                start_pos = str.find_first_not_of(delimiter, end_pos);
                end_pos = str.find_first_of(delimiter, start_pos);
        }

        return tokens.size();
}

} // namespace vtcs
