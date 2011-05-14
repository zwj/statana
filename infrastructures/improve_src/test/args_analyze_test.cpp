#include "args_analyze_test.h"
#include <improve/utility/args_analyze.h>
using namespace improve;
using namespace std;
CPPUNIT_TEST_SUITE_REGISTRATION(args_analyze_testcase); 


void 
args_analyze_testcase::short_args_test()
{
	char* args[] = {"/home/zwj/bin/vim","-S","improve.vim","-a","test"};
	args_analyzer analyzer(5,args);
	char opt=' ';
	string value; 
	bool   is_get = false;
	while(analyzer.getopt(opt,value))
	{
		switch(opt)
		{
		case 'S':
			CPPUNIT_ASSERT(value == "improve.vim"); 
			break;
		case 'a':
			CPPUNIT_ASSERT(value == "test"); 
			break;
		default:
			CPPUNIT_ASSERT(false); 
		}
		is_get = true;
	}
	CPPUNIT_ASSERT(is_get); 
}
