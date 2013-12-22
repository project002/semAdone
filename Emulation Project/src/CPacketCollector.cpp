/*
 * CPacketCollector.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: root
 */

#include "CPacketCollector.h"

CPacketCollector::CPacketCollector(const int Socket):mSocket(Socket)
{

}

CPacketCollector::~CPacketCollector()
{
	try
	{
		list<CPacket * >::iterator it=mReceivedPackets.begin();
		for (;it!=mReceivedPackets.end();it++)
		{
			delete (*it);
		}
		mReceivedPackets.clear();
		it=mTransmittedPackets.begin();
		for (;it!=mTransmittedPackets.end();it++)
		{
			delete (*it);
		}
		mTransmittedPackets.clear();
	}
	catch(CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
	}
}
void CPacketCollector::SendPacket(char * buffer, ssize_t recvSize)
{
	try
	{
		sendto(mSocket,buffer,recvSize,0,NULL,0);
	}
	catch(CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
	}
}
void CPacketCollector::ReceivePackets()
{
	try
	{
		char buffer[ETH_FRAME_LEN] = { 0 };
		ssize_t recvSize;
		CPacket * NewPacket = NULL;
		while (true)
		{
			recvSize = recvfrom(mSocket, buffer, ETH_FRAME_LEN, MSG_DONTWAIT,
					NULL, NULL);
			if (recvSize == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
			{
				cerr << "err number " << errno << endl;
				throw CException("fatal error on receive from socket");
			}

			if (recvSize > 0) //don't print empty packets
			{
				NewPacket = CreatePacket(buffer, recvSize);
				if (NewPacket != NULL)
				{
					NewPacket->Print();
					mReceivedPackets.push_back(NewPacket);
					//TODO: handle the buffer so it won't over flow!!!!!!!!!!!!!!!!!!!!!!
				}
			}
		}
	}
	catch (CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
	}
}

CPacket * CPacketCollector::CreatePacket(char * buffer, ssize_t recvSize)
{

	try
	{
		uint16_t ethernetType = ((buffer[ETH_ALEN * 2] & 0xFF) << 8) | (buffer[(ETH_ALEN * 2) + 1] & 0xFF) ;
		switch (ethernetType)
		{
			case (ETH_P_IP):
				cout << "\n Creating IPv4 Packet \n";
				return CreateIPv4Packet(buffer, recvSize);
				break;
			case (ETH_P_ARP):
				cout << "\n Creating ARP Packet\n";
				return CreateARPPacket(buffer, recvSize);
				break;
			case (ETH_P_IPV6):
				cout << "\n Creating IPv6 Packet\n";
				return CreateIPv6Packet(buffer, recvSize);
				break;
			default:
				PrintPacket(buffer, recvSize);
				cerr << ethernetType << endl;
				throw(CException("\nCan't find ethernet type\n"));
				break;
		}
		return NULL;
	}
	catch(CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
	}
	return NULL;
}
/**
 * Function for debugging. It prints the whole packet as it was received.
 * @param buffer pointer to the array's beginning
 * @param recvSize size of the received packet in bytes
 */
void CPacketCollector::PrintPacket(char * buffer, ssize_t recvSize)
{
	try
	{
		int i=0;
		for (;i<recvSize;i++)
		{
			printf("%02X:",uint8_t(buffer[i]));
		}
		cout<<endl;
	}
	catch(CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
	}
}

CPacket * CPacketCollector::CreateIPv4Packet(char * buffer, ssize_t recvSize)
{

	try
	{
		return ( new CPacketIPv4 ( buffer,recvSize));
	}
	catch(CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
	}
	return NULL;
}

CPacket * CPacketCollector::CreateIPv6Packet(char * buffer, ssize_t recvSize)
{

	try
	{
		return new CPacketIPv6(buffer,recvSize);
	}
	catch(CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
	}
	return NULL;
}


CPacket * CPacketCollector::CreateARPPacket(char * buffer, ssize_t recvSize)
{

	try
	{
		return new CPacketARP(buffer,recvSize);
	}
	catch(CException & error)
	{
		std::cerr << error.what() << std::endl;
		std::cerr << __PRETTY_FUNCTION__ << std::endl;
	}
	return NULL;
}



































/* // If we want to implement more packet types there is a need for a switch :
CPacket * CPacketCollector::CreateIPv4Packet(char * buffer, ssize_t recvSize)
{

	try
	{
		uint8_t IPv4Type = buffer[ETH_HLEN +9];

		switch (IPv4Type)
		{
			case (IPPROTO_TCP):
					//TCP packet

				break;
			case (IPPROTO_ICMP):
					//ICMP packet

				break;
			case (IPPROTO_UDP):
					//UDP packet

				break;
			case (IPPROTO_RAW):
					//RAW IP packet - no data

				break;
			default:
				PrintPacket(buffer, recvSize);
				cerr  << endl;
				throw(CException("Can't find ethernet type"));
				break;
		}
		//TODO :switch with ipv4 protocols
		return NULL;
	}
	catch (CException & e)
	{
		std::cerr << e.what() << std::endl;
	}
}*/
