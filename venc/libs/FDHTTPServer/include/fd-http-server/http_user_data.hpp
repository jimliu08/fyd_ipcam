#ifndef _HTTP_USER_DATA_HPP_
#define _HTTP_USER_DATA_HPP_

namespace vtcs
{

/// The information to pass to user function.
class HttpUserData
{	
public:
	HttpUserData()
		: socket_fd_ptr(NULL)
	{
	}

	// Use it carefully and do not close it in user function. Otherwise you may break the state machine.
	// If you want to use it in the thread, you need to wait the thread in the user callback function.
	/// The pointer of socket file descriptor.
	const int* socket_fd_ptr;
};

} // namespace vtcs

#endif

