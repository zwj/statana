#include <improve/utility/args_analyze.h>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <vector>
	
using namespace std; 
using namespace boost;
namespace improve
{
typedef boost::tokenizer<boost::char_separator<char> > ch_tokenizer;
typedef std::vector<string> str_vector; 
		        
class impl
{
public:
	impl(int argc, char** args)
	{
		string str;
		for(int i=0 ; i< argc ; ++i)
		{
			str += args[i];
			str += " ";
		}
	    boost::char_separator<char> sep("-");
	    ch_tokenizer tokens(str, sep);
		ch_tokenizer::iterator tok_iter = tokens.begin();
		if(tok_iter != tokens.end()) tok_iter++; //first args ignore;
	    for (;tok_iter != tokens.end(); ++tok_iter)
		{
			_args_array.push_back(*tok_iter);
		}
		_pos= _args_array.begin();
	}
	str_vector _args_array;
	str_vector::iterator _pos;
};

args_analyzer::args_analyzer(int argc, char** args)
:_impl_ptr(new impl(argc,args))
{
	
}
bool 
args_analyzer::getopt(char& opt, string& value)
{
	if(_impl_ptr->_pos == _impl_ptr->_args_array.end()) return false;
	match_results<std::string::const_iterator> what; 
	static regex express("(\\S)\\s?(\\S*)");
	if(regex_search(*(_impl_ptr->_pos),what,express))
	{
		opt= string(what[1].first,what[1].second)[0];
		value= string(what[2].first,what[2].second);
		_impl_ptr->_pos++;
		return true;
	}
	return false;
}
args_analyzer::~args_analyzer()
{
	delete _impl_ptr;
}

}
