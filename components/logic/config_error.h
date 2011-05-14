
#include <improve/utility.h>
namespace logs
{
namespace logic
{

/// 
/// @brief  config error;
/// 
class config_error : public improve::rt_exception 
{
public:		
	config_error(const std::string& where)	
	:improve::rt_exception(where)
	{}		
};
}
}
