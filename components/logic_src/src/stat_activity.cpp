#include <logic/common.h>
#include <logic/stat_activity.h>
#include <boost/tokenizer.hpp>
#include <entity/utility.h>
#include <algorithm>
#include <queue>
using namespace std;
using namespace boost;
using namespace logs::entity;
namespace logs
{
namespace logic
{

stat_activity::stat_activity(	const string& in, 
								const string& path,
								const string& filename,
								const string& it_name )
:activity(in,path,filename,it_name)
{}

void 
stat_activity::unique_dimension(const std::string& dimension_name)
{/*{{{*/
	REQUIRE(dimension_name.length()>0); 	
    typedef tokenizer<char_separator<char> > ch_tokenizer;
    char_separator<char> sep("+");
    ch_tokenizer tokens(dimension_name, sep);
    ch_tokenizer::iterator pos = tokens.begin();
    for(;pos!= tokens.end();++pos)
    {
        _args.push_back(*pos);	
    }

}/*}}}*/

void
stat_activity::execute(data_board& board)
{/*{{{*/
	outputer::shared_ptr out= board.stat_outputer(_out_path, _out_filename);
	inputer::shared_ptr in = board.classify_inputer(_in_lable);
	inputer::iterator pos = in->begin();
    int count =0;
    int total =0 ;
	for(; pos!= in->end(); ++pos,++total)
	{
		log_item::shared_ptr item_ptr = *pos;
		if(!_args.empty())
		{
            matcher unique_matcher = item_ptr->get_matcher(_args);
			out->output(unique_matcher,item_ptr);
		}
		else
		{
            ++count;
			matcher sum_matcher(str_dimension::SUM.get());
			out->output(sum_matcher,item_ptr);
		}
	}

    if (_logger.isDebugEnabled())
        _logger.debugStream() << "stat logitem count:"<<count<<" totoal:"<<total;
	out->haved_output();
	_status_control.move_to(exed);
}/*}}}*/



}
}
