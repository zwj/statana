#include "url_parser_test.h"
#include <iostream>
#include <improve/net/url_parser.h>
#include <string>

using namespace std; 

CPPUNIT_TEST_SUITE_REGISTRATION(url_parser_testcase); 
void
url_parser_testcase::ftp_parse_test()
{
	url_parser  parser;
	string ftp_str("ftp://pan:hubei@61.172.241.115:8080/test/mir2setup.exe");
	url_info info = parser.parse(ftp_str);
	CPPUNIT_ASSERT(info._protocal == "ftp"); 
	CPPUNIT_ASSERT(info._username == "pan"); 
	CPPUNIT_ASSERT(info._password == "hubei"); 
	CPPUNIT_ASSERT(info._hostname == "61.172.241.115"); 
	CPPUNIT_ASSERT(info._port	== 8080); 
	CPPUNIT_ASSERT(info._path	== "/test/mir2setup.exe"); 
	cout<<info._filename;
	CPPUNIT_ASSERT(info._filename	== "mir2setup.exe"); 
}
void
url_parser_testcase::http_parse_test()
{
	url_parser  parser;
	string ftp_str("http://61.172.241.115:8080/test/mir2setup.exe");
	url_info info = parser.parse(ftp_str);
	CPPUNIT_ASSERT(info._protocal == "http"); 
	CPPUNIT_ASSERT(info._username.empty() ); 
	CPPUNIT_ASSERT(info._password.empty() ); 
	CPPUNIT_ASSERT(info._hostname == "61.172.241.115"); 
	CPPUNIT_ASSERT(info._port	== 8080); 
	CPPUNIT_ASSERT(info._path	== "/test/mir2setup.exe"); 
	CPPUNIT_ASSERT(info._filename	== "mir2setup.exe"); 
}
void 
url_parser_testcase::no_port_test()
{
	url_parser  parser;
	string ftp_str("http://61.172.241.115/mir2setup.exe");
	url_info info = parser.parse(ftp_str);
	CPPUNIT_ASSERT(info._protocal == "http"); 
	CPPUNIT_ASSERT(info._username.empty() ); 
	CPPUNIT_ASSERT(info._password.empty() ); 
	CPPUNIT_ASSERT(info._hostname == "61.172.241.115"); 
	CPPUNIT_ASSERT(info._port	== 80); 
	CPPUNIT_ASSERT(info._path	== "/mir2setup.exe"); 
	CPPUNIT_ASSERT(info._filename	== "mir2setup.exe"); 
}
