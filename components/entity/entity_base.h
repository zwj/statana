#ifndef _ENTITY_BASE_H 
#define _ENTITY_BASE_H
#include <log4cpp/Category.hh>
namespace logs
{
namespace entity
{
    class entity_base
    {

        protected:
            log4cpp::Category&	_logger;
        public:
            entity_base();
    };
}
}
#endif
