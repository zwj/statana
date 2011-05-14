#include <entity/common.h>
#include <entity/str_dimension.h>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_array.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
using namespace std;

namespace logs
{
namespace entity
{

str_dimension::shared_ptr str_dimension::SUM(new str_dimension("SUM","SUM"));
str_dimension::str_dimension()
:_imp(""),_name("")		
{}
		
str_dimension::str_dimension(const string& name ,const string& value)
:_imp(value),_name(name)		
{
    this->_v_hash_code = cal_hash_code(value);
    this->_n_hash_code = cal_hash_code(name);
}
str_dimension::str_dimension(const std::string& name, const std::string& value,
                             unsigned int n_hash_code , unsigned int v_hash_code)
:_imp(value),_name(name),_n_hash_code(n_hash_code),_v_hash_code(v_hash_code)		
{}
void 
str_dimension::set_value(const string& val)
{
    _imp =val;
    this->_v_hash_code = cal_hash_code(val);
}
string 
str_dimension::value() const
{
	return	_imp;
}
string
str_dimension::name() const
{
	return _name;
}
bool 
str_dimension::equal(const dimension& other)const
{
	const str_dimension* other_str = dynamic_cast<const str_dimension*>(&other);	
	if( 0 == other_str ) return false;
    if( _n_hash_code != other_str->_n_hash_code || _v_hash_code != other_str->_v_hash_code)
    {
        return false;
    }
	return _name== other_str->_name && _imp==other_str->_imp;
}
bool 
str_dimension::less(const dimension& other)const 
{
	const str_dimension* other_str = dynamic_cast<const str_dimension*>(&other);	
	if( 0 == other_str ) return false;
	return _imp<other_str->_imp;
}
const str_dimension& 
str_dimension::operator=(const str_dimension& other)
{
	_imp = other._imp;
	_name = other._name;
    _n_hash_code = other._n_hash_code;
    _v_hash_code = other._v_hash_code;

	return *this;	
}	
dimension*
str_dimension::clone() const
{
	return new str_dimension(_name,_imp,_n_hash_code,_v_hash_code);
}

unsigned int
str_dimension::cal_hash_code(const string& str)
{
    unsigned int hash = 0;
//    unsigned int g;
    string::const_iterator itor = str.begin();

    for(; itor != str.end(); itor++)
    {
/*
        hash = (hash << 4) + *itor;
        if ((g = (hash & 0xF0000000)))
        {
            hash = hash ^ (g >> 24);
            hash = hash ^ g;
        }
*/
        hash = 31 * hash + *itor;
    }
    return hash;

}

dimension::smap null_str_dimension::_global_nullstrs;
null_str_dimension::null_str_dimension(const string& the_name)
:str_dimension(the_name,"\\NULL")
{}

empty_str_dimension::empty_str_dimension(const string& the_name)
:str_dimension(the_name,"\\EMPTY")
{}

dimension::shared_ptr 
null_str_dimension::get(const string& the_name)
{
    dimension::smap::iterator pos =  _global_nullstrs.find(the_name);
    if(pos ==  _global_nullstrs.end())
    {
        dimension::shared_ptr new_dim =  dimension::shared_ptr(new null_str_dimension(the_name));
        _global_nullstrs.insert(make_pair(the_name,new_dim));
        return new_dim;
    }
    return pos->second;
}

}
}
