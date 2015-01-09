#ifndef _FD_HTTP_SERVER_HPP_
#define _FD_HTTP_SERVER_HPP_

#include <fd-http-server/http_request_handler_if.hpp>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <functional>
#include <deque>
#include <memory>
#include <pthread.h>

#include <poll.h>

namespace vtcs
{
// Forward declaration.
class FDHTTPConnection;

class FDHTTPServer
{
public:
	typedef FDHTTPConnection* FDHTTPConnectionPtr;
	typedef std::unordered_map<FDHTTPConnectionPtr, char*> ConnectionBufMap;

	FDHTTPServer(const std::shared_ptr<IHttpRequestHandler>&, const char*, size_t);
	~FDHTTPServer();
	bool Start(bool);
	bool Stop();

	//void RegisterErrorCallback();

private:
	// ==== Functions. ====
	static void* listener_loop_helper(void* context);
	static void* start_connection_helper(void* context);
	void* listener_loop();
	int get_fd(int unix_domain_sock, int *fd, void *data, size_t data_len);
	void connection_exit_callback(FDHTTPConnectionPtr);
	char* allocate_buf();

	// ==== Variables. ====
	/// HTTP request handler.
	std::shared_ptr<IHttpRequestHandler> http_request_handler_;
	/// Unix domain socket for Linux file descriptor transfer.
	int unix_domain_socket_fd_;
	/// The path of Unix domain socket.
	std::string unix_domain_socket_path_;

	/// Flag to indicate whether HTTP server is started or not.
	bool is_started_;
	/// Flag to indicate whether the main loop is running or not.
	bool is_running_;
	/// Flag to indicate whether using another thread to run main loop.
	bool is_use_listener_thread_;
	/// The size of data buffer.
	size_t max_buffer_size_;

	/// Listener thread.
	pthread_t listener_thread_;

	/// For poll function to listen in the Unix domain socket.
	struct pollfd listener_fd_set_[1];

	/// Mutex for map of connections.
	pthread_mutex_t map_mutex_;
	/// The container of connections and data buffers.
	ConnectionBufMap con_buf_map_;
};

} // namespace vtcs

#endif

