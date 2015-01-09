#include <fd-http-server/fd_http_server.hpp>
#include <fd-http-server/fd_http_connection.hpp>

#include <cstring>
#include <cstdio>
#include <cerrno>
#include <cassert>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

#define LISTENER_TIMEOUT 2000

namespace vtcs
{

/**
 * @brief Constructor for FD HTTP server.
 *
 * @param[in] request_handler The HTTP request hander.
 * @param[in] socket_path The Unix domain socket path for Linux file descriptor transfer.
 * @param[in] max_buffer_size The max size of buffer to get the data. It need to get first full request.
 */
FDHTTPServer::FDHTTPServer(const std::shared_ptr<IHttpRequestHandler>& request_handler, const char* socket_path, size_t max_buffer_size)
	: http_request_handler_(request_handler),
	unix_domain_socket_fd_(-1),
	unix_domain_socket_path_(socket_path),
	is_started_(false),
	is_running_(false),
	is_use_listener_thread_(false),
	max_buffer_size_(max_buffer_size)
{
	pthread_mutex_init(&map_mutex_, NULL);
}

/// Destructor for FD HTTP server.
FDHTTPServer::~FDHTTPServer()
{
	this->Stop();
	pthread_mutex_destroy(&map_mutex_);
}

/**
 * @brief Function to start FD HTTP server.
 *
 * @param[in] is_use_thread true: Use another thread to run the main loop. Otherwise, it uses caller thread.
 *
 * @return true: success, false: failed
 */
bool FDHTTPServer::Start(bool is_use_thread)
{
	int rc = 0;
	struct sockaddr_un addr;
	pthread_attr_t attr;

	if(is_started_)
		return false;

	// Prepare Unix domain socket.
	unix_domain_socket_fd_ = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(unix_domain_socket_fd_ == -1)
	{
		fprintf(stderr,"Error creating unix domain socket: %s\n", strerror(errno));
		return false;
	}

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, unix_domain_socket_path_.c_str());

	unlink(unix_domain_socket_path_.c_str());

	if(bind(unix_domain_socket_fd_, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		fprintf(stderr,"Error binding unix domain socket: %s\n", strerror(errno));
		goto server_start_fail;
	}

	if(chmod(unix_domain_socket_path_.c_str(), S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) == -1)
	{
		fprintf(stderr,"Error chmod unix domain socket: %s\n", strerror(errno));
		goto server_start_fail;
	}

	// Because this is datagram socket, we don't need to call listen and accept.

	// Avoid the child process references the file descriptor and do not close it.
	fcntl(unix_domain_socket_fd_, F_SETFD, FD_CLOEXEC | fcntl(unix_domain_socket_fd_, F_GETFD));

	is_use_listener_thread_ = is_use_thread;

	// Prepare to run the main loop.
	if(is_use_listener_thread_)
	{
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		rc = pthread_create(&listener_thread_, &attr, &FDHTTPServer::listener_loop_helper, this);
		if(rc)
		{
			fprintf(stderr,"ERROR: return code from pthread_create() is %d\n", rc);
			pthread_attr_destroy(&attr);
			goto server_start_fail;
		}
		pthread_attr_destroy(&attr);
		is_started_ = true;
	}
	else
	{
		// We need to set this flag before enter the loop if we use main thread to run the loop.
		// Otherwise, the Stop function will always return false.
		is_started_ = true;
		this->listener_loop();
	}

	return true;

server_start_fail:
	if(unix_domain_socket_fd_ > 0)
		close(unix_domain_socket_fd_);
	return false;
}

/**
 * @brief Function to stop FD HTTP server.
 *
 * @return true: success, false: failed
 */
bool FDHTTPServer::Stop()
{
	if(!is_started_)
		return false;

	void* ret_val;

	is_running_ = false;

	// Close Unix domain socket.
	listener_fd_set_[0].fd = -1;
	int temp_fd = unix_domain_socket_fd_;
	unix_domain_socket_fd_ = -1;
	if(temp_fd > 0)
		close(temp_fd);

	// Stop all connections.
	pthread_mutex_lock(&map_mutex_);
	ConnectionBufMap::iterator iter = con_buf_map_.begin();
	for(; iter != con_buf_map_.end(); ++iter)
	{
		iter->first->Stop();
	}
	pthread_mutex_unlock(&map_mutex_);

	// Wait all connections.
	while(!con_buf_map_.empty())
	{
		sleep(1);
	}

	if(is_use_listener_thread_)
	{
		pthread_join(listener_thread_, &ret_val);
	}

	sleep(1);

	is_started_ = false;
	return true;
}

/**
 * @brief Function to allocate data buffer.
 *
 * @return data buffer.
 */
char* FDHTTPServer::allocate_buf()
{
	char* temp_buf = new char [max_buffer_size_];
	return temp_buf;
}

/// Helper for pthread to run listener loop.
void* FDHTTPServer::listener_loop_helper(void* context)
{
	return ((FDHTTPServer*)context)->listener_loop();
}

/// Helper for pthread to start the connection.
void* FDHTTPServer::start_connection_helper(void* context)
{
	return ((FDHTTPConnection*)context)->Start();
}

/// Listener loop.
void* FDHTTPServer::listener_loop()
{
	int ret = 0;
	char* current_data_buf = NULL;
	int connection_socket = -1;
	int recv_ret = -1;
	FDHTTPConnectionPtr current_connection = NULL;
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 5;

	pthread_attr_t attr;
	pthread_t connection_thread;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	is_running_ = true;

	listener_fd_set_[0].fd = unix_domain_socket_fd_;
	listener_fd_set_[0].events = POLLIN;

	while(is_running_)
	{
		ret = poll(listener_fd_set_, 1, LISTENER_TIMEOUT);

#ifdef DEBUG_HTTP_SERVER
		fprintf(stdout,"listener poll ...............\n");
#endif
		if(ret == -1)
		{
			if(EINTR == errno)
				continue;

			fprintf(stderr,"listener poll error: %s\n", strerror(errno));
			break;
		}

		if(ret == 0)
		{
			// timeout
#ifdef DEBUG_HTTP_SERVER
			fprintf(stdout,"listen timeout.\n");
#endif
			continue;
		}

		if(listener_fd_set_[0].revents & POLLIN)
		{
			if(current_data_buf == NULL)
			{
				current_data_buf = allocate_buf();
			}

			memset(current_data_buf, 0, max_buffer_size_);

			// Receive the network socket and data from Unix domain socket.
			recv_ret = get_fd(unix_domain_socket_fd_, &connection_socket, (void*)current_data_buf, max_buffer_size_-1);
			if(recv_ret <= 0)
			{
				break;
			}

			// Avoid the child process references the file descriptor and do not close it.
			fcntl(connection_socket, F_SETFD, FD_CLOEXEC | fcntl(connection_socket, F_GETFD));

			// Set linger
			ret = setsockopt(connection_socket, SOL_SOCKET, SO_LINGER, (void*) &so_linger, sizeof(so_linger));
			if(ret == -1)
			{
				fprintf(stderr,"setsockopt (linger) error: %s\n", strerror(errno));
			}

			if(connection_socket <= 0)
			{
				continue;
			}

			current_data_buf[recv_ret] = '\0';

			assert(current_connection == NULL);

			// Create connection.
			current_connection = new FDHTTPConnection(http_request_handler_, std::bind(&FDHTTPServer::connection_exit_callback, this, std::placeholders::_1));
			current_connection->Init(connection_socket, current_data_buf, max_buffer_size_, recv_ret);

			if(!is_running_)
			{
				close(connection_socket);
				connection_socket = -1;
				break;
			}

			// Use map to record the connection and data buffer.
			pthread_mutex_lock(&map_mutex_);
			assert(con_buf_map_.find(current_connection) == con_buf_map_.end());
			con_buf_map_[current_connection] = current_data_buf;
			pthread_mutex_unlock(&map_mutex_);

			// Start connection.
			int rc = pthread_create(&connection_thread, &attr, &FDHTTPServer::start_connection_helper, current_connection);
			if(rc)
			{
				fprintf(stderr,"ERROR: return code from pthread_create() for connection is %d\n", rc);
				pthread_mutex_lock(&map_mutex_);
				con_buf_map_.erase(current_connection);
				pthread_mutex_unlock(&map_mutex_);

				delete current_connection;
				current_connection = NULL;

				close(connection_socket);
				connection_socket = -1;
			}

			current_data_buf = NULL;
			current_connection = NULL;
			connection_socket = -1;
		}
	}

	// Reclaim current_data_buf, current_connection etc
	if(current_connection)
	{
		current_connection->Stop();
		delete current_connection;
		current_connection = NULL;
	}

	if(current_data_buf)
	{
		delete current_data_buf;
		current_data_buf = NULL;
	}

	if(connection_socket > 0)
	{
		close(connection_socket);
	}
	connection_socket = -1;

	pthread_attr_destroy(&attr);
	is_running_ = false;
	pthread_exit(NULL);
}

/**
 * @brief Function to get the network socket and first HTTP request data from Unix domain socket.
 *
 * @param[in] unix_domain_sock Unix domain socket.
 * @param[in] fd The received network socket.
 * @param[in] data The received HTTP request.
 * @param[in] data_len The length of received HTTP request.
 *
 * @return number of bytes those receive.
 */
int FDHTTPServer::get_fd(int unix_domain_sock, int *fd, void *data, size_t data_len)
{
	struct iovec iov;
	struct msghdr msg;
	char cmsgbuf[CMSG_SPACE(sizeof(int))];
	struct cmsghdr *cmsgptr;
	int n_read;

	*fd = -1;

	iov.iov_base = data;
	iov.iov_len = data_len;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = cmsgbuf;
	msg.msg_controllen = sizeof(cmsgbuf);
	cmsgptr = CMSG_FIRSTHDR(&msg);

	cmsgptr->cmsg_level = SOL_SOCKET;
	cmsgptr->cmsg_type = SCM_RIGHTS;
	cmsgptr->cmsg_len = CMSG_LEN(sizeof(int));
	*(int *)CMSG_DATA(cmsgptr) = -1;

	n_read = recvmsg(unix_domain_sock, &msg, 0);
	if (n_read <= 0)
	{
		return n_read;
	}

	cmsgptr = CMSG_FIRSTHDR(&msg);
	if (cmsgptr && 
			cmsgptr->cmsg_level == SOL_SOCKET && 
			cmsgptr->cmsg_type == SCM_RIGHTS) 
	{
		*fd = *(int*)CMSG_DATA(cmsgptr);
	}

	return n_read;
}

/**
 * @brief This function will be called when connections exit.
 *
 * @param[in] connection The pointer of exited connection.
 */
void FDHTTPServer::connection_exit_callback(FDHTTPConnectionPtr connection)
{
	pthread_mutex_lock(&map_mutex_);

	ConnectionBufMap::iterator iter = con_buf_map_.find(connection);
	assert(iter != con_buf_map_.end());

	FDHTTPConnection* temp_connection = iter->first;
	char* temp_buf = iter->second;

	con_buf_map_.erase(iter);
	if(temp_buf)
	{
		delete temp_buf;
	}

	if(temp_connection)
	{
		delete temp_connection;
	}

	fprintf(stdout,"connection %p exit >>>>>>>>>>>>>.\n", (void*)connection);
	pthread_mutex_unlock(&map_mutex_);
}

} // namespace vtcs
