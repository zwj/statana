#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <log4cpp/PropertyConfigurator.hh>
#include <stdexcept>
#include <iostream>
using namespace std;
using namespace CppUnit;
using namespace log4cpp;
int main( int argc, char* argv[] )
{
	try
	{
		PropertyConfigurator::configure("../config/log4cpp.properties");
		TextUi::TestRunner runner;
		TestFactoryRegistry &registry = TestFactoryRegistry::getRegistry();
		runner.addTest( registry.makeTest() );
		runner.run();
	}
	catch(exception &e )
	{
		cout<<e.what()<<endl;
	}
	catch(...)
	{
		cout<<"unknow exception "<<endl;
	}

    return 0;
}
