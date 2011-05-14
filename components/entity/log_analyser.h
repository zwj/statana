#ifndef __ANA_H__
#define __ANA_H__
#include "common.h"
#include "dimension.h" 
#include "log_item.h" 
namespace logs
{
namespace entity
{
    class log_analyser
    {
       public:
           typedef boost::shared_ptr<log_analyser> shared_ptr;
       virtual  void analyse(std::istream& ins,log_item& item)const = 0 ;
       virtual  void set_parse_proc(const std::string& type, const std::string& field) = 0 ;
       virtual ~log_analyser(){};
    };
}
}
#endif
