#ifndef __FILTER_H__
#define __FILTER_H__
#include "entity/dimension_rule.h" 
#include <improve/utility.h>
#include <boost/shared_ptr.hpp>
class filter
{
public: 		
	typedef boost::shared_ptr<filter>	shared_ptr;
	void register_rule(rule::shared_ptr ptr)
	{
		throw improve::no_realization(__FUNCTION__); 	
	}
private:
	rule::shared_ptr _rule_ptr;
};
#endif
