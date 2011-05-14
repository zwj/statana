#include "dimension_rule_test.h"
#include <entity/apache_anar.h>
#include <entity/dimension_rule.h>
#include <entity/log_item.h>
#include <entity/ip.h>
#include <entity/str_dimension.h>
#include <entity/args_dimension.h>
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(dimension_rule_testcase); 

using namespace std;
using namespace logs::entity;

void
dimension_rule_testcase::rule_args_equal(log_item::shared_ptr item)
{/*{{{*/
    int_dimension dim("%ft3721",9);
	matcher std_matcher(&dim); 
	equal_rule test_rule(std_matcher);
	cout<<test_rule.description()<<endl;

	matcher geted = item->get_matcher(std_matcher.name());
    cout<<"std_matcher:"<<std_matcher.name()<<" value:" << std_matcher.value()<<endl;
    cout<<"geted      :"<<geted.name()<<" value:" << geted.value()<<endl;
	CPPUNIT_ASSERT(std_matcher == geted ); 
	CPPUNIT_ASSERT( test_rule.is_true(item));
}/*}}}*/

void
dimension_rule_testcase::rule_args_not_equal(log_item::shared_ptr item)
{/*{{{*/
	int_dimension dim("%ft3721",14);
	matcher std_matcher(&dim);  
	equal_rule test_rule(std_matcher);
	cout<<test_rule.description()<<endl;

	matcher geted = item->get_matcher(std_matcher.name());
	cout<<"std_matcher:"<<std_matcher.name()<<" value:" << std_matcher.value()<<endl;
	cout<<"geted      :"<<geted.name()<<" value:" << geted.value()<<endl;
	CPPUNIT_ASSERT(std_matcher != geted ); 
	CPPUNIT_ASSERT( !test_rule.is_true(item));
}/*}}}*/

void
dimension_rule_testcase::rule_length_test(log_item::shared_ptr item)
{/*{{{*/
	length dim(100);
	matcher std_matcher(&dim); 
	large_rule test_large(std_matcher);
	less_rule test_less(std_matcher);
	unequal_rule test_unequal(std_matcher);
	equal_rule   test_equal(std_matcher);

	matcher geted = item->get_matcher(std_matcher.name());
	cout<<"std_matcher:"<<std_matcher.name()<<" value:" << std_matcher.value()<<endl;
	cout<<"geted      :"<<geted.name()<<" value:" << geted.value()<<endl;
	CPPUNIT_ASSERT( test_large.is_true(item));
	CPPUNIT_ASSERT( !test_less.is_true(item));
	CPPUNIT_ASSERT( test_unequal.is_true(item));
	CPPUNIT_ASSERT( !test_equal.is_true(item));
}/*}}}*/

void
dimension_rule_testcase::rule_test()
{/*{{{*/
	string data_line("192.168.0.1 - - [02/Apr/2006:00:00:00 +0800] \"GET /forum/portal/frame.html?pid=2201304&u=http%3A%2F%2Fokrest.com%2Fbbs%2Fread.php%3Ftid%3D23897%26fpage%3D1&t=%CD%B5%C5%C4+%D5%E6%CA%B5%C1%CB%D5%E2%B8%F6%CA%C0%BD%E7&bbs=%C3%C0%C3%C0%D0%DD%CF%D0%D0%A1%D6%FE&cid=2349&fp=l&pos=2260_15352&y=2006&md=0331 HTTP/1.1\" 200 1029 \"http://www.demo.com/site/photo/index.html?s=end_red\" \"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\" - 544 1225 \"id%3D32904_1141451365\"");

    args_dimension::add_defined_dim(dimension::shared_ptr(new int_dimension("pid")));
	istringstream sin(data_line);
	
	log_item::shared_ptr  item( new log_item);
    const string log_format = "%h %l %u %t %r %>s %b %{REFERER}I %{USER-AGENT}I %v %I %O %{U}C";
    apache_analyser analyser(log_format);
    analyser.analyse(sin,*item);
	rule_args_equal( item);
	rule_args_not_equal( item);
	rule_length_test( item);
}/*}}}*/
