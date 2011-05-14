#include "logic/logic_base.h"

namespace logs
{
namespace logic
{
logic_base::logic_base()
:_logger(log4cpp::Category::getInstance(__FUNCTION__))		
{}
}
}
