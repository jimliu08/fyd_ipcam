#include <fd-http-server/http_request_dispatcher.hpp>
#include <fd-http-server/http_msg.hpp>
#include <iostream>

namespace vtcs
{

/// Constructor of http request dispatcher.
HttpRequestDispatcher::HttpRequestDispatcher()
{
}

/// Destructor of http request dispatcher.
HttpRequestDispatcher::~HttpRequestDispatcher()
{
	request_handler_map_.clear();
}

/**
 * @brief Function to dispatcher messages to each request handler.
 *
 * @param[in] request_msg The HTTP request message.
 * @param[out] response_msg The HTTP response message.
 * @param[int] start_index The start index point to the unhandle body in the HTTP request message.
 * @param[in] size The size of unhandle body in the HTTP request message.
 * @param[in] body_type It is partial body, final partial body or complete body.
 */
void HttpRequestDispatcher::HandlePartialBody(const HttpRequestMsg& request_msg, HttpResponseMsg& response_msg, size_t start_index, size_t size, HTTPRequestStatus body_type)
{
	// Dispatch HTTP request message.
	URLHandlerContainer::iterator iter = request_handler_map_[request_msg.method].find(request_msg.uri.path);
	if( (iter != request_handler_map_[request_msg.method].end()) && (iter->second) )
	{
		iter->second->HandlePartialBody(request_msg, response_msg, start_index, size, body_type);
		return;
	}
	std::cout << "HttpRequestDispatcher::HandlePartialBody => Use not implement request handler. method = " << request_msg.method << std::endl;

	URIUtility::PrintURI(request_msg.uri);
	not_implement_request_handler_.HandlePartialBody(request_msg, response_msg, start_index, size, body_type);
}

/**
 * @brief Interface for register the request handler for each HTTP message.
 *
 * @param[in] method The method of HTTP request message that the register handler want to handler.
 * @param[in] url The URL of HTTP request message that the register handler want to handler.
 * @param[in] request_handler The register request handler.
 */
void HttpRequestDispatcher::RegisterHttpRequestHandler(const std::string& method, const std::string& url, const std::shared_ptr<IHttpRequestHandler>& request_handler)
{
	URLHandlerContainer::iterator iter = request_handler_map_[method].find(url);

	if(iter != request_handler_map_[method].end())
	{
		std::cout << "HttpRequestDispatcher::RegisterHttpRequestHandler => method = " << method << ", url = " << url << std::endl;

		throw "There is one request handler has already registered.";
	}

	request_handler_map_[method][url] = request_handler;
}

/**
 * @brief Interface for unregister the request handler for each HTTP message.
 *
 * @param[in] method The method of HTTP request message that the register handler want to handler.
 * @param[in] url The URL of HTTP request message that the register handler want to handler.
 */
void HttpRequestDispatcher::UnRegisterHttpRequestHandler(const std::string& method, const std::string& url)
{
	URLHandlerContainer::iterator iter = request_handler_map_[method].find(url);

	if(iter != request_handler_map_[method].end())
	{
		request_handler_map_[method].erase(iter);
	}
}

}
