#ifndef __RULE_EXPRESS__
#define __RULE_EXPRESS__
#include <boost/shared_ptr.hpp>
#include "../dbc.h"
#include <string>
namespace improve
{
		
	/// 
	/// @brief  rule  interface of type  T
	/// 
	template< typename T>
	class rule
	{
	public:		
		typedef boost::shared_ptr< rule > shared_ptr;
		/// 
		/// @brief  compare obj is accord this rule
		/// 
		/// @param obj 
		/// 
		/// @return  ture or flase
		/// 
		virtual bool is_true(const T& obj) const =0;

		/// 
		/// @brief  description this rule;
		/// 
		/// @return 
		/// 
		virtual std::string description() const =0;
		virtual ~rule()
		{}
	};
}
#endif

