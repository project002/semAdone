/*
 * CBasicGUI.h
 * offer basic gui for the emulation
 * using the terminal as the platform
 */

#ifndef CBASICGUI_H_
#define CBASICGUI_H_
#include <iostream>
#include <map>
#include <vector>
#include <boost/thread.hpp>

#define REFRESH_RATE 0.5

class SDataController
{
public:
	static enum DATATYPE {
		PACKETPROCCES,PACKETDROP,IPPACKET,DHCPPACKET,ARPPACKET
	} dataType;

	static SDataController &getInstance()
	{
		static SDataController ref;
		return ref;
	}

	void init()
	{
		clear();
		messages.push_back("Emulation Started");
//		gui_refresh = boost::thread(&SDataController::output,this);
	}

//	std::string get_data()
//	{
//		refreshMTX.lock();
//		std::string temp = mOut;
//		refreshMTX.unlock();
//		return temp;
//	}

	void refresh()
	{
		refreshMTX.lock();
		dropped_packets = calcLostPacketsPercent();
		refreshMTX.unlock();
	}

	void clear()
	{
		mOut="";
	}

	void destroy()
	{
		gui_refresh.interrupt();
		clear();
		dataSet.clear();
		messages.clear();
		insertMTX.unlock();
		refreshMTX.unlock();
	}

	std::string& get_messages()
	{
		stringstream outputSS;
		//print out messages:
		std::vector<std::string>::iterator it = messages.begin();
		for (;it!=messages.end();++it)
		{
			outputSS << "[#]" << (*it) << endl;
		}

		mOut = outputSS.str();
		return mOut;
	}

	bool insertData(DATATYPE type,int unsigned value)
	{
		insertMTX.lock();
		std::pair<std::map<DATATYPE,int unsigned>::iterator,bool> ret;
		ret = dataSet.insert(std::pair<DATATYPE,int unsigned>(type,value));
		if (!ret.second)
		{
			dataSet.at(type) = value;
			insertMTX.unlock();
			return true;
		}
		insertMTX.unlock();
		return ret.second;
	}

	void incData(DATATYPE type)
	{
		insertMTX.lock();
		try
		{
			dataSet.at(type) += 1;
			insertMTX.unlock();
		}
		catch (const std::out_of_range & c)
		{
			insertMTX.unlock();
			insertData(type,1); //if not found add it
		}

	}

	void msg(std::string str)
	{
		messages.push_back(str);
	}

	void msg(const char* format,...)
	{
		va_list args;
		va_start(args,format);
		char str[256] = {0};
		vsprintf(str,format,args);
		va_end(args);
		messages.push_back(std::string(str));
	}

	int unsigned get_data(DATATYPE type)
	{
		insertMTX.lock();
		int unsigned data = 0;
		try {data = dataSet.at(type);}
		catch(const std::out_of_range & e) {}
		insertMTX.unlock();
		return data;
	}

	double get_dropped_precentage()
	{
		refresh();
		return dropped_packets;
	}

	virtual ~SDataController()
	{
		//gui_refresh.interrupt();
		dataSet.clear();
		messages.clear();
		insertMTX.unlock();
		refreshMTX.unlock();
	}

private:
	std::string mOut;
	std::map<SDataController::DATATYPE,int unsigned> dataSet;
	std::vector<std::string> messages;
	boost::signals2::mutex insertMTX;
	boost::signals2::mutex refreshMTX;
	boost::signals2::mutex insertRouterMTX;
	boost::thread gui_refresh;
	double dropped_packets;

	SDataController():dataSet(std::map<SDataController::DATATYPE,int unsigned>()),
				dropped_packets(0){}
	SDataController(const SDataController &):dataSet(std::map<SDataController::DATATYPE,int unsigned>()),
								 dropped_packets(0){}

	void operator=(SDataController const &);

	double calcLostPacketsPercent()
	{
		int unsigned processed = get_data(PACKETPROCCES);
		int unsigned dropped = get_data(PACKETDROP);

		int unsigned packets = processed+dropped;
		if (packets == 0) {return 0;}
		double dropped_percentage = (100.0/packets)*dropped;
		return dropped_percentage;
	}

};

#endif /* CBASICGUI_H_ */