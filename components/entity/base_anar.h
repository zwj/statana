#ifndef __BASE_ANA_H_
#define __BASE_ANA_H_
#include "common.h"
#include "log_item.h"
#include "entity_base.h"
namespace logs
{
namespace entity
{
    class   base_analyser : public entity_base 
    {
       public:
           void proc_url(const std::string& url,log_item& item) const ;
    };
}
}
#endif
