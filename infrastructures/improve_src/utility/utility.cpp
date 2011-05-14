#include "improve/utility/utility.h" 
#include <algorithm>

using namespace std;
namespace improve
{
string 
str_utility::trim(string& will, char ch)
{
	if(will.empty()) return will;	

	unsigned int beg = will.find_first_not_of(ch);                                   
 	beg = string::npos == beg ? 0: --beg;                                             
  	unsigned int end = will.find_last_not_of(ch);                                    
 	end = string::npos == end ? will.length()-1 : ++end;                              
 	will.erase(end,(will.length()-end)+1);                                            
 	will.erase(0,beg+1);          
	return will;
}

char my_toupper(const char a)
{
    return toupper(a);
}

char my_tolower(const char a)
{
    return tolower(a);
}
string 
str_utility::upcase(string& need)
{
	transform(need.begin(),need.end(),need.begin(),my_toupper); 	
	return need;
}
string 
str_utility::lowcase(string& need)
{
	transform(need.begin(),need.end(),need.begin(),my_tolower); 	
	return need;
}
rt_exception::rt_exception(const string& description, const string& where)
:runtime_error(description)
{
	_sout<<description<<where;	
}
rt_exception::rt_exception(const ostringstream& sout, const string& where)
:runtime_error("")
{
	_sout<<sout.str()<<where;	
}
rt_exception::rt_exception(const string& where)
:runtime_error("")
{
	_sout<<"where:"<<where<<endl;	
}
rt_exception::rt_exception(const rt_exception& other)
:runtime_error(other)
{
	_sout<<other._sout.str();
}
ostringstream& 
rt_exception::stream()
{
	return _sout;	
}
const char *
rt_exception::what() const throw()
{
	return _sout.str().c_str();
}
rt_exception::~rt_exception() throw()
{}

}
