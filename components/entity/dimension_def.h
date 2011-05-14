#ifndef _DIMENSION_DEF_H 
#define _DIMENSION_DEF_H 
#include "common.h"
#include "dimension.h"
#include <map>
#include <vector>
namespace logs
{
namespace entity
{

	typedef std::map<std::string,dimension::shared_ptr> dim_map;	
	typedef std::vector<dimension::shared_ptr>   dim_vector;	
	typedef boost::shared_ptr<std::map<std::string,dimension::shared_ptr> > dim_map_sptr;
}
}
#endif
