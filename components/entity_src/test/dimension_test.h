#ifndef  __DIMENSION_TEST_H__
#define  __DIMENSION_TEST_H__
#include<cppunit/extensions/HelperMacros.h> 
#include<cppunit/TestCase.h> 
class dimension_testcase : public CppUnit::TestCase 
{
CPPUNIT_TEST_SUITE(dimension_testcase ); 
CPPUNIT_TEST(ip_test); 
CPPUNIT_TEST(time_test); 
CPPUNIT_TEST(args_test); 
CPPUNIT_TEST(factory_test); 
CPPUNIT_TEST(addi_test); 
CPPUNIT_TEST(str_dim_test); 
CPPUNIT_TEST_SUITE_END(); 
//log4cpp::Category& _logger;
public: 
	dimension_testcase();	
	void ip_test();
	void time_test();
	void args_test();
	void factory_test();
	void addi_test();
    void str_dim_test();
};
#endif

