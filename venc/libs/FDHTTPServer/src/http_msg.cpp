#include <fd-http-server/http_msg.hpp>
#include <sstream>
#include <ctime>

namespace vtcs
{

#if 0
/**
 * @brief Function to transfer the initial line and headers of HTTP request message to c++ standard string.
 *
 * @param[in] msg The HTTP request message.
 * @param[out] result The string contains the initial line and headers of HTTP request message.
 */
void HttpMsgUtility::ComposeHttpRequestMsgExcludeBody(const HttpRequestMsg& msg, std::string& result)
{
	std::stringstream sstream;
	sstream << "HTTP/" << msg.http_version_major << "." << msg.http_version_minor;

	result = msg.method;
	result += " ";
	result += msg.uri.path;

	if(!msg.uri.query_strings.empty())
	{
		URI::QueryStrContainer::const_iterator iter = msg.uri.query_strings.begin();
		if(iter != msg.uri.query_strings.end())
			result += "?";
		for(; iter != msg.uri.query_strings.end(); ++iter)
		{
			result += iter->first;
			result += "=";
			result += iter->second;
		}
	}

	if(!msg.uri.fragment_id.empty())
	{
		result += "#";
		result += msg.uri.fragment_id;
	}

	result += " ";
	result += sstream.str();
	result += "\r\n";

	HTTPMsgHeaderContainer::const_iterator iter = msg.headers.begin();
	for(; iter != msg.headers.end(); iter++)
	{
		result += iter->first;
		result += ": ";
		result += iter->second;
		result += "\r\n";
	}

	result += "\r\n";
}

/**
 * @brief Function to transfer the HTTP request message to c++ standard string.
 *
 * @param[in] msg The HTTP request message.
 * @param[out] result The string contains all HTTP request message.
 */
void HttpMsgUtility::ComposeHttpRequestMsg(const HttpRequestMsg& msg, std::string& result)
{
	HttpMsgUtility::ComposeHttpRequestMsgExcludeBody(msg, result);
	result += msg.entity_body;
}
#endif

/**
 * @brief Function to transfer the initial line and headers of HTTP response message to c++ standard string.
 *
 * @param[in] msg The HTTP response message.
 * @param[out] result The string contains the initial line and headers of HTTP response message.
 */
void HttpMsgUtility::ComposeHttpResponseMsgExcludeBody(const HttpResponseMsg& msg, std::string& result)
{
	std::stringstream sstream;
	sstream << "HTTP/" << msg.http_version_major << "." << msg.http_version_minor << " " << msg.status_code;

	result = sstream.str();
	result += " ";
	result += msg.reason_phrase;
	result += "\r\n";

	HTTPMsgHeaderContainer::const_iterator iter = msg.headers.begin();
	for(; iter != msg.headers.end(); iter++)
	{
		result += iter->first;
		result += ": ";
		result += iter->second;
		result += "\r\n";
	}

	result += "\r\n";
}

/**
 * @brief Function to transfer the HTTP response message to c++ standard string.
 *
 * @param[in] msg The HTTP response message.
 * @param[out] result The string contains all HTTP response message.
 */
void HttpMsgUtility::ComposeHttpResponseMsg(const HttpResponseMsg& msg, std::string& result)
{
	HttpMsgUtility::ComposeHttpResponseMsgExcludeBody(msg, result);
	result += msg.entity_body;
}

/**
 * @brief Function to calculate the length of the entity body of HTTP message then set the Content-Length header into the HTTP message.
 *
 * @param[in,out] msg The HTTP message.
 */
void HttpMsgUtility::SetContentLength(HttpMsg& msg)
{
	std::stringstream sstream;
	sstream << msg.entity_body.length();
	msg.headers["Content-Length"] = sstream.str();
}

/**
 * @brief Function to set HTTP version and date into the HTTP response message.
 *
 * @param[out] response The HTTP response message.
 */
void HttpMsgUtility::SetGeneralHttpResponseHeaders(HttpResponseMsg& response)
{
	response.http_version_major = 1;
	response.http_version_minor = 1;

	std::time_t result = std::time(NULL);
	struct tm* timeinfo = NULL;
	char buffer[128] = {'\0'};
	timeinfo = std::gmtime(&result);

	strftime(buffer, 128, "%a, %d %b %Y %X GMT", timeinfo);

	response.headers["Date"] = buffer;
}

/**
 * @brief Function to set the default status code and reason phrase of the HTTP response message.
 *
 * @param[out] msg The HTTP response message.
 * @param[in] status_code The status code of HTTP response message.
 * @return true: If the status code can be found in the default list. Otherwise, it is false.
 */
bool HttpMsgUtility::SetDefaultStatusCode(HttpResponseMsg& msg, int status_code)
{
	msg.reason_phrase = HttpMsgUtility::GetReasonPhrase(status_code);
	msg.status_code = status_code;

	return (msg.reason_phrase.empty()) ? false : true;
}

/**
 * @brief Function to get the reason phrase of HTTP response message corresponds with the input status code.
 *
 * @param[in] status_code The status code of HTTP response message.
 * @return The reason phrase of HTTP response message.
 */
std::string HttpMsgUtility::GetReasonPhrase(int status_code)
{
	switch(status_code)
	{
	case 100:
		return "Continue";
	case 101:
		return "Switching Protocols";
	case 200:
		return "OK";
	case 201:
		return "Created";
	case 202:
		return "Accepted";
	case 203:
		return "Non-Authoritative Information";
	case 204:
		return "No Content";
	case 205:
		return "Reset Content";
	case 206:
		return "Partial Content";
	case 300:
		return "Multiple Choices";
	case 301:
		return "Moved Permanently";
	case 302:
		return "Found";
	case 303:
		return "See Other";
	case 304:
		return "Not Modified";
	case 305:
		return "Use Proxy";
	case 306:
		return "";
	case 307:
		return "Temporary Redirect";
	case 400:
		return "Bad Request";
	case 401:
		return "Unauthorized";
	case 402:
		return "Payment Required";
	case 403:
		return "Forbidden";
	case 404:
		return "Not Found";
	case 405:
		return "Method Not Allowed";
	case 406:
		return "Not Acceptable";
	case 407:
		return "Proxy Authentication Required";
	case 408:
		return "Request Timeout";
	case 409:
		return "Conflict";
	case 410:
		return "Gone";
	case 411:
		return "Length Required";
	case 412:
		return "Precondition Failed";
	case 413:
		return "Request Entity Too Large";
	case 414:
		return "Request-URI Too Long";
	case 415:
		return "Unsupported Media Type";
	case 416:
		return "Requested Range Not Satisfiable";
	case 417:
		return "Expectation Failed";
	case 500:
		return "Internal Server Error";
	case 501:
		return "Not Implemented";
	case 502:
		return "Bad Gateway";
	case 503:
		return "Service Unavailable";
	case 504:
		return "Gateway Timeout";
	case 505:
		return "HTTP Version Not Supported";
	default:
		return "";
	}
}

} // namespace vtcs
