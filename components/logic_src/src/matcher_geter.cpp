
#include <logic/common.h>
#include <logic/matcher_geter.h>
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

static_matcher_geter::static_matcher_geter(const string& name)	
:_name(name)		
{}	
matcher 
static_matcher_geter::get_matcher(log_item& item)		
{
	return item.get_matcher(_name);	
}		

dyn_matcher_geter::dyn_matcher_geter(const string& first, 
									 const string& second)	
{
    _matcher_names.push_back(first);
    _matcher_names.push_back(second);
}	

dyn_matcher_geter::dyn_matcher_geter(const string& first, 
									 const string& second,
                                     const string& third)	
{
    _matcher_names.push_back(first);
    _matcher_names.push_back(second);
    _matcher_names.push_back(third);
}	

dyn_matcher_geter::dyn_matcher_geter(const string& first, 
									 const string& second,
                                     const string& third,
                                     const string& forth)	
{
    _matcher_names.push_back(first);
    _matcher_names.push_back(second);
    _matcher_names.push_back(third);
    _matcher_names.push_back(forth);
}	
matcher 
dyn_matcher_geter::get_matcher(log_item& item)		
{
    int size = _matcher_names.size();
    switch(size)
    {
        case 2:
	        return item.get_matcher(_matcher_names[0],_matcher_names[1]);	
            break;
        case 3:
	        return item.get_matcher(_matcher_names[0],_matcher_names[1],
                                    _matcher_names[2]);	
            break;
        case 4:          
	        return item.get_matcher(_matcher_names[0],_matcher_names[1],
                                    _matcher_names[2],_matcher_names[3]);	
            break;
        default:
            throw runtime_error("dyn_matcher_geter::get_matcher error");
    }
}		

}
}
