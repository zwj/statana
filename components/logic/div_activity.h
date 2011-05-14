#ifndef __DIV_ACTIVITY_H__
#define __DIV_ACTIVITY_H__
#include "activity.h" 
namespace logs
{
namespace logic
{

/// 
/// @brief classify activity
class classify_activity: public activity
{/*{{{*/
		
public: 		
	typedef boost::shared_ptr<classify_activity> shared_ptr ;		
	/// 
	/// @brief register classified rule; 
	/// 
	/// @param classify_rule 
	/// 
	void 	register_rule(entity::rule::shared_ptr classify_rule);
	void 	execute(data_board& board);
	classify_activity(const std::string& in, const std::string& out,
                            const std::string& it_name = "classify_activity");	
protected:
private:	
	entity::rule::shared_ptr  	_classify_rule_ptr;
};/*}}}*/
}
}
#endif
