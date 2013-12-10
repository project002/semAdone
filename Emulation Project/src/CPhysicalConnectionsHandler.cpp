/*
 * CPhysicalConnectionsHandler.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: project
 */

#include "CPhysicalConnectionsHandler.h"

CPhysicalConnectionsHandler::CPhysicalConnectionsHandler():mPhysicalConnections(NULL)
{
	mPhysicalConnections = new vector<CPhysicalConnection * >;
}

CPhysicalConnectionsHandler::~CPhysicalConnectionsHandler()
{
	try
	{
		vector<CPhysicalConnection * >::iterator it;
		for (it = mPhysicalConnections->begin(); it!=mPhysicalConnections->end();it++)
		{
			delete (*it);
		}
		delete mPhysicalConnections;
	}
	catch(CException & error)
	{

	}
}

/**
 * Function performs system calls to check the connected Ethernet cards and open sockets accordingly
 * using the Physical Connection class
 * // TODO : check when a physical link should be created and what is not a real interface.
 */
/*void CPhysicalConnectionsHandler::CreatePhyiscalConnections()
{
	try
	{
		struct ifaddrs* list;
		struct ifaddrs* node;
		int ret = 0;
		int address_size = 0;
		int address_int = 0;
		char host_buffer[NI_MAXHOST];

		//get devices list
		ret = getifaddrs(&list);
		if (ret == -1) {throw CException("Err: when retrieving devices list");}

		//iterate over list
		for (node = list; node != NULL; node = node->ifa_next)
		{
			std::cout << "name:\t" << node->ifa_name << std::endl;

			if (node->ifa_addr == NULL)
			{
				std::cout << "empty address" << std::endl;
				continue;
			}

			std::cout << "\tfamily type:";
			switch (node->ifa_addr->sa_family)
			{
				case AF_PACKET:
					std::cout << "AF_PACKET";
					break;
				case AF_INET:
					std::cout << "AF_INET";
					break;
				case AF_INET6:
					std::cout << "AF_INET6";
					break;
			}
			std::cout << std::endl;

			std::cout << "\taddress:";
			switch (node->ifa_addr->sa_family)
			{
				case AF_INET:
					address_size = sizeof(struct sockaddr_in);
					break;
				case AF_INET6:
					address_size = sizeof(struct sockaddr_in6);
					break;
			}
			address_int = getnameinfo(node->ifa_addr, address_size, host_buffer,
					NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

			if (address_int != 0)
			{
				std::cout << "no address" << std::endl;
				continue;
			}
			else
			{
				std::cout << host_buffer << std::endl;
			}

		}

		//clear the allocated list
		freeifaddrs(list);
	}
	catch (CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
	}
}*/

/**
 *  modefied method for testing
 */
void CPhysicalConnectionsHandler::CreatePhyiscalConnections()
{
	try
	{
		struct ifaddrs* list;
		struct ifaddrs* node;
		int ret = 0;
		int address_size = 0;
		int address_int = 0;
		char host_buffer[NI_MAXHOST];

		//get devices list
		ret = getifaddrs(&list);
		if (ret == -1) {throw CException("Err: when retrieving devices list");}

		//iterate over list
		int i = 0;
		for (i=0,node = list; node != NULL; node = node->ifa_next,++i)
		{
			std::cout << "name:\t" << node->ifa_name << std::endl;

			if (node->ifa_addr == NULL)
			{
				continue;
			}

			std::cout << "\tfamily type:";
			switch (node->ifa_addr->sa_family)
			{
				case AF_PACKET:
					std::cout << "AF_PACKET";
					if(strcmp(node->ifa_name,"lo")==0)
					{
						break;
					}
					mPhysicalConnections->push_back(new CPhysicalConnection(node,i));
					break;
				case AF_INET:
					std::cout << "AF_INET";
					break;
				case AF_INET6:
					std::cout << "AF_INET6";
					break;
			}
			std::cout << std::endl;

			std::cout << "\taddress:";
			switch (node->ifa_addr->sa_family)
			{
				case AF_INET:
					address_size = sizeof(struct sockaddr_in);
					break;
				case AF_INET6:
					address_size = sizeof(struct sockaddr_in6);
					break;
			}
			address_int = getnameinfo(node->ifa_addr, address_size, host_buffer,
					NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

			if (address_int != 0)
			{
				std::cout << "no address" << std::endl;
				continue;
			}
			else
			{
				std::cout << host_buffer << std::endl;
			}

		}

		//clear the allocated list
		freeifaddrs(list);
	}
	catch (CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
	}
}
