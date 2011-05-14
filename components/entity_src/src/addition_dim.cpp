#include <entity/common.h>/*{{{*/
#include <entity/addition_dim.h>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_array.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
 /*}}}*/

using namespace boost;
using namespace std;
namespace logs
{
namespace entity
{

const char* COL_NAME[] ={"&1","&2","&3","&4","&5","&6","&7","&8","&9","&a","&b","&c","&d","&e","&f","&g","&h","&M","&m","&n","&o","&p","&R","&r","&s","&t","&w","&x","&Z","&z"};
char addition_init::COL_TABLE[255];
const char FAILED_VAL=255;
addition_init::addition_init()
{/*{{{*/
	memset(COL_TABLE,FAILED_VAL,sizeof(COL_TABLE));	
	COL_TABLE[(int)'1']=addition_dimension::E1;
	COL_TABLE[(int)'2']=addition_dimension::E2;
	COL_TABLE[(int)'3']=addition_dimension::E3;
	COL_TABLE[(int)'4']=addition_dimension::E4;
	COL_TABLE[(int)'5']=addition_dimension::E5;
	COL_TABLE[(int)'6']=addition_dimension::E6;
	COL_TABLE[(int)'7']=addition_dimension::E7;
	COL_TABLE[(int)'8']=addition_dimension::E8;
	COL_TABLE[(int)'9']=addition_dimension::E9;
	COL_TABLE[(int)'a']=addition_dimension::Ea;
	COL_TABLE[(int)'b']=addition_dimension::Eb;
	COL_TABLE[(int)'c']=addition_dimension::Ec;
	COL_TABLE[(int)'d']=addition_dimension::Ed;
	COL_TABLE[(int)'e']=addition_dimension::Ee;
	COL_TABLE[(int)'f']=addition_dimension::Ef;
	COL_TABLE[(int)'g']=addition_dimension::Eg;
	COL_TABLE[(int)'h']=addition_dimension::Eh;
	COL_TABLE[(int)'M']=addition_dimension::EM;
	COL_TABLE[(int)'m']=addition_dimension::Em;
	COL_TABLE[(int)'n']=addition_dimension::En;
	COL_TABLE[(int)'o']=addition_dimension::Eo;
	COL_TABLE[(int)'p']=addition_dimension::Ep;
	COL_TABLE[(int)'R']=addition_dimension::ER;
	COL_TABLE[(int)'r']=addition_dimension::Er;
	COL_TABLE[(int)'s']=addition_dimension::Es;
	COL_TABLE[(int)'t']=addition_dimension::Et;
	COL_TABLE[(int)'w']=addition_dimension::Ew;
	COL_TABLE[(int)'x']=addition_dimension::Ex;
	COL_TABLE[(int)'Z']=addition_dimension::EZ;
	COL_TABLE[(int)'z']=addition_dimension::Ez;
}/*}}}*/

addition_dimension::COL
addition_init::index(char c)
{
	int index_v = *(COL_TABLE+ c );
	REQUIRE(index_v!=FAILED_VAL); 		
	return (addition_dimension::COL)index_v;
}		
addition_dimension::COL
addition_init::index(const string& name)
{
	char ch;
	bool ret=index_c(name,ch);
	REQUIRE(ret); 	
	return index(ch);
}

bool 
addition_init::index_c(const std::string& name,char& ch)
{

	if(name.empty()) return false;	
    string cur_name = name;
    erase_all(cur_name,item_def::name_of(item_def::ADDI));
	if(cur_name=="") return false;
	ch = cur_name[0];
	return true;
	
}
bool 
addition_init::is_define(char c)
{
	int index_v =*(COL_TABLE + c);
	return index_v >= addition_dimension::E1 &&
			index_v <= addition_dimension::Ez;
}
bool addition_init::is_define(const string& name) 
{ 
	char ch; 
	if(index_c(name,ch)) 
	{ 
		return is_define(ch);	
	} 
	return false; 
} 
static addition_init auto_init;

addition_dimension::addition_dimension(
				addition_dimension::COL index, 
				const string& value)
:_dim_array(Ez+1)
{
	string son_name =create_son_name(COL_NAME[index]);
	_dim_array[index].reset(new str_dimension(son_name,value));	
}
addition_dimension::addition_dimension()
:_dim_array(Ez+1)
{} 

addition_dimension::addition_dimension(	const string& name, 
										const string& value)
:_dim_array(Ez+1)
{
	COL index = addition_init::index(name);
	string son_name =create_son_name(COL_NAME[index]);
	_dim_array[index].reset(new str_dimension(son_name,value));	
}	

void 
addition_dimension::add_dimension(
				addition_dimension::COL index,
				const string& value)
{
	REQUIRE(index <= Ez && index >= E1); 	
//	REQUIRE(_dim_array[index].get()==0); 	
//	have the same addition in log item,not use assert;

	string son_name =create_son_name(COL_NAME[index]);
	_dim_array[index].reset(new str_dimension(son_name,value));	
}
string 
addition_dimension::name() const
{
	return item_def::name_of(item_def::ADDI)+"*";	
}
string
addition_dimension::value() const 
{/*{{{*/
	stringstream buf;	
	dim_vector::const_iterator pos = _dim_array.begin();
	for(; pos!= _dim_array.end(); ++ pos)
	{
		dimension::shared_ptr dim =*pos;	
		if(dim) buf<<dim->value()<<",";
				
	}
	return buf.str();
}/*}}}*/

bool 
addition_dimension::parse(const std::string& name_value,
				addition_dimension::COL& col, 
				string& value)
{/*{{{*/
	if(name_value.length()<2) return false;
    string symbol =item_def::name_of(item_def::ADDI);
    string name = name_value;
    erase_all(name,symbol);
	char index_c = name_value[0];
	if(!addition_init::is_define(index_c)) return false;
	col = addition_init::index(index_c);
	value = name_value.c_str()+1;
	return true;
}/*}}}*/

bool 
addition_dimension::true_equal(const dimension& other) const
{/*{{{*/
	const addition_dimension* 
		other_ptr = dynamic_cast< const addition_dimension* >(&other);
	if( 0 == other_ptr ) return false;
	const addition_dimension& other_addi = * other_ptr;
	REQUIRE(_dim_array.size()== other_addi._dim_array.size()); 	
	dim_vector::const_iterator pos = _dim_array.begin();
	dim_vector::const_iterator other_pos = other_addi._dim_array.begin();
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
addition_dimension::in(const dimension& other) const 
{/*{{{*/
	const addition_dimension* 
		other_ptr = dynamic_cast< const addition_dimension* >(&other);
	if( 0 == other_ptr ) return false;
	const addition_dimension& other_addi = * other_ptr;

	REQUIRE(_dim_array.size()== other_addi._dim_array.size()); 	
	dim_vector::const_iterator pos = _dim_array.begin();
	dim_vector::const_iterator other_pos = other_addi._dim_array.begin();
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
addition_dimension::equal(const dimension& other)const
{
	return true_equal(other);
}
bool
addition_dimension::less(const dimension& other)const 
{
	throw improve::no_realization(__FUNCTION__); 	
}
bool 
addition_dimension::have_son(const string& name) const 
{/*{{{*/
	if(name.empty()) return false;	
	
	if(addition_init::is_define(name))	
	{
		return _dim_array[addition_init::index(name)].get() !=0;	
	}
	return false;
}/*}}}*/

const dimension*
addition_dimension::son_dim(const string& name)
{/*{{{*/
	REQUIRE(have_son(name));
	dimension::shared_ptr dim = _dim_array[addition_init::index(name)];
	ENSURE(dim.get()!=0); 
	return dim.get();
}/*}}}*/

string
addition_dimension::create_son_name(const string& name)
{
	string son_name=item_def::name_of(item_def::ADDI)+"." + name;	
    return son_name;
}		
const addition_dimension&
addition_dimension::operator=(const addition_dimension& other)
{
		
	_dim_array=other._dim_array;	
	return *this;
}
addition_dimension::addition_dimension(const addition_dimension& other)
:_dim_array(other._dim_array)
{
}
dimension*
addition_dimension::clone()const 
{
	return new addition_dimension(*this);
}

void
addition_dimension::add_son(dimension::shared_ptr dim)
{
    assert(false); //Œ¥ µœ÷
}
}
}
