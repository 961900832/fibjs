/*
 * net.cpp
 *
 *  Created on: Apr 22, 2012
 *      Author: lion
 */

#include "Socket.h"

namespace fibjs
{

result_t net_base::resolve(const char* name, int32_t family,
		std::string& retVal, exlib::AsyncEvent* ac)
{
	if (family != net_base::_AF_INET && family != net_base::_AF_INET6)
		return CALL_E_INVALIDARG;

	if (!ac)
		return CALL_E_NOSYNC;

	inetAddr addr_info;

	addr_info.init(family);

	addrinfo hints =
	{ 0 };
	addrinfo *result = NULL;
	addrinfo *ptr = NULL;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(name, NULL, &hints, &result))
		return SocketError();

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
		if (ptr->ai_family == addr_info.addr4.sin_family)
		{
			memcpy(&addr_info, ptr->ai_addr, addr_info.size());
			break;
		}

	freeaddrinfo(result);

	if (ptr == NULL)
	{
#ifdef _WIN32
		return -WSAHOST_NOT_FOUND;
#else
		return -ETIME;
#endif
	}

	retVal = addr_info.str();

	return 0;
}

result_t net_base::ip(const char* name, std::string& retVal,
		exlib::AsyncEvent* ac)
{
	return resolve(name, net_base::_AF_INET, retVal, ac);
}

result_t net_base::ipv6(const char* name, std::string& retVal,
		exlib::AsyncEvent* ac)
{
	return resolve(name, net_base::_AF_INET6, retVal, ac);
}


}
