#ifndef _NOT_IMPLEMENT_REQUEST_HANDLER_HPP_
#define _NOT_IMPLEMENT_REQUEST_HANDLER_HPP_

#include <fd-http-server/http_request_handler_if.hpp>

namespace vtcs
{

/// Not implement request handler for the HTTP server.
class NotImplementRequestHandler : public IHttpRequestHandler
{
public:
	NotImplementRequestHandler();
	~NotImplementRequestHandler();

	void HandlePartialBody(const HttpRequestMsg&, HttpResponseMsg&, size_t, size_t, HTTPRequestStatus);
};

}

#endif

