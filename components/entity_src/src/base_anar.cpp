#include "entity/base_anar.h"
#include "entity/log_item.h"
#include "entity/str_dimension.h"
#include "entity/args_dimension.h"

using namespace std;
using namespace boost;


namespace logs
{
namespace entity
{
void 
base_analyser::proc_url(const std::string& url,log_item& item) const
{
	string::const_iterator qust_mark_pos = find(url.begin(),url.end(),'?');
	string::const_reverse_iterator uri_rbegin(qust_mark_pos);
	string::const_reverse_iterator path_rend = find(uri_rbegin,url.rend(),'/');
	string::const_iterator path_end(path_rend.base());
	item._path =  str_dimension(item_def::name_of(item_def::PATH),string(url.begin(),path_end));

	string::const_iterator  file_beg=  path_end;
	string::const_iterator  args_beg = qust_mark_pos == url.end()? qust_mark_pos : qust_mark_pos + 1;
	item._file = str_dimension(item_def::name_of(item_def::FILE),string(file_beg,qust_mark_pos));
    item._uri = str_dimension(item_def::name_of(item_def::URI),string(url.begin(),qust_mark_pos));
	item._args = args_dimension(string( args_beg,url.end()));
}
}
}
