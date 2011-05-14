#include "rule_factory_test.h"
#include <logic/rule_factory.h>
#include <logic/config_error.h>

CPPUNIT_TEST_SUITE_REGISTRATION(rule_factory_testcase); 
using namespace std;
using namespace logs::logic;
using namespace logs::entity;
void 
rule_factory_testcase::symbol_test()
{/*{{{*/
	rule_factory factory;
	CPPUNIT_ASSERT(factory.is_relation_symbol("&&")); 
	CPPUNIT_ASSERT(factory.is_relation_symbol("||")); 
	CPPUNIT_ASSERT(factory.is_relation_symbol("!")); 

	CPPUNIT_ASSERT(!factory.is_relation_symbol("&")); 
	CPPUNIT_ASSERT(!factory.is_relation_symbol("|")); 
	CPPUNIT_ASSERT(!factory.is_relation_symbol("!!")); 


	CPPUNIT_ASSERT(factory.is_compare_symbol("<")); 
	CPPUNIT_ASSERT(factory.is_compare_symbol("<=")); 
	CPPUNIT_ASSERT(factory.is_compare_symbol(">=")); 
	CPPUNIT_ASSERT(factory.is_compare_symbol(">")); 
	CPPUNIT_ASSERT(factory.is_compare_symbol("<>")); 

	CPPUNIT_ASSERT(!factory.is_compare_symbol("><")); 
	CPPUNIT_ASSERT(!factory.is_compare_symbol("<<")); 
	CPPUNIT_ASSERT(!factory.is_compare_symbol(">>")); 

	CPPUNIT_ASSERT(!factory.is_compare_symbol("&<")); 
	CPPUNIT_ASSERT(!factory.is_compare_symbol("<<")); 
	CPPUNIT_ASSERT(!factory.is_compare_symbol(">>")); 
}/*}}}*/
void
rule_factory_testcase::analyse_test()
{/*{{{*/
	string  data("$ff >= 1");
	rule_factory factory;
	string left,cmp,right;
	factory.analyse_compare_exp(data,left,cmp,right);
    cout<<"*********1********"<<endl;
	cout<<"left: "<< left<<endl;
	cout<<"cmp :"<< cmp <<endl;
	cout<<"right :"<< right<<endl;
	CPPUNIT_ASSERT("$ff" == left ); 
	CPPUNIT_ASSERT(">=" == cmp); 
	CPPUNIT_ASSERT("1" == right); 
	
	
    cout<<"*********2********"<<endl;
	string  data1("$TIME  == \"2004-3-1 10:10:10\"");
	factory.analyse_compare_exp(data1,left,cmp,right);

	cout<<"left :"<< left<<endl;
	cout<<"cmp :"<< cmp <<endl;
	cout<<"right :"<< right<<endl;
	CPPUNIT_ASSERT("$TIME" == left); 
	CPPUNIT_ASSERT("==" == cmp);
	CPPUNIT_ASSERT("\"2004-3-1 10:10:10\"" == right); 

    
    cout<<"*********3********"<<endl;
	string  data2("($ff >=1) && ($TIME == \"2004-2-1 10:10:10\")");
	CPPUNIT_ASSERT(factory.analyse_relation_exp(data2,left,cmp,right));
	cout<<"left:"<<left<<endl;
	cout<<"cmp :"<< cmp <<endl;
	cout<<"right:"<<right<<endl;
	CPPUNIT_ASSERT("$ff >=1" == left); 
	CPPUNIT_ASSERT("&&" == cmp);
	CPPUNIT_ASSERT("$TIME == \"2004-2-1 10:10:10\"" == right); 
}/*}}}*/

void 
rule_factory_testcase::config_error_test()
{/*{{{*/
	try
	{
		config_error test_err(_WHERE);
		
		test_err.stream()<<"kill it";
		throw test_err;
	}
	catch(exception& e)
	{
		cout<<"exception"<<e.what();	
	}
	
}/*}}}*/

void 
rule_factory_testcase::rule_test()
{
    string str="(  $IP == 192.168.0.1 ) && ( $TIME == 2004-10-10 0:0:0 )";
    string str1="(IP==192.168.0.1)&&($TIME==2004-10-10 0:0:0)";
    string str2="( $IP == 192.168.0.1) && ($TIME == 2004-10-10 0:0:0 )";
	rule_factory factory;
    rule::shared_ptr rule_ptr = factory.create_rule(str);
    rule::shared_ptr rule_ptr1 = factory.create_rule(str1);
    rule::shared_ptr rule_ptr2 = factory.create_rule(str2);
    CPPUNIT_ASSERT(rule_ptr != NULL);
    CPPUNIT_ASSERT(rule_ptr1 != NULL);
}
