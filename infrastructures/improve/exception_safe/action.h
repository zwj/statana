#if !defined EXCEPTION_SAFE_ACTION 
#define EXCEPTION_SAFE_ACTION
/*
  Name:  exception_safe_machine  acthion.h
  Author:  zuowenjian
  Description: 
  Date: 30-07-02 21:27
  Copyright:  
*/

#include <boost/shared_ptr.hpp>
namespace improve
{
	class action  
	{
	public:
		typedef boost::shared_ptr<action> shared_ptr;
		action(){};
		virtual ~action(){};
		virtual void undo()=0;
	};
}
#endif //!defined EXCEPTION_SAFE_ACTION
