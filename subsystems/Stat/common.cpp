#include"common.h"
#include <exception>
#include <iostream>
#include <log4cpp/PropertyConfigurator.hh>
using namespace std;
using namespace log4cpp;

bool config_log(const string& name)
{	try/*{{{*/
	{		
		PropertyConfigurator::configure(name);
	}
	catch(exception& ce)
	{
		cout<<ce.what()<<endl;	
		return false;
	}
	catch(...)
	{
		cout<< "log config error"<<endl;	
		return false;
	}	
	return true;
}/*}}}*/

