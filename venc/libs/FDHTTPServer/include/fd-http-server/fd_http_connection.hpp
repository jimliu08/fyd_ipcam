#ifndef _FD_HTTP_CONNECTION_HPP_
#define _FD_HTTP_CONNECTION_HPP_

#include <http-parser/http_parser.h>
#include <fd-http-server/http_request_handler_if.hpp>
#include <fd-http-server/http_msg.hpp>
#include <functional>
#include <sys/types.h>
#include <unordered_map>
#include <string>
#include <poll.h>
#include <fd-http-server/http_user_data.hpp>

namespace vtcs
{

class FDHTTPConnection
{
public:
	FDHTTPConnection(const std::shared_ptr<IHttpRequestHandler>&, const std::function<void(FDHTTPConnection*)>&);
	~FDHTTPConnection();

	void Init(int, char*, size_t, ssize_t);
	void Reset();

	void* Start();
	bool Stop();

private:
	enum HTTPHeaderState { HTTPHead_Nothing = 0, HTTPHead_Value, HTTPHead_Field };

	enum ConnectionState
	{
		receive_data,
		parse_data,
		shutdown
	};

	// ==== Functions. ====
	static int on_message_begin_helper(http_parser* parser);
	static int on_url_helper(http_parser* parser, const char *at, size_t length);
	static int on_header_field_helper(http_parser* parser, const char *at, size_t length);
	static int on_header_value_helper(http_parser* parser, const char *at, size_t length);
	static int on_headers_complete_helper(http_parser* parser);
	static int on_body_helper(http_parser* parser, const char *at, size_t length);
	static int on_message_complete_helper(http_parser* parser);

	int on_message_begin(http_parser* parser);
	int on_url(http_parser* parser, const char *at, size_t length);
	int on_header_field(http_parser* parser, const char *at, size_t length);
	int on_header_value(http_parser* parser, const char *at, size_t length);
	int on_headers_complete(http_parser* parser);
	int on_body(http_parser* parser, const char *at, size_t length);
	int on_message_complete(http_parser* parser);

	void init_http_info();
	ConnectionState start_parse_data();
	ConnectionState start_receive_data();
	bool start_send_data(const char* buf, size_t& len);

	void prepare_user_data();

	static bool parse_url(const std::string&, URI&);

#ifdef DEBUG_HTTP_SERVER
	static void print_http_request_msg(const HttpRequestMsg&);
	static void print_http_response_msg(const HttpResponseMsg&);
#endif

	// ==== Variable. ====

	/// HTTP request handler.
	std::shared_ptr<IHttpRequestHandler> http_request_handler_;
	/// Exit callback function.
	std::function<void(FDHTTPConnection*)> exit_callback_;
	/// Network socket.
	int socket_fd_;
	/// Data buffer.
	char* data_buf_;
	/// Size of data buffer.
	size_t buf_size_;
	/// The length of received data.
	ssize_t recv_len_;
	/// Main loop is running.
	bool is_running_;
	/// HTTP keep alive.
	bool is_keep_alive_;

	/// HTTP parser.
	http_parser parser_;
	/// Callback functions for HTTP parser.
	static http_parser_settings parser_settings_;

	/// Temporal string to store one field of HTTP header.
	std::string current_http_header_field_;
	/// Temporal string to store one value of HTTP header.
	std::string current_http_header_value_;
	/// State for parsing HTTP headers.
	HTTPHeaderState current_http_header_state_;

	/// HTTP URL string.
	std::string http_url_str_;
	/// HTTP request message.
	HttpRequestMsg http_request_msg_;
	/// HTTP response message.
	HttpResponseMsg http_response_msg_;

	/// For poll function to read data from network.
	struct pollfd con_read_fd_set_[1];
	/// For poll function to send data to client.
	struct pollfd con_write_fd_set_[1];

	/// State of HTTP connection.
	ConnectionState con_state_;

        /// The information to pass to user function.
        HttpUserData user_data_;

};

} // namespace VN

#endif


