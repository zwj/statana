#ifndef __LOGIC_BASE_H__
#define __LOGIC_BASE_H__
#include <log4cpp/Category.hh>
#include <boost/shared_ptr.hpp> 

namespace logs
{
namespace logic
{
class logic_base
{
    protected:
	    log4cpp::Category&	_logger;
    public:
        logic_base();
};
}
}
#endif
