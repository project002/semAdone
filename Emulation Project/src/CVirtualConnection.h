/*
 * CVirtualConnection.h
 *
 *  Created on: Dec 31, 2013
 *      Author: root
 */

#ifndef CVIRTUALCONNECTION_H_
#define CVIRTUALCONNECTION_H_
//TODO: add virtual connection randomized mac address. or to router....
#include "CConnection.h"
#include "CPacketCollector.h"
#define NUMBER_OF_CONNECTED_DEVICES 2
#include <algorithm>
class CVirtualConnection: public CConnection
{
public:
	CVirtualConnection();
	virtual Crafter::Packet* GetPacket(int routerNumber=NO_CHOICE_OF_PACKET_COLLECTOR);
	virtual bool SendPacket(Packet* packet,int routerNumber=NO_CHOICE_OF_PACKET_COLLECTOR);
	CUIPV4*& getGetwayAddress() {throw CException("missing implementation of get gateway in virtual connection");}
	void AddInvolvedRouter(const unsigned int & routerNumber);
	bool IsInvolved(const unsigned int &routerNumber){return std::find(mInvolvedRoutersByNumber.begin()
			, mInvolvedRoutersByNumber.end(), routerNumber)!=mInvolvedRoutersByNumber.end();}
	virtual ~CVirtualConnection();
	void AddRoutingTableReference(map<string,pair<CConnection const*,string> > * routTable, int routerNumber);
	virtual vector<pair<string, string> >& GetTable() ;
private:
	vector<unsigned int> mInvolvedRoutersByNumber;
	CPacketCollector mPacketCollectors[NUMBER_OF_CONNECTED_DEVICES];
	map<unsigned int ,unsigned int > mRouterToPacketCollectorMap;
	//MAP < IP , <Connection,MAC> > reference
	map<string,pair<CConnection const*,string> > * mRoutingTable[NUMBER_OF_CONNECTED_DEVICES];
	set< string > mCombinedRoutingTable;
	vector<pair<string, string> > mRoutingToReturn;
};

#endif /* CVIRTUALCONNECTION_H_ */
