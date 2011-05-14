#include <entity/common.h>/*{{{*/
#include <entity/utility.h>
#include <entity/dimension_def.h>
#include <entity/args_dimension.h>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_array.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
 /*}}}*/

using namespace boost;
using namespace std;
using namespace log4cpp;
namespace logs
{
namespace entity
{

dim_vector args_dimension::_s_def_dims;

void
args_dimension::add_defined_dim(dimension::shared_ptr dim)
{/*{{{*/
    _s_def_dims.push_back(dim);
}/*}}}*/

args_dimension::args_dimension()
:_data_err_logger(Category::getInstance("data_error"))
{/*{{{*/
}/*}}}*/

bool 
args_dimension::get_defined_dim(const string& name, dimension::shared_ptr& dim)
{/*{{{*/
    dim_vector::iterator it_pos = _s_def_dims.begin();
    for(;it_pos!=_s_def_dims.end();++it_pos)
    {
        if(name == (*it_pos)->name())
        {
            dim = *it_pos;
            return true;
        }
    }
    return false;
}/*}}}*/

void 
args_dimension::construct(const string& val_str)
{/*{{{*/
    if( _s_def_dims.empty()) return ;

	typedef tokenizer<char_separator<char> > arg_tokenizer;
  	char_separator<char> sep("&");
    arg_tokenizer tokens(val_str, sep);
	arg_tokenizer::iterator tok_iter = tokens.begin();
  	for (;tok_iter != tokens.end(); ++tok_iter)
  	{
         string name,value;
         dimension::shared_ptr dim;
         if(fetch_pair(*tok_iter,name,value) && get_defined_dim(create_son_name(name),dim))
         {
             dimension::shared_ptr new_dim(dim->clone()); 
            new_dim->set_value(value);
		    _imp[create_son_name(name)]= new_dim;
         }
         else
		 {
			if(_data_err_logger.isErrorEnabled()) 
		    			_data_err_logger.errorStream()<<*tok_iter;
		 }		 
    }
}/*}}}*/

args_dimension::args_dimension(const string& value)
:_data_err_logger(Category::getInstance("data_error"))
{/*{{{*/
    construct(value);
}/*}}}*/

args_dimension::args_dimension( const string& arg_name,
								const string& arg_val)
:_data_err_logger(Category::getInstance("data_error"))
{/*{{{*/
	_imp[arg_name]= str_dimension::shared_ptr(new str_dimension(arg_name,arg_val));
}/*}}}*/

const args_dimension& 
args_dimension::operator=(const args_dimension& other)
{/*{{{*/
	_imp= other._imp;
	return *this;
}/*}}}*/

std::string 
args_dimension::create_son_name(const std::string& name)
{/*{{{*/
	string son_name=item_def::name_of(item_def::ARG) + name;	
	return son_name;
}		/*}}}*/

bool 
args_dimension::fetch_pair(const string& express ,  string& name,string& value)
{/*{{{*/
    if(utility::split_pair(express, name,value,"="))
	{
        trim(value);
        return true;
	}
    return false;

}/*}}}*/

args_dimension::args_dimension( const args_dimension& other)
:_imp(other._imp), _data_err_logger(Category::getInstance("data_error"))
{/*{{{*/
}/*}}}*/

dimension*
args_dimension::clone() const
{/*{{{*/
	return new args_dimension(*this);
}/*}}}*/

string
args_dimension::name() const 
{/*{{{*/
	return item_def::name_of(item_def::ARG)+"*";	
    
}/*}}}*/

string
args_dimension::value() const
{/*{{{*/
	ostringstream buf;
	args_map::const_iterator pos = _imp.begin();
	for(; pos != _imp.end(); ++pos)
	{
        string name= (*pos).first;	
        string value= (*pos).second->value();	
        buf<<name<<"="<<value;
	}
	return buf.str();
}/*}}}*/

void 
args_dimension::set_value(const string& val)
{/*{{{*/
    construct(val);
}/*}}}*/

bool 
args_dimension::true_equal(const dimension& other) const 
{/*{{{*/
	const args_dimension* 
		other_ptr = dynamic_cast< const args_dimension* >(&other);
	if ( 0 == other_ptr) return false;

	const args_dimension& other_args =	*other_ptr;  

	unsigned int count = _imp.size();	
	if(count != other_args._imp.size()) return false;
	
	args_map::const_iterator self_pos= _imp.begin(); 
	for( ; self_pos != _imp.end() ; ++self_pos)
	{
		dimension::shared_ptr 	left   = (*self_pos).second;
		args_map::const_iterator found = other_args._imp.find(left->name());
		if(found == other_args._imp.end()) return false;
		
		dimension::shared_ptr right = (*found).second;
		if(*left != * right) return false;
	}
	return true;
}	/*}}}*/

bool 
args_dimension::in(const dimension& other) const
{/*{{{*/
	args_map const * small;
	args_map const * big;
	const args_dimension* other_ptr  = dynamic_cast< const args_dimension* >(&other);
	if ( 0 == other_ptr ) return false ;
	const args_dimension& other_args = *other_ptr; 
	if( _imp.size() > other_args._imp.size())
	{
		small= &(other_args._imp);	
		big = &_imp;
	}
	else
	{
		big = &(other_args._imp);	
		small= &_imp;
	}
	args_map::const_iterator pos = small->begin();
	for(;pos != small->end(); ++pos)
	{
		dimension::shared_ptr left= (*pos).second;
		args_map::const_iterator found = big->find(left->name());
		if(found == big->end()) return false;
	
		dimension::shared_ptr right = (*found).second;
		if(*left != * right) return false;
	}
	return true;
}/*}}}*/

bool 
args_dimension::equal(const dimension& other) const 
{/*{{{*/
	return true_equal(other);
}	/*}}}*/
bool 
args_dimension::less(const dimension& other) const 
{/*{{{*/
	throw improve::no_realization(__FUNCTION__); 	
}/*}}}*/


bool 
args_dimension::have_son(const std::string& name) const
{/*{{{*/
	return _imp.find(name) != _imp.end();		
}/*}}}*/
const dimension*
args_dimension::son_dim(const std::string& name) 
{/*{{{*/
	return 	_imp[name].get();
}/*}}}*/

void args_dimension::add_son(dimension::shared_ptr dim)
{
    assert(false);
}
int args_dimension::count()
{
    return _s_def_dims.size();
}
}
}
