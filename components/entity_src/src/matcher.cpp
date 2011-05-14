#include <entity/common.h>
#include <entity/matcher.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/function.hpp>
using namespace std;
namespace logs
{
namespace entity
{
matcher::matcher(const dimension* dim_ptr)	
{/*{{{*/
	REQUIRE(dim_ptr);

	_dim_array.push_back(dim_ptr);
}/*}}}*/


matcher::matcher( 	const dimension* first,
				 	const dimension* second)
{/*{{{*/
	REQUIRE(first);
	REQUIRE(second); 
	_dim_array.push_back(first);
	_dim_array.push_back(second);
}/*}}}*/

matcher::~matcher()
{/*{{{*/
}/*}}}*/

void
matcher::merge(const matcher& other)
{/*{{{*/
    dimension_vector::const_iterator pos = other._dim_array.begin();
	for(; pos!= other._dim_array.end(); ++pos)
	{
		_dim_array.push_back(*pos);
	}
}/*}}}*/

bool operator==( const matcher& left, const matcher& right)
{/*{{{*/
	unsigned int count = left._dim_array.size();	
	if(count != right._dim_array.size())
			return false;
	for(unsigned int i=0 ; i< count ; ++i)
	{
        const dimension* left_ptr  = left._dim_array[i];
        const dimension* right_ptr = right._dim_array[i];
		if( *left_ptr != * right_ptr) return false;
	}
	return true;
}
/*}}}*/

bool operator< (const matcher& left, const matcher& right)
{/*{{{*/
	unsigned int lsize = left._dim_array.size();
	unsigned int rsize = right._dim_array.size();
	if( lsize == rsize && lsize ==0 ) return false;
	if( lsize == 0 ) return true;	
    dimension_vector::const_iterator left_pos = left._dim_array.begin();
    dimension_vector::const_iterator right_pos = right._dim_array.begin();		


	for(;	left_pos  != left._dim_array.end() &&
			right_pos != right._dim_array.end();
			++left_pos, ++right_pos)
	{
            const dimension* left_ptr = *left_pos;
            const dimension* right_ptr = *right_pos;
			if( *left_ptr == * right_ptr) continue;
			return (*left_ptr) < (*right_ptr);
	}
	return false;
}/*}}}*/

bool operator!=( const matcher& left, const matcher& right)
{/*{{{*/
    return !(left==right);
}/*}}}*/
bool operator<=(  const matcher& left, const matcher& right)
{/*{{{*/
    return  (left<right) || (left== right);
}/*}}}*/

bool operator>(  const matcher& left, const matcher& right)
{/*{{{*/
    return !(left<=right);
}/*}}}*/
bool operator>=(  const matcher& left, const matcher& right)
{/*{{{*/
    return !(left<right);
}/*}}}*/
string
matcher::value() const
{/*{{{*/
	unsigned int count = _dim_array.size();	
	stringstream buf;
    const dimension* dim_ptr;

	for(unsigned int i= 0 ; i< count ; ++i)		
	{
		dim_ptr = _dim_array[i];
        if(0==i)
		    buf<<dim_ptr->value();
        else
            buf<<SEPARATOR<<dim_ptr->value();
	}
	return buf.str();
}/*}}}*/

string
matcher::name() const
{/*{{{*/
	unsigned int count = _dim_array.size();	
	stringstream buf;
    const dimension* dim_ptr;
	string lable="";
	for(unsigned int i= 0 ; i< count ; ++i,lable="+")		
	{
		buf<<lable;
		dim_ptr = _dim_array[i];
		buf<<dim_ptr->name(); 
	}
	return buf.str();
}/*}}}*/

}
}
