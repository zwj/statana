#ifndef __STAT_ACTIVITY_H__
#define __STAT_ACTIVITY_H__
#include "activity.h"
#include <boost/shared_ptr.hpp>
#include "matcher_geter.h"
namespace logs
{
namespace logic
{

typedef std::vector<std::string> str_vector;
/// 
/// @brief  collect for unique dimension 
/// 
class stat_activity : public activity 
{/*{{{*/
public: 		
	stat_activity(	const std::string&  	in, 
					const std::string&	    path,
					const std::string&	    filename,
					const std::string& it_name = "stat_activity");

	typedef boost::shared_ptr<stat_activity> shared_ptr ;		
	void unique_dimension(const std::string& dimension_name);
	void execute(data_board& board);
private:
    str_vector _args;
	friend class activity_testcase;
};/*}}}*/


}}
#endif
