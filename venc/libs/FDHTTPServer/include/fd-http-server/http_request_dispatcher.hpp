#ifndef _HTTP_REQUEST_DISPATCHER_HPP_
#define _HTTP_REQUEST_DISPATCHER_HPP_

#include <fd-http-server/http_request_handler_if.hpp>
#include <fd-http-server/not_implement_request_handler.hpp>
#include <unordered_map>

namespace vtcs
{

/// HTTP request dispatcher for the HTTP server.
class HttpRequestDispatcher : public IHttpRequestDispatcher
{
public:
	typedef std::unordered_map< std::string, std::shared_ptr<IHttpRequestHandler> > URLHandlerContainer;
	typedef std::unordered_map< std::string, URLHandlerContainer > RequestHandlerContainer;
public:
	HttpRequestDispatcher();
	~HttpRequestDispatcher();

	void HandlePartialBody(const HttpRequestMsg&, HttpResponseMsg&, size_t, size_t, HTTPRequestStatus);
	void RegisterHttpRequestHandler(const std::string& method, const std::string& url, const std::shared_ptr<IHttpRequestHandler>& request_handler);
	void UnRegisterHttpRequestHandler(const std::string& method, const std::string& url);

private:
	/// Use method and URI to map the HTTP request handler.
	RequestHandlerContainer request_handler_map_;
	/// Default HTTP request handler for not implement.
	NotImplementRequestHandler not_implement_request_handler_;
};

} // namespace vtcs

#endif

