#ifndef __DATA_LOADER_H__
#define __DATA_LOADER_H__
#include <entity/dimension.h>
#include <improve/utility.h>
#include <logic/filter.h>
#include <entity/log_item.h> 
class data_loader
{
public: 		
	void load_data()
	{
		throw improve::no_realization(__FUNCTION__); 	
	}
	const logitems_container* source()
	{
		throw improve::no_realization(__FUNCTION__); 	
	}
	void register_filter(filter::shared_ptr filter_ptr)
	{
		throw improve::no_realization(__FUNCTION__); 	
	}	
private:
	//logitems_container  _data;
};

#endif
