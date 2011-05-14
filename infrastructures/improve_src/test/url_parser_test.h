#ifndef _URL_PARSER_TEST_H_
#define _URL_PARSER_TEST_H_
#include<cppunit/extensions/HelperMacros.h>	
#include<cppunit/TestCase.h> 

class url_parser_testcase : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE(url_parser_testcase); 
	CPPUNIT_TEST(ftp_parse_test); 
	CPPUNIT_TEST(http_parse_test); 
	CPPUNIT_TEST(no_port_test); 
	CPPUNIT_TEST_SUITE_END();	
public:
	void ftp_parse_test();
	void http_parse_test();
	void no_port_test();
};

#endif
