#include <logic/common.h>
#include <iostream>
#include "logic/activity.h"
using namespace std;
using namespace log4cpp; 
using namespace logs::entity;
namespace logs
{
namespace logic
{

activity::activity(	const std::string&  in, const std::string& path,const std::string& filename, const std::string& it_name)
:logic_base(),_in_lable(in),_out_path(path),_out_filename(filename),
_name(it_name)
{
	_status_control.add(unexed,exed);
	_status_control.moveable(unexed,exed);
	_status_control.init_status(unexed);
}

activity::activity(	const std::string&  in, const std::string& it_name)
:logic_base(),_in_lable(in),
_name(it_name)
{
	_status_control.add(unexed,exed);
	_status_control.moveable(unexed,exed);
	_status_control.init_status(unexed);
}

const string 
activity::name() const
{
	return _name;
}
bool 
activity::have_exed()
{
	return _status_control.current_status()==exed;
}

const string&
activity::get_inputer()
{
	return _in_lable;
}
const string&
activity::get_out_path()
{
	return _out_path;
}
const string&
activity::get_out_filename()
{
	return _out_filename;
}


}
}
