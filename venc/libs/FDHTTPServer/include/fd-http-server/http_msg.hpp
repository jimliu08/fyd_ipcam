#ifndef _HTTP_MSG_HPP_
#define _HTTP_MSG_HPP_

#include <string>
#include <unordered_map>
#include <fd-http-server/uri.hpp>

namespace vtcs
{

typedef std::unordered_map<std::string, std::string> HTTPMsgHeaderContainer;

/// Class for partial HTTP message
class HttpMsg
{
public:
	HttpMsg() { http_version_major = http_version_minor = 0; private_user_data = NULL; }
	virtual ~HttpMsg() {}
	/// Function to clear the partial (common) HTTP message.
	virtual void Clear()
	{
		partial_msg_reset();
	}
protected:
	void partial_msg_reset()
	{
		http_version_major = http_version_minor = 0;
		headers.clear();
		entity_body.clear();
		private_user_data = NULL;
	}
public:
	unsigned short http_version_major;
	unsigned short http_version_minor;
	std::string entity_body;
	HTTPMsgHeaderContainer headers;
	/// Pointer to user data for private use.
	void* private_user_data;
};

/// Class for HTTP request message
class HttpRequestMsg : public HttpMsg
{
public:
	/// Function to clear the HTTP request message.
	virtual void Clear()
	{
		partial_msg_reset();
		partial_request_line_reset();
	}
protected:
	void partial_request_line_reset()
	{
		method.clear();
		uri.Clear();
	}
public:
	std::string method;
	URI uri;
};

/// Class for HTTP response message
class HttpResponseMsg : public HttpMsg
{
public:
	HttpResponseMsg() { status_code = 0; }
	/// Function to clear the HTTP response message.
	virtual void Clear()
	{
		partial_msg_reset();
		partial_status_line_reset();
	}
protected:
	void partial_status_line_reset()
	{
		status_code = 0;
		reason_phrase.clear();
	}
public:
	int status_code;
	std::string reason_phrase;
};

/// Utility class for HTTP message
class HttpMsgUtility
{
public:
#if 0
	static void ComposeHttpRequestMsgExcludeBody(const HttpRequestMsg&, std::string&);
	static void ComposeHttpRequestMsg(const HttpRequestMsg&, std::string&);
#endif
	static void ComposeHttpResponseMsgExcludeBody(const HttpResponseMsg&, std::string&);
	static void ComposeHttpResponseMsg(const HttpResponseMsg&, std::string&);

	static void SetContentLength(HttpMsg&);
	static void SetGeneralHttpResponseHeaders(HttpResponseMsg&);
	static bool SetDefaultStatusCode(HttpResponseMsg&, int);
	static std::string GetReasonPhrase(int);
};

} // namespace vtcs

#endif
