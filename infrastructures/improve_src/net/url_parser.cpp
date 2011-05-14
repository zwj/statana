#include <improve/net/url_parser.h>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp> 
using namespace std;	
using namespace boost;
const string url_parse_exp("(\\S+)://(?:(\\S+):(\\S+)@)?([^/:]+)(?::(\\d+))?(\\S*/([^/]+))");
url_parser::url_parser()
:_url_pattern(url_parse_exp)	
{
}
bool is_empty(cmatch::value_type item)
{
	return item.first==item.second;
}

string to_str(cmatch::value_type item)
{
	return string(item.first,item.second);
}

url_info
url_parser::parse(const string& url)
{
	url_info cur_info;
	cmatch what;
    match_flag_type flags = match_default; 
	if(regex_search(url.c_str(),what,_url_pattern,flags))
	{
		if(!is_empty(what[1]))
			cur_info._protocal = to_str(what[1]);
		if(!is_empty(what[2]))
			cur_info._username = to_str(what[2]); 
		if(!is_empty(what[3]))
			cur_info._password = to_str(what[3]); 
		if(!is_empty(what[4]))
			cur_info._hostname = to_str(what[4]); 
		if(!is_empty(what[5]))
			cur_info._port	   = lexical_cast<int>(to_str(what[5]));
		if(!is_empty(what[6])) 
			cur_info._path	   = to_str(what[6]);
		if(!is_empty(what[7]))
			cur_info._filename = to_str(what[7]);
	}
	return cur_info;
}

