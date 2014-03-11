//============================================================================
// Name        : Emulation.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include "EmulationWrapper.h"
#include "CGUIGTK.h"
#include <gtkmm/application.h>
#include "CSocketNotReadyException.h"

//TODO: here for testing purposes. remove when done
#include "CXMLBuilder.h"

/**
 * The main function of the program that initializes the loggers
 * and the operating system itself. Some preceding steps should be taken.
 * for more information view the README file attached to raanan's gmail
 * @param argc number of arguments provided for the emulation
 * @param argv the arguments themselves
 * @return Failure value if at some point an exception was thrown
 */
int main(int argc, char *argv[])
{
//	//TODO: remove after testing
//
//	CXMLBuilder builder;
//	RouterInformation rt;
//	rt.sBufferSize=100;
//	rt.sDropRate=0;
//	rt.sRouterNumber=0;
//	builder.AddRouter(rt);
//	builder.AddPhysicalConnection("test1");
//	//should fail
//	rt.sBufferSize=150;
//	builder.AddRouter(rt);
//	builder.AddPhysicalConnection("test2");
//
//	builder.AddPhysicalConnection("test1");
//	//should work
//	rt.sBufferSize=250;
//	rt.sRouterNumber=2;
//	builder.AddRouter(rt);
//
//	builder.AddPhysicalConnection("test3");
//	builder.SetCurrentRouter(0);
//	builder.AddPhysicalConnection("test3");
//	rt.sRouterNumber=0;
//	rt.sBufferSize=350;
//	builder.EditCurrentRouterInformation(rt);
//
//	builder.Finalize();
//	return(EXIT_SUCCESS);

	try
	{

		//if you uncomment this the gui window will stay open until the program closes
		int a_argc = 1;
		Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(a_argc,argv,"org.NetworkEmulation");
		CGUIGTK window;
		app->run(window);

		return(EXIT_SUCCESS);
	}
	catch (CException & error)
	{
		return(EXIT_FAILURE);
	}
}

