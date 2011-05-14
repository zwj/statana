#include <entity/common.h>/*{{{*/
#include <entity/group_dim.h>
#include <entity/vector_dim.h>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
 /*}}}*/

using namespace boost;
using namespace std;
namespace logs
{
namespace entity
{
 /*}}}*/


vector_dimension::vector_dimension()
:_basename(item_def::name_of(item_def::ADDI))
{
}

vector_dimension::vector_dimension(const std::string& name)
:_basename(name)
{
}
int vector_dimension::count()
{
    return _dim_array.size();
}

vector_dimension::vector_dimension(const vector_dimension& other)
:_dim_array(other._dim_array)
{
}

std::string 
vector_dimension::create_son_name(const std::string& name)
{
	string son_name=_basename+"."+name;	
	return son_name;
}
void
vector_dimension::set_basename(const std::string& basename)
{
    _basename = basename;
}

string 
vector_dimension::name() const
{
	return item_def::name_of(item_def::ADDI)+"*";	
}
string
vector_dimension::value() const 
{/*{{{*/
	stringstream buf;	
	dim_vector::const_iterator pos = _dim_array.begin();
	for(; pos!= _dim_array.end(); ++ pos)
	{
		dimension::shared_ptr dim =*pos;	
		if(dim) buf<<dim->name()<<"="<<dim->value();
	}
	return buf.str();
}/*}}}*/


bool 
vector_dimension::true_equal(const dimension& other) const
{/*{{{*/
	const vector_dimension* 
		other_ptr = dynamic_cast< const vector_dimension* >(&other);
	if( 0 == other_ptr ) return false;

	const vector_dimension& other_vect= * other_ptr;
	REQUIRE(_dim_array.size()== other_vect._dim_array.size()); 	
	dim_vector::const_iterator pos = _dim_array.begin();
	dim_vector::const_iterator other_pos = other_vect._dim_array.begin();
	for(; pos!=_dim_array.end(); ++pos,++other_pos)
	{
		if((*pos).get()!=0 && other_pos->get()!=0)
		{
			if(!( *(*pos) == *(*other_pos))) return false;		
		}
		else if (! (pos->get() ==0 && other_pos->get() == 0) )
		{
			return false;	
		}
	}
	return true;	
}/*}}}*/

bool 
vector_dimension::in(const dimension& other) const 
{/*{{{*/

	const vector_dimension* 
		other_ptr = dynamic_cast< const vector_dimension* >(&other);
	if( 0 == other_ptr ) return false;

	const vector_dimension& other_vect= * other_ptr;
	REQUIRE(_dim_array.size()== other_vect._dim_array.size()); 	
	dim_vector::const_iterator pos = _dim_array.begin();
	dim_vector::const_iterator other_pos = other_vect._dim_array.begin();
	for(; pos!=_dim_array.end(); ++pos,++other_pos)
	{
		if((*pos).get()!=0 && other_pos->get()!=0)
		{
			if(!( *(*pos) == *(*other_pos))) return false;		
		}
	}
	return true;	
}/*}}}*/

bool 
vector_dimension::equal(const dimension& other)const
{
	return true_equal(other);
}
bool
vector_dimension::less(const dimension& other)const 
{
	throw improve::no_realization(__FUNCTION__); 	
}
bool 
vector_dimension::have_son(const string& name) const 
{/*{{{*/
	if(name.empty()) return false;	
    if(_dim_array.empty()) return false;
	
	dim_vector::const_iterator pos = _dim_array.begin();
    for(;pos!=_dim_array.end();++pos)
    {
        dimension* ptr = (*pos).get();
        if(ptr->name()==name)
        {
            return true;
        }
    }
	return false;
}/*}}}*/

const dimension*
vector_dimension::son_dim(const string& name)
{/*{{{*/
	REQUIRE(have_son(name));
	dim_vector::iterator pos = _dim_array.begin();
    for(;pos!=_dim_array.end();++pos)
    {
        dimension* ptr = (*pos).get();
        if(ptr->name()==name)
            return ptr;
    }
    return NULL;
}/*}}}*/

const vector_dimension&
vector_dimension::operator=(const vector_dimension& other)
{
		
	_dim_array=other._dim_array;	
	return *this;
}
void
vector_dimension::add_son(dimension::shared_ptr dim)
{
    _dim_array.push_back(dim);
}
dimension*
vector_dimension::clone()const 
{
	return new vector_dimension(*this);
}


}
}
