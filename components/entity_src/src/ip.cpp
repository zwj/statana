#include <entity/common.h>
#include <entity/ip.h>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <entity/xml_utility.h>
#include <boost/scoped_array.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include "entity/utility.h"
#include <boost/tokenizer.hpp>
using namespace std;
using namespace boost;
using namespace  boost::posix_time;
namespace logs
{
namespace entity
{

ip::ip()
:int_dimension(item_def::name_of(item_def::IP))
{}		
ip::ip(	unsigned char first,unsigned char second,
		unsigned char third,unsigned char forth	)	
:int_dimension(item_def::name_of(item_def::IP))
{
	construct(first,second,third,forth);		
}
ip::ip(const string& str)
:int_dimension(item_def::name_of(item_def::IP))
{/*{{{*/
    str_construct(str);
}/*}}}*/

void ip::str_construct(const string& str)
{/*{{{*/

	typedef tokenizer<char_separator<char> > char_tokenizer;
  	char_separator<char> sep(".");
    char_tokenizer tokens(str, sep);
	char_tokenizer::iterator tok_iter = tokens.begin();
    unsigned char  data[4];
  	for (int i=0 ;tok_iter != tokens.end(); ++tok_iter,++i)
    {
    	data[i]=lexical_cast<short>(*tok_iter);
        if(i>4) throw runtime_error(str+ " constract ip failed");
    }
	construct(data[0], data[1], data[2], data[3] );
}/*}}}*/
void ip::construct(	unsigned char first, unsigned char second,
					unsigned char third, unsigned char forth)
{/*{{{*/
    _imp = first;
    _imp<<=  8;
    _imp+= second;
    _imp<<=  8;
    _imp+=third;
    _imp<<=  8;
	_imp+=forth;
}/*}}}*/
ip::ip( unsigned int value)
:int_dimension(item_def::name_of(item_def::IP),value)
{}
void 
ip::set_value(const string& val)
{
    str_construct(val); 
}
ip&
ip::operator=(const ip& other)
{/*{{{*/
    if(&other == this) return *this;
    _imp = other._imp;
    return *this;
}/*}}}*/
std::string
ip::value() const
{/*{{{*/
	char dot[]=".";	
	stringstream buf;
	buf<<(short)first()<<dot<<(short)second()<<dot
			<<(short)third()<<dot<<(short)forth();
	return buf.str();
}/*}}}*/

unsigned char 
ip::first() const
{/*{{{*/
	unsigned int tmp = _imp;
	tmp >>=24;
	tmp &= 0x0000ff;
	return tmp;	
}/*}}}*/
unsigned char 
ip::second() const
{/*{{{*/
	unsigned int tmp = _imp;
	tmp >>=16;
	tmp &= 0x0000ff;
	return tmp;	
}/*}}}*/
unsigned char 
ip::third() const
{/*{{{*/
	unsigned int tmp = _imp;
	tmp >>=8;
	tmp &= 0x0000ff;
	return tmp;	
}/*}}}*/
unsigned char 
ip::forth() const
{/*{{{*/
	unsigned int tmp = _imp;
	tmp &= 0x0000ff;
	return tmp;	
}/*}}}*/

dimension*
ip::clone() const
{/*{{{*/
	return new ip(_imp);
}/*}}}*/

time
time::now()
{
    return time(second_clock::local_time());
}
time::time()
:_imp(not_a_date_time)
{
}	

time::time(ptime now_time)
{
    gregorian::date now_d = now_time.date();
    time_duration now_td = now_time.time_of_day();
    time_duration td = time_duration(now_td.hours(), 0, 0);
    _imp = ptime(now_d, td);
}

time::time(time_t value)
:_imp(time_from_string(time_t2str(value)))
{/*{{{*/
}/*}}}*/

time::time(const std::string& time_str)
:_imp(time_from_string(time_str))		
{/*{{{*/
}/*}}}*/

void 
time::set_value(const string& val)
{/*{{{*/
    _imp = time_from_string(val);
}/*}}}*/
string 
time::name() const
{/*{{{*/
	return item_def::name_of(item_def::TIME);	
}/*}}}*/
std::string 
time::time_t2str(time_t v)
{/*{{{*/
	const short TIME_BUF_LEN=255;	
	boost::scoped_array<char> buf(new char[TIME_BUF_LEN]);
	memset(buf.get(),0,TIME_BUF_LEN);
	strftime(buf.get(),TIME_BUF_LEN-1,"%Y-%m-%d %H:%M:%S",gmtime(&v));
	return string(buf.get());
}/*}}}*/

std::string 
time::value() const
{/*{{{*/
	return to_simple_string(_imp);					
}/*}}}*/
bool 
time::equal(const dimension& other)const
{/*{{{*/
		
	const time* other_time = dynamic_cast<const time*>(&other);	
	if(0 == other_time) return false;
	return _imp==other_time->_imp;
}/*}}}*/
bool 
time::less(const dimension& other)const 
{/*{{{*/
	const time* other_time = dynamic_cast<const time*>(&other);	
	if(0 == other_time) return false;
	return _imp<other_time->_imp;
}/*}}}*/

time&
time::operator=(const time& other)
{/*{{{*/
    if(&other == this) return *this;
    _imp = other._imp;
    return *this;
}/*}}}*/
dimension*
time::clone() const
{/*{{{*/
	return new time(value());
}/*}}}*/

length::length()
:int_dimension(item_def::name_of(item_def::LEN))
{}	
length::length(unsigned int size)
:int_dimension(item_def::name_of(item_def::LEN),size)
{}

dimension*
length::clone() const
{/*{{{*/
	return new length(_imp);
}/*}}}*/

length&
length::operator=(const length& other)
{/*{{{*/
    if(&other == this) return *this;
    _imp = other._imp;
    return *this;
}/*}}}*/

status_code::status_code()
:int_dimension(item_def::name_of(item_def::STATUS))
{}	
status_code::status_code(unsigned int size)
:int_dimension(item_def::name_of(item_def::STATUS),size)
{}

dimension*
status_code::clone() const
{/*{{{*/
	return new status_code(_imp);
}/*}}}*/

status_code&
status_code::operator=(const status_code& other)
{/*{{{*/
    if(&other == this) return *this;
    _imp = other._imp;
    return *this;
}/*}}}*/

}
}
