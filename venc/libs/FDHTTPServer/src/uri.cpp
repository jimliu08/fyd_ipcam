#include <fd-http-server/uri.hpp>
#include <fd-http-server/utils.hpp>
#include <iostream>

namespace vtcs
{

URI::URI()
{
}

void URI::Clear()
{
	scheme.clear();
	userinfo.clear();
	hostname.clear();
	port.clear();
	path.clear();
	query_strings.clear();
	fragment_id.clear();
}

// ===============================================================

/// Function to print all data in the URI_Struct.
void URIUtility::PrintURI(const URI& uri)
{
	std::cout << "--------- URI start ---------" << std::endl;
	std::cout << "Scheme: " << uri.scheme << std::endl;
	std::cout << "User Info: " << uri.userinfo << std::endl;
	std::cout << "Hostname: " << uri.hostname << std::endl;
	std::cout << "Port: " << uri.port << std::endl;
	std::cout << "Path: " << uri.path << std::endl;

	std::cout << "Query:" << std::endl;

	URI::QueryStrContainer::const_iterator iter = uri.query_strings.begin();
	for(; iter!= uri.query_strings.end(); ++iter)
	{
		std::cout << "( " << iter->first << " , " << iter->second << " )" << std::endl;
	}

	std::cout << "Fragment ID: " << uri.fragment_id << std::endl;

	std::cout << "--------- URI end ---------" << std::endl;
}

#ifdef USE_UNESCAPE
/**
 * @brief Function to convert hexadecimal character to decimal unsigned integer.
 *
 * @param[in] ch The input hexadecimal character.
 * @return The output decimal unsigned integer.
 */
unsigned int URIUtility::CharHex2DecUint(char ch)
{
	if( (ch >= '0') && (ch <= '9') )
	{
		return (unsigned int)(ch - '0');
	}

	if(ch >= 'A' && ch <= 'F')
	{
		return ( (unsigned int) (ch - 'A') + 10 );
	}

	if(ch >= 'a' && ch <= 'f')
	{
		return ( (unsigned int) (ch - 'a') + 10 );
	}

	return 16;
}

/**
 * @brief Function to unescape (decode) the URI string. (For Percent-encoding)
 *
 * @param[in] str The input URI string.
 * @param[out] result The decoded URI string.
 * @return true: Success to decode the URI string. false: Failed to decode the URI string.
 */
bool URIUtility::Unescape(const std::string& str, std::string& result)
{
	// SPACE is encoded as '+' or %20
	// If we don't replace them, + won't be unescaped properly and we get validation errors.
	std::string temp_str = str;
	size_t temp_index = 0;
	while( (temp_index = temp_str.find_first_of('+')) != std::string::npos )
	{
		temp_str.replace(temp_index, 1, "%20");
	}

	for(size_t i = 0; i < temp_str.length(); )
	{
		if(temp_str[i] != '%')
		{
			// The character is normal data.
			result.push_back(temp_str[i]);
			++i;
			continue;
		}

		// Check it has one byte encoded data (There is one % followed by two encoded hex characters).
		if( (i + 2) >= temp_str.length() )
			return false;

		// Decode the data.
		unsigned int high_part = URIUtility::CharHex2DecUint(temp_str[i + 1]);
		unsigned int low_part = URIUtility::CharHex2DecUint(temp_str[i + 2]);

		if( (high_part > 15) || (low_part > 15) )
			return false;

		result.push_back( (char) ( (high_part << 4) | low_part) );

		i += 3;
	}

	return true;
}
#endif

/**
 * @brief Function to parse query string of URI.
 *
 * @param[in] query_str The query string of URI.
 * @param[out] key_value_map The parsed key, value pairs.
 * @return true: Success, false: Failed.
 */
bool URIUtility::ParseQueryString(const std::string& query_str, URI::QueryStrContainer& key_value_map)
{
	// Separate each pair of key and value.
	size_t index = 0;
	std::deque<std::string> pair_tokens;
	Utils::Tokenize2String("&;", query_str, pair_tokens);
	std::deque<std::string>::iterator tok_iter = pair_tokens.begin();

	for(; tok_iter != pair_tokens.end(); ++tok_iter)
	{
		// Try to get the key and value.
		index = tok_iter->find('=');
		if(index == std::string::npos)
			return false;

		key_value_map[tok_iter->substr(0, index)] = ( (index + 1) < tok_iter->length() ) ? tok_iter->substr(index + 1) : "";
	}

	return true;
}

} // namespace vtcs
