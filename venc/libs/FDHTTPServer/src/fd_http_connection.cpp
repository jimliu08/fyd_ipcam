#include <fd-http-server/fd_http_connection.hpp>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>

#define READ_TIMEOUT 2000
#define WRITE_TIMEOUT 8000

namespace vtcs
{

/// Callbacks for HTTP parser.
http_parser_settings FDHTTPConnection::parser_settings_ = {
	&FDHTTPConnection::on_message_begin_helper,
	&FDHTTPConnection::on_url_helper,
	NULL,
	&FDHTTPConnection::on_header_field_helper,
	&FDHTTPConnection::on_header_value_helper,
	&FDHTTPConnection::on_headers_complete_helper,
	&FDHTTPConnection::on_body_helper,
	&FDHTTPConnection::on_message_complete_helper,
};

/**
 * @brief Constructor of FD HTTP connection.
 *
 * @param[in] request_handler HTTP request handler.
 * @param[in] func The exit callback function.
 */
FDHTTPConnection::FDHTTPConnection(const std::shared_ptr<IHttpRequestHandler>& request_handler, const std::function<void(FDHTTPConnection*)>& func)
	: http_request_handler_(request_handler),
	exit_callback_(func),
	socket_fd_(-1),
	data_buf_(NULL),
	buf_size_(0),
	recv_len_(-1),
	is_running_(false),
	is_keep_alive_(true),
	current_http_header_state_(HTTPHead_Nothing)
{
}

/// Destructor of FD HTTP connection.
FDHTTPConnection::~FDHTTPConnection()
{
	this->Stop();
}

/**
 * @brief Function to assign the network socket and data buffer.
 *
 * @param[in] socket_fd The network socket.
 * @param[in] data_buf The data buffer.
 * @param[in] buf_size The total size of data buffer.
 * @param[in] recv_len The length of data in the data buffer.
 */
void FDHTTPConnection::Init(int socket_fd, char* data_buf, size_t buf_size, ssize_t recv_len)
{
	assert(!is_running_);

	assert(socket_fd > 0);
	assert(data_buf != NULL);

	socket_fd_ = socket_fd;
	data_buf_ = data_buf;
	buf_size_ = buf_size;
	recv_len_ = recv_len;
}

/// Function to reset some internal values.
void FDHTTPConnection::Reset()
{
	assert(!is_running_);

	socket_fd_ = -1;
	data_buf_ = NULL;
	buf_size_ = 0;
	recv_len_ = -1;
	is_running_ = false;
}

/// Function to initialize the HTTP request, response message and some information for the HTTP parser.
void FDHTTPConnection::init_http_info()
{
	http_request_msg_.Clear();
	http_response_msg_.Clear();
	http_request_msg_.private_user_data = static_cast<void*>(&user_data_);

	current_http_header_field_.clear();
	current_http_header_value_.clear();
	http_url_str_.clear();
	current_http_header_state_ = HTTPHead_Nothing;

}

/**
 * @brief Function to use HTTP parser to parse HTTP message.
 *
 * @return The next state of state machine.
 */
FDHTTPConnection::ConnectionState FDHTTPConnection::start_parse_data()
{
	size_t nparsed = 0;
	if(recv_len_ <= 0)
	{
		if(recv_len_ == 0)
			fprintf(stdout,"Connection is closed.\n");
		else
			fprintf(stderr,"Error: When recv the message from client.\n");

		return shutdown;
	}

	nparsed = http_parser_execute(&(this->parser_), &(FDHTTPConnection::parser_settings_), data_buf_, recv_len_);

	// Because we may change the state in the callback function of parser,
	// we check the state again here.
	if(con_state_ != parse_data)
	{
		if(con_state_ == shutdown)
			return shutdown;

		// FIXME: Handle other state.
	}

	if (parser_.upgrade)
	{
		/* handle new protocol */
		std::cout << "handle new protocol" << std::endl;
	}
	else if (nparsed != static_cast<size_t>(recv_len_))
	{
		/* Handle error. Usually just close the connection. */
		std::cout << "Handle error" << std::endl;
		std::cout << "Name = " << http_errno_name(HTTP_PARSER_ERRNO(&parser_)) << std::endl;
		std::cout << "Description = " << http_errno_description(HTTP_PARSER_ERRNO(&parser_)) << std::endl;

		return shutdown;
	}

	return receive_data;
}

/**
 * @brief Function to receive data from network.
 *
 * @return The next state of state machine.
 */
FDHTTPConnection::ConnectionState FDHTTPConnection::start_receive_data()
{
#ifdef DEBUG_HTTP_SERVER
	fprintf(stdout,"connection %p start_receive_data.\n", (void*)this);
#endif
	// Receive data from client.
	int ret = poll(con_read_fd_set_, 1, READ_TIMEOUT);
	if(ret == -1)
	{
		if(EINTR == errno)
			return receive_data;

		fprintf(stderr,"connection read poll error: %s\n", strerror(errno));
		return shutdown;
	}

	if(ret == 0)
	{
#ifdef DEBUG_HTTP_SERVER
		fprintf(stdout,"connection %p start_receive_data timeout.\n", (void*)this);
#endif
		// timeout
		return receive_data;
	}

	recv_len_ = -1;

	if(con_read_fd_set_[0].revents & POLLIN)
	{
		recv_len_ = recv(socket_fd_, data_buf_, buf_size_, 0);
		if(recv_len_ <= 0)
		{
			if(recv_len_ == 0)
				fprintf(stdout,"Connection is closed.\n");
			else
				fprintf(stderr,"Error: When recv the message from client. %s\n", strerror(errno));

			return shutdown;
		}
	}

	if(recv_len_ == -1)
	{
		fprintf(stderr,"%s, %s: Unexpect Errors.\n", __FILE__, __func__);
	}

	// If we have new data then we start to parse them.
	return parse_data;
}

/**
 * @brief Function to send data to client.
 *
 * @param[in] buf The data need to be sent.
 * @param[in, out] len The length of data. When the function is returned, the value will be changed to the bytes that be sent.
 * @return true: success, false: failed.
 */
bool FDHTTPConnection::start_send_data(const char* buf, size_t& len)
{
#ifdef DEBUG_HTTP_SERVER
	fprintf(stdout,"connection %p start_send_data.\n", (void*)this);
#endif
	// Send data to client.
	int ret = 0;
	size_t total = 0;        // how many bytes we've sent
	size_t bytesleft = len; // how many we have left to send
	ssize_t n = 0;
	bool result = true;

	while(total < len)
	{
		ret = poll(con_write_fd_set_, 1, WRITE_TIMEOUT);
		if(ret == -1)
		{
			if(EINTR == errno)
				continue;

			fprintf(stderr,"connection write poll error: %s\n", strerror(errno));
			result = false;
			break;
		}

		if(ret == 0)
		{
#ifdef DEBUG_HTTP_SERVER
			fprintf(stdout,"connection %p start_send_data timeout.\n", (void*)this);
#endif
			// timeout
			continue;
		}

		if(con_write_fd_set_[0].revents & POLLERR)
		{
			fprintf(stderr,"connection write error: POLLERR\n");
			result = false;
			break;
		}
		if(con_write_fd_set_[0].revents & POLLHUP)
		{
			fprintf(stderr,"connection write error: POLLHUP\n");
			result = false;
			break;
		}
		if(con_write_fd_set_[0].revents & POLLNVAL)
		{
			fprintf(stderr,"connection write error: POLLNVAL\n");
			result = false;
			break;
		}

		if(con_write_fd_set_[0].revents & POLLOUT)
		{
			n = send(socket_fd_, buf + total, bytesleft, 0);
			if (n == -1)
			{
				fprintf(stderr,"connection send error: %s\n", strerror(errno));
				result = false;
				break;
			}
			total += n;
			bytesleft -= n;
		}
	}

	len = total; // return number actually sent here
	return result;
}

/// The main loop (state machine) of HTTP connection.
void* FDHTTPConnection::Start()
{
	if(is_running_)
		return (void*)(-1);

#ifdef DEBUG_HTTP_SERVER
	fprintf(stdout,"connection %p start >>>>>>>>>>>>>>\n", (void*)this);
#endif

	is_running_ = true;
	this->prepare_user_data();
	this->init_http_info();
	http_parser_init(&parser_, HTTP_REQUEST);
	parser_.data = this;

	con_read_fd_set_[0].fd = socket_fd_;
	con_read_fd_set_[0].events = POLLIN;

	con_write_fd_set_[0].fd = socket_fd_;
	con_write_fd_set_[0].events = POLLOUT;

	con_state_ = parse_data;

	while(is_running_)
	{
		if(con_state_ == shutdown)
			break;

		switch(con_state_)
		{
			case parse_data:
				con_state_ = start_parse_data();
				break;
			case receive_data:
				con_state_ = start_receive_data();
				break;
			default:
				break;
		}
	}

	if(socket_fd_ > 0)
		close(socket_fd_);
	socket_fd_ = -1;

	is_running_ = false;

	if(exit_callback_)
		exit_callback_(this);

	return (void*)(0);
}

/**
 * @brief Function to stop the connection.
 *
 * @return true: success, false: failed.
 */
bool FDHTTPConnection::Stop()
{
	if(!is_running_)
		return false;

#ifdef DEBUG_HTTP_SERVER
	fprintf(stdout,"connection %p stop >>>>>>>>>>>>>>\n", (void*)this);
#endif
	is_running_ = false;

	return true;
}

int FDHTTPConnection::on_message_begin_helper(http_parser* parser)
{
	return ((FDHTTPConnection*)(parser->data))->on_message_begin(parser);
}

int FDHTTPConnection::on_url_helper(http_parser* parser, const char *at, size_t length)
{
	return ((FDHTTPConnection*)(parser->data))->on_url(parser, at, length);
}

int FDHTTPConnection::on_header_field_helper(http_parser* parser, const char *at, size_t length)
{
	return ((FDHTTPConnection*)(parser->data))->on_header_field(parser, at, length);
}

int FDHTTPConnection::on_header_value_helper(http_parser* parser, const char *at, size_t length)
{
	return ((FDHTTPConnection*)(parser->data))->on_header_value(parser, at, length);
}

int FDHTTPConnection::on_headers_complete_helper(http_parser* parser)
{
	return ((FDHTTPConnection*)(parser->data))->on_headers_complete(parser);
}

int FDHTTPConnection::on_body_helper(http_parser* parser, const char *at, size_t length)
{
	return ((FDHTTPConnection*)(parser->data))->on_body(parser, at, length);
}

int FDHTTPConnection::on_message_complete_helper(http_parser* parser)
{
	return ((FDHTTPConnection*)(parser->data))->on_message_complete(parser);
}

int FDHTTPConnection::on_message_begin(http_parser* /*parser*/)
{
#ifdef DEBUG_HTTP_SERVER
	std::cout << "on_message_begin" << std::endl;
#endif
	return 0;
}

int FDHTTPConnection::on_url(http_parser* /*parser*/, const char *at, size_t length)
{
	http_url_str_.append(at, length);
	return 0;
}

int FDHTTPConnection::on_header_field(http_parser* /*parser*/, const char *at, size_t length)
{
	switch(current_http_header_state_)
	{
		case HTTPHead_Value:
			http_request_msg_.headers[current_http_header_field_] = current_http_header_value_;
		case HTTPHead_Nothing:
			current_http_header_field_.assign(at, length);
			current_http_header_state_ = HTTPHead_Field;
			break;

		case HTTPHead_Field:
			current_http_header_field_.append(at, length);
			break;
		default:
			fprintf(stderr,"%s, %s: Unknown HTTP header state.\n", __FILE__, __func__);
			return -1;
	}

	return 0;
}

int FDHTTPConnection::on_header_value(http_parser* /*parser*/, const char *at, size_t length)
{
	switch(current_http_header_state_)
	{
		case HTTPHead_Value:
			current_http_header_value_.append(at, length);
			break;
		case HTTPHead_Field:
			current_http_header_value_.assign(at, length);
			current_http_header_state_ = HTTPHead_Value;
			break;
		default:
			fprintf(stderr,"%s, %s: Unknown HTTP header state.\n", __FILE__, __func__);
			return -1;
	}

	return 0;
}

int FDHTTPConnection::on_headers_complete(http_parser* parser)
{
	// Copy all headers and HTTP request information.
#ifdef DEBUG_HTTP_SERVER
	std::cout << "on_header_complete" << std::endl;
#endif
	switch(current_http_header_state_)
	{
		case HTTPHead_Value:
			http_request_msg_.headers[current_http_header_field_] = current_http_header_value_;
			break;
		default:
			break;
	}

	http_request_msg_.http_version_major = parser->http_major;
	http_request_msg_.http_version_minor = parser->http_minor;
	http_request_msg_.method = http_method_str((http_method)(parser->method));

	parse_url(http_url_str_, http_request_msg_.uri);

#ifdef DEBUG_HTTP_SERVER
	URIUtility::PrintURI(http_request_msg_.uri);
#endif

	// Handle Expect: 100-continue
	// We need to send response first then receive the body.
	HTTPMsgHeaderContainer::iterator iter = http_request_msg_.headers.find("Expect");
	if( (iter != http_request_msg_.headers.end()) && (iter->second == "100-continue") && http_request_handler_)
	{
		http_response_msg_.Clear();
		HttpMsgUtility::SetDefaultStatusCode(http_response_msg_, 100);
		http_request_handler_->HandlePartialBody(http_request_msg_, http_response_msg_, 0, 0, IHttpRequestHandler::Continue100);
		HttpMsgUtility::SetGeneralHttpResponseHeaders(http_response_msg_);
		HttpMsgUtility::SetContentLength(http_response_msg_);

		std::string temp_buf;
		HttpMsgUtility::ComposeHttpResponseMsg(http_response_msg_, temp_buf);

		size_t send_len = temp_buf.length();
		if(!start_send_data(temp_buf.c_str(), send_len))
		{
			fprintf(stderr,"%s, %s: Send 100 continue response error.\n", __FILE__, __func__);
			http_parser_pause(parser, 1);
			con_state_ = shutdown;
			return 0;
		}

		http_response_msg_.Clear();
	}

	return 0;
}

int FDHTTPConnection::on_body(http_parser* parser, const char *at, size_t length)
{
	size_t start_index = http_request_msg_.entity_body.length();
	http_request_msg_.entity_body.append(at, length);
	IHttpRequestHandler::HTTPRequestStatus request_status = http_body_is_final(parser) ? IHttpRequestHandler::FinalBody : IHttpRequestHandler::NotFinalBody;

	// Call the request handler.
	if(http_request_handler_)
	{
		http_request_handler_->HandlePartialBody(http_request_msg_, http_response_msg_, start_index, length, request_status);
	}

	return 0;
}

int FDHTTPConnection::on_message_complete(http_parser* parser)
{
#ifdef DEBUG_HTTP_SERVER
	std::cout << "on_message_complete" << std::endl;

	this->print_http_request_msg(http_request_msg_);
#endif

	if(http_request_handler_)
	{
		http_request_handler_->HandlePartialBody(http_request_msg_, http_response_msg_, 0, http_request_msg_.entity_body.length(), IHttpRequestHandler::CompleteBody);
	}

	// Handle keep-alive
	is_keep_alive_ = (!http_should_keep_alive(parser) || !is_running_) ? false : true;
	if(!is_keep_alive_)
	{
		http_response_msg_.headers["Connection"] = "close";
	}

	// If it is chunked data, we do not set the content length.
	HttpMsgUtility::SetGeneralHttpResponseHeaders(http_response_msg_);
	HTTPMsgHeaderContainer::const_iterator header_iter = http_response_msg_.headers.find("Transfer-Encoding");
	if( (header_iter == http_response_msg_.headers.end()) || ( (header_iter->second) != "chunked") )
	{
		HttpMsgUtility::SetContentLength(http_response_msg_);
	}

	// If the Connection header is set to close then we shutdown after sending the response.
	header_iter = http_response_msg_.headers.find("Connection");
	if( (header_iter != http_response_msg_.headers.end()) && ( (header_iter->second) == "close") )
	{
		con_state_ = shutdown;
		http_parser_pause(parser, 1);
	}

#ifdef DEBUG_HTTP_SERVER
	this->print_http_response_msg(http_response_msg_);
#endif

	// Send status line and headers of HTTP response message.
	std::string temp_buf;
	HttpMsgUtility::ComposeHttpResponseMsgExcludeBody(http_response_msg_, temp_buf);

	size_t send_len = temp_buf.length();
	if(!start_send_data(temp_buf.c_str(), send_len))
	{
		fprintf(stderr,"%s, %s: Send the status line and headers of HTTP response error.\n", __FILE__, __func__);
		http_parser_pause(parser, 1);
		con_state_ = shutdown;
		return 0;
	}

	// Send the body of HTTP response message.
	send_len = http_response_msg_.entity_body.length();
	if(!start_send_data(http_response_msg_.entity_body.c_str(), send_len))
	{
		fprintf(stderr,"%s, %s: Send the body of HTTP response error.\n", __FILE__, __func__);
		http_parser_pause(parser, 1);
		con_state_ = shutdown;
		return 0;
	}

	// Initialize the HTTP parser.
	this->init_http_info();

	return 0;
}

/// Function to prepare the information which is passed to user function.
void FDHTTPConnection::prepare_user_data()
{
	user_data_.socket_fd_ptr = &socket_fd_;
}

/**
 * @brief Function to parse URL.
 *
 * @param[in] url The input URL.
 * @param[in] result_url The parsed URI structure.
 * @return true: success, false: failed.
 */
bool FDHTTPConnection::parse_url(const std::string& url, URI& result_url)
{
#ifdef USE_UNESCAPE
	std::string temp_str;
	if(!URIUtility::Unescape(url, temp_str))
	{
		fprintf(stderr,"%s, %s: Unescape URL error: %s.\n", __FILE__, __func__, url.c_str());
		return false;
	}
#else
	const std::string& temp_str = url;
#endif

	struct http_parser_url parsed_url;
	int ret = http_parser_parse_url(temp_str.c_str(), temp_str.length(), 0, &parsed_url);
	if(ret != 0)
	{
		fprintf(stderr,"%s, %s: Parse URL error: %s.\n", __FILE__, __func__, url.c_str());
		return false;
	}

	if((parsed_url.field_set & (1 << UF_SCHEMA)) != 0)
	{
		result_url.scheme = temp_str.substr(parsed_url.field_data[UF_SCHEMA].off, parsed_url.field_data[UF_SCHEMA].len);
	}

	if((parsed_url.field_set & (1 << UF_HOST)) != 0)
	{
		result_url.hostname = temp_str.substr(parsed_url.field_data[UF_HOST].off, parsed_url.field_data[UF_HOST].len);
	}

	if((parsed_url.field_set & (1 << UF_PORT)) != 0)
	{
		result_url.port = temp_str.substr(parsed_url.field_data[UF_PORT].off, parsed_url.field_data[UF_PORT].len);
	}

	if((parsed_url.field_set & (1 << UF_PATH)) != 0)
	{
		result_url.path = temp_str.substr(parsed_url.field_data[UF_PATH].off, parsed_url.field_data[UF_PATH].len);
	}

	if((parsed_url.field_set & (1 << UF_FRAGMENT)) != 0)
	{
		result_url.fragment_id = temp_str.substr(parsed_url.field_data[UF_FRAGMENT].off, parsed_url.field_data[UF_FRAGMENT].len);
	}

	if((parsed_url.field_set & (1 << UF_USERINFO)) != 0)
	{
		result_url.userinfo = temp_str.substr(parsed_url.field_data[UF_USERINFO].off, parsed_url.field_data[UF_USERINFO].len);
	}

	if((parsed_url.field_set & (1 << UF_QUERY)) != 0)
	{
		if (!URIUtility::ParseQueryString(temp_str.substr(parsed_url.field_data[UF_QUERY].off, 
					parsed_url.field_data[UF_QUERY].len), 
				result_url.query_strings))
			return false;
	}

	return true;
}

#ifdef DEBUG_HTTP_SERVER
void FDHTTPConnection::print_http_request_msg(const HttpRequestMsg& request_msg)
{
        printf("\n\n*************Request Message*************\n");
        printf("\nRequest Line ====>\n");
        printf("Method = %s\\n\n", request_msg.method.c_str());
        URIUtility::PrintURI(request_msg.uri);
        printf("version = %d.%d\\n\n",request_msg.http_version_major, request_msg.http_version_minor);

        HTTPMsgHeaderContainer::const_iterator iter;
        printf("\nHeaders =======>\n");
        for (iter = request_msg.headers.begin(); iter != request_msg.headers.end(); iter++)
                printf("%s:%s\\n\n", (*iter).first.c_str(), (*iter).second.c_str());

        printf("\nBody =======> Real length of decompressed body = %u\n", request_msg.entity_body.length());
        for(size_t i = 0; i < request_msg.entity_body.length(); ++i)
                printf("%c",request_msg.entity_body[i]);
        printf("\n=====================\n");
}

void FDHTTPConnection::print_http_response_msg(const HttpResponseMsg& response_msg)
{
        printf("\n\n*************Response Message*************\n");
        printf("\nStatus Line ====>\n");
        printf("version = %d.%d\\n\n",response_msg.http_version_major, response_msg.http_version_minor);
        printf("Status Code = %d\\n\n", response_msg.status_code);
        printf("Reason phrase = %s\\n\n", response_msg.reason_phrase.c_str());

        HTTPMsgHeaderContainer::const_iterator iter;
        printf("\nHeaders =======>\n");
        for (iter = response_msg.headers.begin(); iter != response_msg.headers.end(); iter++)
                printf("%s:%s\\n\n", (*iter).first.c_str(), (*iter).second.c_str());

        printf("\nBody =======>\n");
        for(size_t i = 0; i < response_msg.entity_body.length(); ++i)
                printf("%c",response_msg.entity_body[i]);
        printf("\n=====================\n");
}
#endif

} // namespace vtcs
