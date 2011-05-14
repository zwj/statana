#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <stdexcept>
#include <iostream>
using namespace std;
using namespace CppUnit;
int main( int argc, char* argv[] )
{
	try
	{
		//PropertyConfigurator::configure("log4cpp.properties");
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
