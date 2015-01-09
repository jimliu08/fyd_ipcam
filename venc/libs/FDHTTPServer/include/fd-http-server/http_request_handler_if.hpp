#ifndef _HTTP_REQUEST_HANDLER_IF_HPP_
#define _HTTP_REQUEST_HANDLER_IF_HPP_

#include <fd-http-server/http_msg.hpp>
#include <memory>

namespace vtcs
{

/// Abstract interfaces for the HTTP request handler.
class IHttpRequestHandler
{
public:
	enum HTTPRequestStatus { NotFinalBody, FinalBody, CompleteBody, Continue100 };

	virtual ~IHttpRequestHandler(){}

	/**
	 * @brief Interface for request handler to handle the 
	 * partial body (chunked etc.).
	 *
	 * @param[in] request HTTP request message.
	 * @param[out] response HTTP response message.
	 * @param[in] start_index The index point to the start index 
	 * of the unhandled body in the HTTP request message.
	 * @param[in] size The size of the unhandled body in the HTTP request message.
	 * @param[in] body_type It is partial body, final partial body or complete body.
	 */
	virtual void HandlePartialBody(const HttpRequestMsg& request,
			HttpResponseMsg& response,
			size_t start_index,
			size_t size,
			HTTPRequestStatus body_type) = 0;
};


/// Abstract interfaces for the HTTP request dispatcher.
class IHttpRequestDispatcher : public IHttpRequestHandler
{
public:
	virtual ~IHttpRequestDispatcher(){}

	/**
	 * @brief Interface for register the request handler for each HTTP message.
	 *
	 * @param[in] method The method of HTTP request message that the register handler want to handler.
	 * @param[in] url The URL of HTTP request message that the register handler want to handler.
	 * @param[in] request_handler The register request handler.
	 */
	virtual void RegisterHttpRequestHandler(const std::string& method, const std::string& url, const std::shared_ptr<IHttpRequestHandler>& request_handler) = 0;

	/**
	 * @brief Interface for unregister the request handler for each HTTP message.
	 *
	 * @param[in] method The method of HTTP request message that the register handler want to handler.
	 * @param[in] url The URL of HTTP request message that the register handler want to handler.
	 */
	virtual void UnRegisterHttpRequestHandler(const std::string& method, const std::string& url) = 0;
};

} // namespace vtcs

#endif

