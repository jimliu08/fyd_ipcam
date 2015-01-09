#ifndef _URI_HPP_
#define _URI_HPP_

#include <unordered_map>
#include <string>

namespace vtcs
{
// scheme://username:password@domain:port/path?query_string#fragment_id

struct URI
{
public:
	typedef std::unordered_map<std::string, std::string> QueryStrContainer;

	// Functions.
	URI();
	void Clear();

	// Variables.
	std::string scheme;
	std::string userinfo; // username:password
	std::string hostname;
	std::string port;
	std::string path;
	QueryStrContainer query_strings;
	std::string fragment_id;
};

class URIUtility
{
public:
	static void PrintURI(const URI& uri);
#ifdef USE_UNESCAPE
	static unsigned int CharHex2DecUint(char ch);
	static bool Unescape(const std::string& str, std::string& result);
#endif
	static bool ParseQueryString(const std::string& query_str, URI::QueryStrContainer& key_value_map);
};

} // namespace vtcs

#endif

