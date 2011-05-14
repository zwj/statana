#include <entity/common.h>
#include <entity/utility.h>
#include <entity/simple_dim.h>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
namespace logs
{
namespace entity
{

int_dimension::int_dimension(const string& name)	
:_imp(0),_name(name)		
{}
int_dimension::int_dimension(const string& name,unsigned int value)	
:_imp(value),_name(name)		
{}		
int_dimension::~int_dimension()
{}
void
int_dimension::set_value(const string& val)
{/*{{{*/
   _imp = lexical_cast<int>(val);
}/*}}}*/


void 
int_dimension::set_by_hexstr(const std::string& val)
{
    _imp = utility::hexstr2int(val);
}
string 
int_dimension::name() const 
{
    return _name;
}
dimension*
int_dimension::clone() const 
{
    return new int_dimension(_name,_imp);
}
string
int_dimension::value() const
{/*{{{*/
	stringstream buf;
	buf<<_imp;
	return buf.str();
}/*}}}*/
bool
int_dimension::equal(const dimension& other) const
{/*{{{*/
	const int_dimension* other_int = dynamic_cast<const int_dimension*>(&other);
    if(other_int == NULL) return false;
    return _imp == other_int->_imp;
}/*}}}*/
bool 
int_dimension::less(const dimension& other) const
{/*{{{*/
	const int_dimension* other_int = dynamic_cast<const int_dimension*>(&other);	
    if(other_int == NULL) return false;
	return _imp <  other_int->_imp;	
}/*}}}*/

}
}
