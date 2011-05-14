#include "log_item_test.h"
#include <sstream>
#include <entity/log_item.h>
#include <fstream>
#include <clocale>
#include <cctype>
#include "entity_support.h"
#include <improve/time.h>
#include "entity/apache_anar.h"

using namespace std;
using namespace logs;
using namespace logs::entity;

CPPUNIT_TEST_SUITE_REGISTRATION(log_item_testcase ); 
void 
log_item_testcase::input_usecase()
{/*{{{*/
	string buf("dd04c402,409da028000a3f5e00003ce4");		
	istringstream in(buf);
	in.unsetf(std::ios::dec);
	in.setf(std::ios::hex);
	unsigned int  x;
	in.width(8);
	in>>x;
	//_logger.debugStream()<<"x:"<<x;
}/*}}}*/

void log_item_testcase::apache_constrcut_test()
{/*{{{*/
	string data_line(
"127.0.0.1,-,frank,2002-01-20 13:55:36, GET /apache_pb.gif HTTP/1.0, 200, 2326, http://www.example.com/start.html, Mozilla/4.08 en (Win98; I ;Nav),\n"
"127.0.0.1,-,frank,2002-01-20 13:55:36, GET /apache_pb.gif HTTP/1.0, 200, 2326, http://www.example.com/start.html, Mozilla/4.08 en (Win98; I ;Nav),\n"
);
	istringstream sin(data_line);
    apache_analyser analyser("%h,%l,%u,%t,%r,%>s,%b,%{Referer}i");
	log_item item;
    analyser.analyse(sin,item);
    CPPUNIT_ASSERT(item.get_ip()==ip("127.0.0.1")); 
    CPPUNIT_ASSERT(item.get_len()==length(2326)); 

    string data_line2 = "192.168.0.161,2005-10-10 10:08:28,GET /stat.html?m=150001&p=1007&a=2036&b=1003&oid=146855019&e=MTI3NTgwNzkwOSAtMTA2MjczMTYxNQ== HTTP/1.1,200,-,http://templar-studio.ods.org/test.htm?qh_union=m%3D150001%26p%3D1007%26a%3D2036%26b%3D1003%26oid%3D146855019%26e%3DMTI3NTgwNzkwOSAtMTA2MjczMTYxNQ%253D%253D,-\n";

    args_dimension::add_defined_dim(dimension::shared_ptr(new int_dimension("%p",0)));
    args_dimension::add_defined_dim(dimension::shared_ptr(new int_dimension("%a",0)));
    args_dimension::add_defined_dim(dimension::shared_ptr(new int_dimension("%b",0)));

	istringstream sin2(data_line2);
    apache_analyser analyser1("%h,%t,%r,%>s, ,%{Referer}i,%{addi}n");
	log_item item2;
    analyser1.analyse(sin2,item2);
    CPPUNIT_ASSERT(item2.get_ip()==ip("192.168.0.161")); 
    matcher a_m = item2.get_matcher("%a");
    CPPUNIT_ASSERT(a_m.value()=="2036"); 

    item2.get_matcher("$TIME");
    item2.get_matcher("$IP");

    matcher ref_m = item2.get_matcher("$REF");
    cout<< ref_m.name()<<endl;
    cout<< ref_m.value()<<endl;

    matcher m = item2.get_matcher("&ADDI");
    cout<<m.name()<<endl;
    cout<<m.value()<<endl;
    CPPUNIT_ASSERT(m.value()=="-"); 

    matcher p_m = item2.get_matcher("%p");
    cout<< p_m.value()<<endl;
    cout<< p_m.value()<<endl;
    CPPUNIT_ASSERT(p_m.value()=="1007"); 
}/*}}}*/
void
log_item_testcase::constrcut_test()
{/*{{{*/
	//_logger.infoStream()<<"******** construct_test *******";	
 
    const string log_format = "%h %l %u %t %r %>s %b %{REFERER}I %{USER-AGENT}I %v %I %O %{U}C";
    apache_analyser analyser(log_format);
	string data_line("dd04c402409da028000a3f5e00003ce4/right.htm?argse=new3721&ft3721=9&p=%B7%F0%BD%CC%BD%B2%B4%C8%B1%AF&lang=Z265b965877515#2#2ZgMozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)18l4eb3p09p889&b=2");

	string data_line1("dd04c402409da028000a3f5e00003ce4/left.htm?argse=new3721&ft3721=8&p=%B7%F0%BD%CC%BD%B2%B4%C8%B1%AF&lang=cnZ265b965877515#2#2ZgMozilla/4.0 (compatible; MSIE 6.0; Windows NT 4.0)18l4eb3p09p889&b=2");

	string data_line2("dd04c402409da028000a3f5e00003ce4/left.htm?argse=new3721&ft3721=9&p=%B7%F0%BD%CC%BD%B2%B4%C8%B1%AF&lang=zhZ265b965877515#2#2ZgMozilla/4.0 (compatible; MSIE 6.0; Windows NT 4.0)18l4eb3p09p889&b=2");
    args_dimension::add_defined_dim(dimension::shared_ptr(new str_dimension("%ft3721","")));
    args_dimension::add_defined_dim(dimension::shared_ptr(new str_dimension("%lang","")));
	istringstream sin(data_line);
	log_item item;
    analyser.analyse(sin,item);
	//_logger.infoStream()<<WATCH(item);

	log_item item_2;
	istringstream sin2(data_line1);
    analyser.analyse(sin2,item_2);

	log_item item_3;
	istringstream sin3(data_line2);
    analyser.analyse(sin3,item_3);

	CPPUNIT_ASSERT((item.get_ip())==ip(221,4,196,2));
	matcher ip_m1=item.get_matcher("$IP");
	//_logger.infoStream()<<WATCH(ip_m1.value());
	matcher file_m10=item.get_matcher("$FILE");
	matcher file_m11=item.get_matcher("$FILE");
	matcher file_m20=item_2.get_matcher("$FILE");
	//_logger.infoStream()<<WATCH(file_m10.value()); 
	//_logger.infoStream()<<WATCH(file_m20.value()); 
	CPPUNIT_ASSERT(file_m10 == file_m11);
	CPPUNIT_ASSERT(file_m20 != file_m10); 
	matcher ip_file_m10 = item.get_matcher("$FILE","$IP");
	matcher ip_file_m11 = item.get_matcher("$FILE","$IP");
	//_logger.infoStream()<<WATCH(ip_file_m10.value());
	CPPUNIT_ASSERT(ip_file_m10 == ip_file_m11); 
	CPPUNIT_ASSERT(ip_file_m10 != file_m10); 
	CPPUNIT_ASSERT(ip_file_m10 != ip_m1); 

	matcher arg_m10=item.get_matcher("%ft3721");
	matcher arg_m11=item.get_matcher("%ft3721");

	matcher arg_m13=item.get_matcher("%ft3720");
	matcher arg_m14=item.get_matcher("%ft3722");
	matcher arg_m15=item.get_matcher("%ft3722");

	matcher arg_m20=item_2.get_matcher("%ft3721");

    cout<<WATCH(arg_m10.name())<<","<<WATCH(arg_m10.value())<<endl; 
    cout<<WATCH(arg_m13.name())<<","<<WATCH(arg_m13.value())<<endl; 
    cout<<WATCH(arg_m15.name())<<","<<WATCH(arg_m15.value())<<endl; 
    cout<<WATCH(arg_m20.name())<<","<<WATCH(arg_m20.value())<<endl; 
	//_logger.infoStream()<<WATCH(arg_m10.value()); 
	//_logger.infoStream()<<WATCH(arg_m13.value()); 
	//_logger.infoStream()<<WATCH(arg_m20.value());
	CPPUNIT_ASSERT(arg_m10 == arg_m11);
	CPPUNIT_ASSERT(arg_m13 != arg_m11);
	CPPUNIT_ASSERT(arg_m13 != arg_m14);
	CPPUNIT_ASSERT(arg_m15 == arg_m14);
	CPPUNIT_ASSERT(arg_m20 != arg_m10);

	arg_m13.merge(arg_m10);
	arg_m13.merge(arg_m14);
	arg_m10.merge(arg_m13);


	matcher addi_m10 = item.get_matcher("&g");
	matcher addi_m11 = item.get_matcher("&g");
	matcher addi_m13 = item.get_matcher("&n");
	matcher addi_m14 = item.get_matcher("&n");
	matcher addi_m20 = item_2.get_matcher("&g");
	
	
	//_logger.infoStream()<<WATCH(addi_m10.value()); 
	//_logger.infoStream()<<WATCH(addi_m13.value()); 
	//_logger.infoStream()<<WATCH(addi_m20.value());
    cout<<WATCH(addi_m10.name())<<","<<WATCH(addi_m10.value())<<endl; 
    cout<<WATCH(addi_m11.name())<<","<<WATCH(addi_m11.value())<<endl; 
    cout<<WATCH(addi_m14.name())<<","<<WATCH(addi_m14.value())<<endl; 
    cout<<WATCH(addi_m13.name())<<","<<WATCH(addi_m13.value())<<endl; 
    cout<<WATCH(addi_m20.name())<<","<<WATCH(addi_m20.value())<<endl; 

	CPPUNIT_ASSERT(addi_m10 == addi_m11);
	CPPUNIT_ASSERT(addi_m13 != addi_m11);
	CPPUNIT_ASSERT(addi_m13 == addi_m14);
	CPPUNIT_ASSERT(addi_m20 != addi_m10);

	matcher arg_addi_m10 = item.get_matcher("%ft3721","&g");
	matcher arg_addi_m11 = item.get_matcher("%ft3721","&g");

	//_logger.infoStream()<<WATCH(arg_addi_m11.value());
	CPPUNIT_ASSERT(arg_addi_m10 == arg_addi_m11); 

	matcher arg_lang_m10= item.get_matcher("%lang");
	matcher arg_lang_m11= item.get_matcher("%lang");
	matcher arg_lang_m20= item_2.get_matcher("%lang");
	matcher arg_lang_m21= item_2.get_matcher("%lang");

    cout<<WATCH(arg_lang_m10.name())<<","<<WATCH(arg_lang_m10.value())<<endl; 
    cout<<WATCH(arg_lang_m11.name())<<","<<WATCH(arg_lang_m11.value())<<endl; 
    cout<<WATCH(arg_lang_m20.name())<<","<<WATCH(arg_lang_m20.value())<<endl; 
    cout<<WATCH(arg_lang_m21.name())<<","<<WATCH(arg_lang_m21.value())<<endl; 
	CPPUNIT_ASSERT(arg_lang_m10 == arg_lang_m11); 
	CPPUNIT_ASSERT(arg_lang_m20 != arg_lang_m11); 
	CPPUNIT_ASSERT(arg_lang_m20 == arg_lang_m21); 
	//_logger.infoStream()<<WATCH(arg_lang_m20.name())<<" : "<<arg_lang_m20.value();

	typedef map<matcher,int> unique_map;
	unique_map test_map;
//	test_map.insert(make_pair(file_m10,1));
	//test_map.insert(make_pair(file_m20,1));
	test_map.insert(make_pair(arg_lang_m10,1));
	test_map.insert(make_pair(arg_lang_m20,1));
	CPPUNIT_ASSERT(test_map.find(arg_lang_m10)!=test_map.end()); 
	CPPUNIT_ASSERT(test_map.find(arg_lang_m20)!=test_map.end()); 
	//_logger.infoStream()<<WATCH(test_map.size());
	CPPUNIT_ASSERT(test_map.size()==2); 
}/*}}}*/
void
log_item_testcase::multi_constrcut()
{/*{{{*/
		
	ifstream file("/home/zwj/work/data/access.log");
    const string log_format = "%h %l %u %t %r %>s %b %{REFERER}I %{USER-AGENT}I %v %I %O %{U}C";
    apache_analyser analyser(log_format);

	for(int i=0 ; i<10; i++)
	{
		try
		{
			log_item item;
            analyser.analyse(file,item);
			//_logger.debugStream()<<i;
			//_logger.debugStream()<<item;
		}
		catch(exception& e)
		{
			//_logger.errorStream()<<e.what();
		}
	}
}/*}}}*/

matcher get_from_data(const string& data, const string name)
{/*{{{*/
    const string log_format = "%h %l %u %t %r %>s %b %{REFERER}I %{USER-AGENT}I %v %I %O %{U}C";
    apache_analyser analyser(log_format);
	istringstream sin(data);
	log_item item;
    analyser.analyse(sin,item);
	matcher matcher = item.get_matcher(name);
	return matcher;

}/*}}}*/
void
log_item_testcase::unique_test()
{/*{{{*/
	
	string data_line("dd04c402409da028000a3f5e00003ce4/left.htm?argse=new3721&ft3721=8&p=%B7%F0%BD%CC%BD%B2%B4%C8%B1%AF&lang=Z265b965877515#2#2ZgMozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)18l4eb3p09p889&b=2");
	matcher matcher1 = get_from_data(data_line,"$IP");
	matcher matcher2 = get_from_data(data_line,"$IP");
	CPPUNIT_ASSERT(matcher1 == matcher2); 
}
void /*}}}*/

log_item_testcase::ctype_test()
{/*{{{*/
	string data_line("dd04c402409da028000a3f5e00003ce4/left.htm?argse=new3721&ft3721=8&p=%B7%F0%BD%CC%BD%B2%B4%C8%B1%AF&lang=Z265b965877515#2#2ZgMozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)18l4eb3p09p889&b=2");
	stringstream sin(data_line);
	sin.unsetf(std::ios::dec);
	sin.setf(std::ios::hex);
	int w,x,y,z;
	w = x = y = z = 0;
	sin>>setw(1)>>w >>setw(1)>>x >>setw(1)>>y>>setw(1)>>z;
	cout<<w<<"|"<<x<<"|"<<y<<"|"<<z<<endl;
}/*}}}*/
void
log_item_testcase::performace_test()
{/*{{{*/
    const string log_format = "%h %l %u %t %r %>s %b %{REFERER}I %{USER-AGENT}I %v %I %O %{U}C";
    apache_analyser analyser(log_format);
	improve::performance_time new_action;
	new_action.start();
	for(int i = 0 ; i <1000 ; ++i)
	{
		log_item* ptr = new log_item();	
		delete ptr;
	}
	new_action.end();
	//_logger.infoStream()<<"new action :" <<new_action.duration_ms();

	string data_line("dd04c402409da028000a3f5e00003ce4/right.htm?argse=new3721&ft3721=9&p=%B7%F0%BD%CC%BD%B2%B4%C8%B1%AF&lang=Z265b965877515#2#2ZgMozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)18l4eb3p09p889&b=2");
	improve::performance_time get_action;
	log_item item;
	get_action.start();
	for(int m = 0 ;  m < 1000 ; ++m)
	{
		istringstream sin(data_line);
        analyser.analyse(sin,item);
	}
	get_action.end();
	//_logger.infoStream()<<"get action :" <<get_action.duration_ms();
	improve::performance_time strm_action;
	int x=0;
	strm_action.start();
	for(int n = 0 ;  n < 1000 ; ++n)
	{
		istringstream sin(data_line);
		sin.str();
		++x;
	}
	strm_action.end();
	//_logger.infoStream()<<"stream action :" <<strm_action.duration_ms();

}/*}}}*/
