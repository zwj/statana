#include "utility_test.h" 
#include <entity/utility.h>
#include <iostream>
CPPUNIT_TEST_SUITE_REGISTRATION(utility_testcase); 

using namespace std ; 
void 
utility_testcase::unify_test()
{
	string lowcase(" abc7 def ");
	string upcasetest= utility::unify(lowcase);
	CPPUNIT_ASSERT(upcasetest==string("ABC7 DEF")); 

	string one("     Lala");
	CPPUNIT_ASSERT(utility::unify(one) == string("LALA"));
	
	string two("Lala  ");
	CPPUNIT_ASSERT(utility::unify(two) == string("LALA"));
	string three("La     la");
	CPPUNIT_ASSERT(utility::unify(three) == string("LA     LA"));
	string forth("LALA");
	CPPUNIT_ASSERT(utility::unify(forth) == string("LALA"));
	string five("12345678");
	CPPUNIT_ASSERT(utility::unify(five) == string("12345678"));
}

void 
utility_testcase::split_test()
{
    string first,second;
    CPPUNIT_ASSERT(utility::split_pair("abc=cba",first,second,"="));
    CPPUNIT_ASSERT_EQUAL(first,string("abc"));
    CPPUNIT_ASSERT_EQUAL(second,string("cba"));
    CPPUNIT_ASSERT(!utility::split_pair("abc=",first,second,"="));
    CPPUNIT_ASSERT(utility::split_pair("abc.cba",first,second,"."));
}

