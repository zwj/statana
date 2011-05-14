#include <entity/common.h>
#include <entity/utility.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
using namespace std; 
using namespace boost; 
using namespace improve;
std::string
utility::unify(std::string will)
{
    trim(will);
    to_upper(will);
    return will;
}

bool utility::split_pair(const std::string str, std::string& first, 
                         std::string& second,const std::string& separator)
{
    typedef tokenizer<char_separator<char> > char_tokenizer;
    char_separator<char> sep(separator.c_str());
    char_tokenizer tokens(str, sep);
    char_tokenizer::iterator tok_iter = tokens.begin();
    int i =0;
    for (;tok_iter != tokens.end(); ++tok_iter,++i)
    {
        if(i==0) first = *tok_iter;
        if(i==1) second = *tok_iter;
        if(i>=2) break;
    }
    if(i!=2) return false;
    return true;

}


unsigned int
utility::hexstr2int(const string& str)
{
	istringstream iis(str);
	iis.unsetf(std::ios::dec);
	iis.setf(std::ios::hex);
	unsigned int rev=0;
	iis>>rev;
	return rev;
}

