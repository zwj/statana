#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <entity/apache_anar.h>
#include <entity/dimension_rule.h>
#include <entity/log_item.h>
#include <entity/ip.h>
#include <entity/str_dimension.h>
#include <entity/args_dimension.h>

using namespace std;
using namespace logs::entity;

void rule_args_equal(log_item::shared_ptr item)
{/*{{{*/
    int_dimension dim("%cid",2349);
	matcher std_matcher(&dim); 
	equal_rule test_rule(std_matcher);
	cout<<test_rule.description()<<endl;

	matcher geted = item->get_matcher(std_matcher.name());
    cout<<"std_matcher:"<<std_matcher.name()<<" value:" << std_matcher.value()<<endl;
    cout<<"geted      :"<<geted.name()<<" value:" << geted.value()<<endl;
    BOOST_CHECK(std_matcher==geted);
	BOOST_CHECK( test_rule.is_true(item));
}/*}}}*/

void rule_args_not_equal(log_item::shared_ptr item)
{/*{{{*/
	int_dimension dim("%cid",14);
	matcher std_matcher(&dim);  
	equal_rule test_rule(std_matcher);
	cout<<test_rule.description()<<endl;

	matcher geted = item->get_matcher(std_matcher.name());
	cout<<"std_matcher:"<<std_matcher.name()<<" value:" << std_matcher.value()<<endl;
	cout<<"geted      :"<<geted.name()<<" value:" << geted.value()<<endl;
	BOOST_CHECK(std_matcher != geted ); 
	BOOST_CHECK( !test_rule.is_true(item));
}/*}}}*/

void rule_length_test(log_item::shared_ptr item)
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
	BOOST_CHECK( test_large.is_true(item));
	BOOST_CHECK( !test_less.is_true(item));
	BOOST_CHECK( test_unequal.is_true(item));
	BOOST_CHECK( !test_equal.is_true(item));
}/*}}}*/

/*
BOOST_AUTO_TEST_CASE(rule_test)
{
	string data_line("192.168.0.1 - - [02/Apr/2006:00:00:00 +0800] \"GET /forum/portal/frame.html?pid=2201304&u=http%3A%2F%2Fokrest.com%2Fbbs%2Fread.php%3Ftid%3D23897%26fpage%3D1&t=%CD%B5%C5%C4+%D5%E6%CA%B5%C1%CB%D5%E2%B8%F6%CA%C0%BD%E7&bbs=%C3%C0%C3%C0%D0%DD%CF%D0%D0%A1%D6%FE&cid=2349&fp=l&pos=2260_15352&y=2006&md=0331 HTTP/1.1\" 200 1029 \"http://www.demo.com/site/photo/index.html?s=end_red\" \"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)\" - 544 1225 \"id%3D32904_1141451365\"");

    args_dimension::add_defined_dim(dimension::shared_ptr(new int_dimension("pid")));
	istringstream sin(data_line);
	
	log_item::shared_ptr  item( new log_item);
    const string log_format = "%h %l %u %t %r %>s %b %{REFERER}I %{USER-AGENT}I %v %I %O %{U}C";
    apache_analyser analyser(log_format);
    analyser.analyse(sin,*item);
//    rule_args_equal( item);
//    rule_args_not_equal( item);
	rule_length_test( item);
}
*/

void apache_analyser_test()
{/*{{{*/
	string data_line(
"127.0.0.1,-,frank,2002-01-20 13:55:36, GET /apache_pb.gif HTTP/1.0, 200, 2326, http://www.example.com/start.html, Mozilla/4.08 en (Win98; I ;Nav),\n"
"127.0.0.1,-,frank,2002-01-20 13:55:36, GET /apache_pb.gif HTTP/1.0, 200, 2326, http://www.example.com/start.html, Mozilla/4.08 en (Win98; I ;Nav),\n"
);
	istringstream sin(data_line);
    apache_analyser analyser("%h,%l,%u,%t,%r,%>s,%b,%{Referer}i",',',false);
	log_item item;
    analyser.analyse(sin,item);
    cout << item.get_ip().value()<<endl;
    BOOST_CHECK(item.get_ip()==ip("127.0.0.1")); 
    BOOST_CHECK(item.get_len()==length(2326)); 

    string data_line2 = "192.168.0.161,2005-10-10 10:08:28,GET /stat.html?m=150001&p=1007&a=2036&b=1003&oid=146855019&e=MTI3NTgwNzkwOSAtMTA2MjczMTYxNQ== HTTP/1.1,200,-,http://templar-studio.ods.org/test.htm?qh_union=m%3D150001%26p%3D1007%26a%3D2036%26b%3D1003%26oid%3D146855019%26e%3DMTI3NTgwNzkwOSAtMTA2MjczMTYxNQ%253D%253D,-\n";

    args_dimension::add_defined_dim(dimension::shared_ptr(new int_dimension("%p",0)));
    args_dimension::add_defined_dim(dimension::shared_ptr(new int_dimension("%a",0)));
    args_dimension::add_defined_dim(dimension::shared_ptr(new int_dimension("%b",0)));

	istringstream sin2(data_line2);
    apache_analyser analyser1("%h,%t,%r,%>s, ,%{Referer}i,%{addi}n",',',false);
	log_item item2;
    analyser1.analyse(sin2,item2);
    BOOST_CHECK(item2.get_ip()==ip("192.168.0.161")); 
    matcher a_m = item2.get_matcher("%a");
    BOOST_CHECK(a_m.value()=="2036"); 

    item2.get_matcher("$TIME");
    item2.get_matcher("$IP");

    matcher ref_m = item2.get_matcher("$REF");
    cout<< ref_m.name()<<endl;
    cout<< ref_m.value()<<endl;

    matcher m = item2.get_matcher("$ADDI.1");
    cout<<m.name()<<endl;
    cout<<m.value()<<endl;
    BOOST_CHECK(m.value()=="-"); 

    matcher p_m = item2.get_matcher("%p");
    cout<< p_m.value()<<endl;
    cout<< p_m.value()<<endl;
    BOOST_CHECK(p_m.value()=="1007"); 

    string data_line3 = "219.133.186.145 - - [07/Jul/2008:12:59:59 +0800] \"GET /rd?_u=aHR0cDovL3d3dy4xN2suY29tL2luZGV4MDQzMC5zaHRtbA==&_t=1215053475&_f=0&p=13&a=88&m=1018&ft=1&_s=ee88f25533deff2599ae86a2455537db HTTP/1.1\" 302 100 \"-\" \"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.1.4322)\" 545 636 \"B=ID=508351214372231:V=2:S=82c34ca164;UT=-\" SU\n";
        
    apache_analyser analyser3("%h %l %u %t %r %>s %b %{REFERER}I %{USER-AGENT}I %I %O %{U}C %{addi}n",' ',false);
	istringstream sin3(data_line3);
	log_item item3;
    analyser3.analyse(sin3,item3);

    matcher n = item3.get_matcher("$ADDI.1");
    cout<<n.name()<<endl;
    cout<<n.value()<<endl;
    BOOST_CHECK(n.value()=="SU"); 

}/*}}}*/

BOOST_AUTO_TEST_CASE(test_apache_analyser)
{
    apache_analyser_test();
}
