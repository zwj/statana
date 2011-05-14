#ifndef  __DIMENSION_TEST_H__
#define  __DIMENSION_TEST_H__
#include<cppunit/extensions/HelperMacros.h> 
#include<cppunit/TestCase.h> 
class dimension_factory_testcase : public CppUnit::TestCase 
{
CPPUNIT_TEST_SUITE(dimension_factory_testcase); 
CPPUNIT_TEST(create_defut); 
CPPUNIT_TEST_SUITE_END(); 
public: 
	dimension_factory_testcase();	
	void create_defut();
};
#endif

