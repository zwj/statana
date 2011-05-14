#ifndef _MATCHER_DEF_H_
#define	_MATCHER_DEF_H_ 
#include "matcher_def.h"
#include <map>
namespace logs
{
namespace entity
{
typedef std::pair<matcher,unsigned int > matcher_count;
typedef std::map<matcher,unsigned int > matcher_count_map;
}
}
#endif

