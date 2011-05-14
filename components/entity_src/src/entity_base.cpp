#include "entity/entity_base.h"

namespace logs
{
namespace entity
{
entity_base::entity_base()
:_logger(log4cpp::Category::getInstance(__FUNCTION__))		
{}
}
}
