#ifndef __TEST_BASE__
#define __TEST_BASE__

#include<cppunit/extensions/HelperMacros.h> 
#include<cppunit/TestCase.h> 
class test_base : public CppUnit::TestCase 
{
public:		
	test_base()	
	//:_logger(log4cpp::Category::getInstance("test"))
	{}				
protected:		
	//log4cpp::Category& _logger;
};

#endif
