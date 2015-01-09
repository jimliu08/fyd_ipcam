#include <fd-http-server/not_implement_request_handler.hpp>
#include <fd-http-server/http_msg.hpp>

namespace vtcs
{

/// Constructor of http request handler for the not implement case.
NotImplementRequestHandler::NotImplementRequestHandler()
{
}

NotImplementRequestHandler::~NotImplementRequestHandler()
{
}

/**
 * @brief Function to handle the partial http entity body (Set the "501 Not implemented" for the HTTP response message).
 *
 * @param[in] request_msg The HTTP request message.
 * @param[out] response_msg The HTTP response message.
 * @param[in] start_index The start index point to the unhandle body in the HTTP request message.
 * @param[in] size The size of unhandle body in the HTTP request message.
 * @param[in] body_type It is partial body, final partial body or complete body.
 */
void NotImplementRequestHandler::HandlePartialBody(const HttpRequestMsg& /*request_msg*/, HttpResponseMsg& response_msg, size_t /*start_index*/, size_t /*size*/, HTTPRequestStatus body_type)
{
	if(body_type == FinalBody)
	{
		// We do not implement other funtions now.
		// So we set "501 Not implemented"
		HttpMsgUtility::SetDefaultStatusCode(response_msg, 501);
		response_msg.entity_body.clear();
	}
}

}
