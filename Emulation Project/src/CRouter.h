/*
 * CRouter.h
 *
 *  Created on: Dec 31, 2013
 *      Author: root
 */

#ifndef CROUTER_H_
#define CROUTER_H_
#include "BasicIncludes.h"
#include "CConnection.h"
#include "CUIPV4.h"
#include "CPacketCollector.h"
#include "CVirtualConnection.h"
/**
 * Thread Related Inclusions
 */
#include <boost/thread.hpp>
#include <boost/tuple/tuple.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/signals2/mutex.hpp>
#define DEFAULT_ROUTER_BUFFER_SIZE 100
class CRouter
{
public:
	CRouter();
	virtual ~CRouter();
	void AddConnection(const CConnection * connection){mConnections.push_back(connection);}
	void AppendConnectionList(list<CVirtualConnection const *> &connectionList);
	void RequestTables();
	void Sniffer();
	void nonThreadedSniffer();
	void nonThreadedInit();
	void StopEmulation();

	unsigned int GetBufferSize() const
	{
		return mBufferSize;
	}

	void SetBufferSize(unsigned int bufferSize)
	{
		mBufferSize = bufferSize;
	}

	double GetDropRate() const
	{
		return mDropRate;
	}

	void SetDropRate(double dropRate)
	{
		mDropRate = dropRate;
	}

	unsigned int GetRouterNumber() const
	{
		return mRouterNumber;
	}

	void SetRouterNumber(unsigned int routerNumber)
	{
		mRouterNumber = routerNumber;
	}

	void AddPacketsToBuffer(unsigned int numberOfPackets=0)
	{
		mPacketCollector->AddRandomPackets(numberOfPackets);
	}

	void setRouterThreaded(bool threaded) {mThreaded = threaded;}

	/**
	 * this method determines if the router is comprised out of
	 * virtual connection only
	 * Note: this method uses RTTI so use it in the init only
	 * @return
	 */
	bool isVirtualRouter()
	{
		//check if all connection are virtual =>| this router is virtual
		list<CConnection const *>::iterator it = mConnections.begin();
		CVirtualConnection const * cvc;
		for (;it!=mConnections.end();++it)
		{

			cvc = dynamic_cast<CVirtualConnection const *>(*it);
			if (cvc == NULL) {return false;}
		}
		return true;
	}

	double GetFillage() const
	{
		return mFillage;
	}

	void SetFillage(double fillage)
	{
		mFillage = fillage;
	}

	unsigned int GetInitialBufferUse() const
	{
		return mInitialBufferUse;
	}

	void SetInitialBufferUse(unsigned int initialBufferUse)
	{
		mInitialBufferUse = initialBufferUse;
	}

private:
	void HandleArp(Packet * pkt);
	void HandleIPv4(Packet * pkt,const double popTime);
	void Sniff();
	void PacketHandler();
	bool ProcessSendPacket(Packet* packet);
	list<CConnection const *> mConnections;
	//MAP < IP , <Connection,MAC> >
	map<string,pair<CConnection const*,string> > mRoutingTable;
	unsigned int mBufferSize;
	boost::thread mSniffingThread;
	boost::thread mPacketsHandlingThread;
	CPacketCollector * mPacketCollector;
	double mDropRate;
	unsigned int mRouterNumber;
	bool mThreaded; //determins if the router is threaded or not
	boost::signals2::mutex mMtx;
	double mFillage;
	unsigned int mInitialBufferUse;
};

#endif /* CROUTER_H_ */
