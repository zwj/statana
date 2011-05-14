#include "dimension_test.h" 
#include <entity/dimension.h>
#include <entity/ip.h>
#include <entity/str_dimension.h>
#include <entity/args_dimension.h>
#include <entity/str_dimension.h>
#include <entity/addition_dim.h>
#include <sstream>
#include <entity/dimension_factory.h>
CPPUNIT_TEST_SUITE_REGISTRATION(dimension_testcase); 
using namespace std;
dimension_testcase::dimension_testcase()
//:_logger(Category::getRoot())		
{
}
void
dimension_testcase::ip_test()
{/*{{{*/
	using namespace logs::entity;
	ip one(192,168,0,1);	
	CPPUNIT_ASSERT(one.first()==192); 
	CPPUNIT_ASSERT(one.second()==168); 
	CPPUNIT_ASSERT(one.third()==0); 
	CPPUNIT_ASSERT(one.forth()==1); 
	ip two(192,168,0,2);	
	string one_v = one.value();
	//_logger.debugStream()<<one_v;
	//_logger.debugStream()<<two.value();
	
	CPPUNIT_ASSERT(one_v == string("192.168.0.1")); 
	CPPUNIT_ASSERT(one<two); 		
	CPPUNIT_ASSERT(!(one>two)); 		
	
	ip three(192,168,0,2);	
	CPPUNIT_ASSERT(two==three); 
	ip forth("192.168.0.2");
	CPPUNIT_ASSERT(two==forth); 
	dimension::shared_ptr cloned(three.clone());
	CPPUNIT_ASSERT(*cloned == three ); 
}/*}}}*/

void 
dimension_testcase::time_test()
{/*{{{*/
	time_t now_t;
	time(&now_t);
	cout<<logs::entity::time(now_t).value();
	
	string data_line("409da028");
	stringstream sin(data_line);
	int timev=0;
	sin.unsetf(std::ios::dec);
	sin.setf(std::ios::hex);
	sin >> timev;
	cout<< "time:"<<timev;
	logs::entity::time one(timev);
	cout<< one.value()<<endl;

	logs::entity::time three("2004-5-6 0:0:00");
    
	logs::entity::time forth("2004-5-6 0:0:0");
    cout<<"********"<<endl;
	cout<<three.value()<< " " << forth.value()<<endl;
	CPPUNIT_ASSERT(three== forth); 

	logs::entity::time time1("2004-5-7 0:0:0");
    
	logs::entity::time time2("2004-5-6 0:0:0");

	logs::entity::time time3("2005-06-07 0:0:0");
	CPPUNIT_ASSERT(time1>time2); 
	CPPUNIT_ASSERT(time2<time1); 
}/*}}}*/

void
dimension_testcase::args_test()
{/*{{{*/
	using namespace logs::entity;
    args_dimension::add_defined_dim(dimension::shared_ptr(new str_dimension("%type","")));
    args_dimension::add_defined_dim(dimension::shared_ptr(new str_dimension("%coagent","")));
	string args_line("coagent=msn_sa&type=z&act=info&name=%66%6f%78%77%77%68&cvar1=_main");		
	args_dimension one(args_line);
	cout<< one.value();

	string args_line1("type=z");
	args_dimension two(args_line1);
	CPPUNIT_ASSERT((one != two)); 

	args_dimension three("coagent=msn_sa");
	CPPUNIT_ASSERT((one != three)); 

	args_dimension four("coagent=msn_sa");
	CPPUNIT_ASSERT((four == three)); 

	dimension::shared_ptr cloned(one.clone());
	CPPUNIT_ASSERT(*cloned == one); 
}/*}}}*/
void 
dimension_testcase::addi_test()
{/*{{{*/
	using namespace logs::entity;
	cout<<"***addi_test***"<<endl;
	addition_dimension addi;
	addition_dimension::COL col;
	string value;
	if(addition_dimension::parse("gMozilla",col,value))
	{
		addi.add_dimension(col,value);	
	}
	if(addition_dimension::parse("z265",col,value))
	{
		addi.add_dimension(col,value);	
	}

	cout << addi.value()<<endl;
	addition_dimension one("&z","265");
	cout << one.value()<<endl;
	CPPUNIT_ASSERT(addi != one); 
	addition_dimension two("&z","265");
	CPPUNIT_ASSERT(two == one ); 
	dimension::shared_ptr cloned(addi.clone());
	CPPUNIT_ASSERT(*cloned == addi); 

}/*}}}*/
void 
dimension_testcase::factory_test()
{/*{{{*/
	using namespace logs::entity;
	string src="%ff  ";	
	
	dimension_factory factory;
	dimension::shared_ptr dim=factory.create_dimension(src,"1");
	cout<<endl;
	cout<<dim->name();
	cout<<dim->value()<<endl;
	dimension::shared_ptr  one(new args_dimension("ff=1")) ;
	dimension::shared_ptr  two(new args_dimension("ff=1")) ;
	cout<< one->name();
	cout << one->value()<<endl;
	CPPUNIT_ASSERT(*one == * two); 
	string addi_src="&z ";
	dimension::shared_ptr addi= factory.create_dimension(addi_src,"1");
	dimension::shared_ptr addi_one(new addition_dimension("&z","1"));
	dimension::shared_ptr addi_two(new addition_dimension("&z","1"));
	CPPUNIT_ASSERT(*addi_one == * addi_two); 

}/*}}}*/
void
dimension_testcase::str_dim_test()
{/*{{{*/
	using namespace logs::entity;
    str_dimension str_1("A","1");
    str_dimension str_2("A","1");
    str_dimension str_3("A","2");
    str_dimension str_4("B","2");
    CPPUNIT_ASSERT(str_1==str_2);
    CPPUNIT_ASSERT(str_1!=str_3);
    CPPUNIT_ASSERT(str_4!=str_3);
    CPPUNIT_ASSERT(str_1!=str_3);
}/*}}}*/

