#include <entity/common.h>
#include <entity/null_dimension.h>
using namespace std; 
namespace logs
{
namespace entity
{

null_dimension::null_dimension(string dim_name) :_name(dim_name) {} string null_dimension::name() const { return _name; }

string
null_dimension::value() const
{
	return "\\null";
}

dimension*
null_dimension::clone() const 
{
	return  new null_dimension(_name);
}

bool
null_dimension::equal(const dimension& other) const
{
	const null_dimension* other_ptr = 
		dynamic_cast< const null_dimension* >(&other);
	if( 0 == other_ptr) return false;
	return _name == other_ptr->_name;
}

bool
null_dimension::less(const dimension& other) const
{
	//return false;	
	const null_dimension* other_ptr = 
		dynamic_cast< const null_dimension* >(&other);
	if( 0 == other_ptr) return false;
	return _name < other_ptr->_name;

}
}
}
