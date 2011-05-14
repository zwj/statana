#include <entity/common.h>
#include <entity/dimension.h>
using namespace std;
namespace logs
{
namespace entity
{

item_def* item_def::_imp_ptr=0;
item_def*
item_def::instance()
{
	if(_imp_ptr!= 0) return _imp_ptr;
	_imp_ptr = new item_def();
	return _imp_ptr;
}
//const string item_def::NAME[] ={"$IP","$TIME","$PATH","$FILE","$URI","$ARGS","$STATUS","$LEN","$REF","$COOKIE","$ADDI"};
const string item_def::NAME[] =
     {"$IP","$TIME","$PATH","$FILE","$URI","%","$STATUS","$LEN","$REF","$AGENT","$HOST","$COOKIE","$ADDI"};
bool 
item_def::have_col(const string& name)
{/*{{{*/
	return instance()->name_col_map.find(name)!= instance()->name_col_map.end();	
}/*}}}*/
string item_def::name_of(item_def::col item_col)
{/*{{{*/
	REQUIRE(item_col>= item_def::IP); 	
	REQUIRE(item_col<= item_def::ADDI); 	
	return instance()->NAME[item_col];
}/*}}}*/
item_def::col item_def::col_of(const string& name)
{/*{{{*/
	REQUIRE(have_col(name)); 		
	return instance()->name_col_map[name];
}/*}}}*/

item_def::item_def()
{/*{{{*/
	name_col_map.insert(make_pair(NAME[IP],IP));	
	name_col_map.insert(make_pair(NAME[TIME],TIME));	
	name_col_map.insert(make_pair(NAME[LEN],LEN));	
	name_col_map.insert(make_pair(NAME[PATH],PATH));	
	name_col_map.insert(make_pair(NAME[FILE],FILE));	
	name_col_map.insert(make_pair(NAME[URI],URI));	
	name_col_map.insert(make_pair(NAME[REF],REF));	
    name_col_map.insert(make_pair(NAME[AGENT],AGENT));
	name_col_map.insert(make_pair(NAME[ARG],ARG));	
	name_col_map.insert(make_pair(NAME[ADDI],ADDI));	
	name_col_map.insert(make_pair(NAME[STATUS],STATUS));	
	name_col_map.insert(make_pair(NAME[COOKIE],COOKIE));	
	name_col_map.insert(make_pair(NAME[HOST],HOST));	
}/*}}}*/

bool operator==(const dimension& left , const dimension& right) 
{/*{{{*/
    return left.equal(right);
}/*}}}*/

bool operator!=(const dimension& left , const dimension& right) 
{/*{{{*/
    return !(left==right);
}/*}}}*/
bool operator<(const dimension& left , const dimension& right)
{/*{{{*/
    return  left.less(right); 
}/*}}}*/

bool operator<=(const dimension& left , const dimension& right)
{/*{{{*/
    return (left<right) || (left==right);
}/*}}}*/

bool operator>(const dimension& left , const dimension& right)
{/*{{{*/
    return !(left<=right);
}/*}}}*/

bool operator>=(const dimension& left , const dimension& right)
{/*{{{*/
    return !(left<right);
}/*}}}*/
}	
}
