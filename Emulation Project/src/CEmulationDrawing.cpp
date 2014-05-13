/*
 * CEmulationDrawing.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: root
 */

#include "CEmulationDrawing.h"
#include <cairomm/context.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <glibmm/fileutils.h>
#include <iostream>
#include "CException.h"
//canvas padding for the physical router & connection
#define CNVS_PAD 20
CEmulationDrawing::CEmulationDrawing() :
	mXMLPath(),
	mElementsPos(new std::map<unsigned int,pair<int,int> >()),
	mImgBuffers(std::map<Glib::ustring,Glib::RefPtr<Gdk::Pixbuf> >())
{
	loadImagesSrouces();
}

void CEmulationDrawing::insertNewImage(Glib::ustring imageName,Glib::ustring imagePath)
{
	std::pair<Glib::ustring,Glib::RefPtr<Gdk::Pixbuf> > newImg;
	newImg.first = imageName;
	try
	{
		newImg.second = Gdk::Pixbuf::create_from_file(imagePath);
	}
	catch(const Glib::FileError& ex)
	{
		std::cerr << "Image File Missing: " << ex.what() << std::endl;
		throw CException("Missing Image File");
		return;
	}
	catch(const Gdk::PixbufError& ex)
	{
		std::cerr << "PixbufError: " << ex.what() << std::endl;
		throw CException("Unable To Load an Image");
		return;
	}
	mImgBuffers.insert(newImg);
}

void CEmulationDrawing::loadImagesSrouces()
{
	//insert router image
	insertNewImage("RouterImage","ricon.png");
	insertNewImage("RouterImageSource","rsicon.png");
}

CEmulationDrawing::~CEmulationDrawing()
{
	delete mElementsPos;
}

void CEmulationDrawing::resetDrawing(string xml_path)
{
	mXMLPath = xml_path;
	mXMLprs.ParseXML(mXMLPath);
	//clear info vectors or what not
	mElementsPos->clear();
	initial_positions();
}

void CEmulationDrawing::initial_positions()
{
	if (mXMLPath.length()==0) {return;}
	vector<RouterInformation> routers = mXMLprs.GetRoutersInformation();
	multimap<unsigned int,string> phCon = mXMLprs.GetPhysicalConnections();
	int x_percent=CNVS_PAD;
	int y_percent=50;
	int percent_inc = (100-(CNVS_PAD*2))/((routers.size()+1)-phCon.size());
	std::pair<int,int> pos;
	vector<RouterInformation>::iterator it = routers.begin();

	for (;it!=routers.end();++it)
	{
		unsigned int id = (*it).sRouterNumber;
		//is source element
		if (mXMLprs.isPhysicalRouter(id))
		{
			pos = next_source_router_pos();
		}
		else
		{
			x_percent += percent_inc;
			pos = std::pair<int,int>(x_percent,y_percent);
		}
		mElementsPos->insert(std::pair< unsigned int,std::pair<int,int> >(id,pos));
	}
}


std::pair<int, int> CEmulationDrawing::next_source_router_pos()
{
	static int i=0;
	static int r=4;
	int source_pos[][2] = {{CNVS_PAD,50},{100-CNVS_PAD,50},{50,CNVS_PAD},{50,100-CNVS_PAD}}; //matrix of source positions in percent
	if (i>=r) {i=0;}
	std::pair<int,int> ret = std::pair<int,int>(source_pos[i][0],source_pos[i][1]);
	i++;
	return ret;
}

std::pair<int, int> CEmulationDrawing::next_source_con_pos()
{
	static int i=0;
	static int r=4;
	int source_pos[][2] = {{0,50},{100,50},{50,0},{50,100}}; //matrix of source positions in percent
	if (i>=r) {i=0;}
	std::pair<int,int> ret = std::pair<int,int>(source_pos[i][0],source_pos[i][1]);
	i++;
	return ret;
}

int CEmulationDrawing::percent2pixel(int percent, int pixel_value)
{
	float v = (pixel_value/100.0)*percent;
	return (int) v;
}

vector<std::pair<int, int> > CEmulationDrawing::get_connected_routers(int id)
{
	vector<std::pair<int, int> > ret = vector<std::pair<int, int> >();

	if (mXMLPath.length()==0) {return ret;}
	multimap<unsigned int,unsigned int> vCons = mXMLprs.GetVirtualConnections();
	//can it be any longer?
	std::pair<multimap<unsigned int,unsigned int>::iterator,multimap<unsigned int,unsigned int>::iterator> range = vCons.equal_range(id);
	if (range.first == vCons.end()) {return ret;}

	multimap<unsigned int,unsigned int>::iterator it = range.first;
	for (;it!=range.second;++it)
	{
		ret.push_back((mElementsPos->at(it->second)));
	}
	return ret;

}

bool CEmulationDrawing::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (mImgBuffers.empty()) {return false;}

	//get the available draw area
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	std::vector< std::pair<int,int> > tps = std::vector< std::pair<int,int> >(); //temp target pos
	std::vector< std::pair<int,int> >::iterator tps_it;
	std::vector< std::pair<int,int> > temp;
	//draw elements by positions
	std::map<unsigned int,pair<int,int> >::iterator it = mElementsPos->begin();
	cr->set_source_rgb(0,0,0);
	cr->set_line_width(1.0);
	for (;it!=mElementsPos->end();++it)
	{
		string src_name = mXMLprs.isPhysicalRouter(it->first) ? "RouterImageSource" : "RouterImage";
		int base_pos[] = {percent2pixel(it->second.first,width) - mImgBuffers.at(src_name)->get_width()/2,
						  percent2pixel(it->second.second,height) - mImgBuffers.at(src_name)->get_height()/2};

		if (mXMLprs.isPhysicalRouter(it->first))
		{
			tps.push_back(next_source_con_pos());
		}

		temp = get_connected_routers(it->first);
		tps.insert(tps.end(),temp.begin(),temp.end());

		//paint lines
		for (tps_it = tps.begin();tps_it!=tps.end();++tps_it)
		{
			cr->move_to(base_pos[0],base_pos[1]);
			cr->line_to(percent2pixel((*tps_it).first,width),percent2pixel((*tps_it).second,height));
//			std::cout << "move to: " << base_pos[0] << "," << base_pos[1] << std::endl;
//			std::cout << "line to:" << percent2pixel((*tps_it).first,width) << " , " << percent2pixel((*tps_it).second,height) << std::endl;
//			cr->close_path();
		}
		cr->stroke();

		//paint router image
		Gdk::Cairo::set_source_pixbuf(cr,
				mImgBuffers.at(src_name),
				base_pos[0],
				base_pos[1]);

		cr->paint();
		tps.clear();
	}


	//draw all images
//	mImageBufferDItr = mImgBuffers.begin();
//	for (;mImageBufferDItr!=mImgBuffers.end();++mImageBufferDItr)
//	{
//		Gdk::Cairo::set_source_pixbuf(cr, mImageBufferDItr->second,
//			(width - mImageBufferDItr->second->get_width())/2,
//			(height - mImageBufferDItr->second->get_height())/2);
//
//		cr->paint();
//	}


//	Gdk::Cairo::set_source_pixbuf(cr, mImgBuffers.at("routerImage").second,
//				(width -  mImgBuffers.at("routerImage")->second->get_width())/2,
//				(height -  mImgBuffers.at("routerImage")->second->get_height())/2);

	cr->paint();

	return true;
}



