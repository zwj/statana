#ifndef __INPUT_H__
#define __INPUT_H__

#include <entity/matcher_def.h>
#include <entity/log_item.h>
#include <boost/shared_ptr.hpp> 
#include <improve/status.h>
#include <map>
#include "logic_base.h"
namespace logs
{
namespace logic
{

/// 
/// @brief input of activity 
/// 
class inputer : public logic_base
{
public:
	typedef boost::shared_ptr<inputer> shared_ptr;
	typedef entity::log_item::vector::iterator iterator;
	typedef entity::log_item::vector::const_iterator const_iterator;
	virtual std::string name() =0 ;
	virtual iterator begin()=0;
	virtual iterator end()=0;
	virtual bool ready() =0 ;
	virtual ~inputer(){};
};

/// 
/// @brief output of activity
/// 
class outputer: public logic_base

{
public:	
	typedef boost::shared_ptr<outputer> shared_ptr;
	typedef std::pair<const std::string,shared_ptr> name_sptr_pair;
	typedef std::map<const std::string, shared_ptr> name_map;
	virtual void haved_output()=0;
	virtual void output(const entity::matcher& the_matcher,
						entity::log_item::shared_ptr item )=0;
	virtual void output(const entity::log_item::shared_ptr item )=0;
	virtual ~outputer(){}
	virtual std::string name() = 0 ;
};

/// 
/// @brief a mock inputer for load activity;
///			ready allways is true;
/// 
class load_inputer : public inputer
{
public:	
	load_inputer(const std::string& the_name);
	std::string name();
	iterator begin();
	iterator end();
	bool ready();
private:
	std::string _name;	
};
/// 
/// @brief  implement inputer and outputer interface;
/// 
class logitem_container : public inputer , public outputer 
{
	enum status{unoutput,outputed};
public:
	typedef boost::shared_ptr<logitem_container> shared_ptr ;
	typedef std::pair<const std::string,shared_ptr> name_sptr_pair;
	typedef std::map<const std::string, shared_ptr> name_map;
	logitem_container(const std::string& the_name);
	iterator begin();
	iterator end();
	void haved_output();
	bool ready();
	std::string name();
	void output(const entity::matcher& matcher,
				entity::log_item::shared_ptr item );
	void output(const entity::log_item::shared_ptr item );
	//void clear();
private:	
	entity::log_item::vector _vector;
	std::string 	_name;
	improve::status_machine<status> _status_ctrl;
};

/// 
/// @brief Stat result;
/// 
class stat_result : public outputer
{
public:
	typedef boost::shared_ptr<stat_result> shared_ptr ;
	typedef std::pair<const std::string,shared_ptr> name_sptr_pair;
	typedef std::map<const std::string, shared_ptr> name_map;
	stat_result(const std::string& path,const std::string& the_name);
	void output(const entity::matcher& matcher,
				entity::log_item::shared_ptr item );
	void output(const entity::log_item::shared_ptr item );
	void haved_output();
    
	std::string name();
    std::string result_name();
	unsigned int size();
	unsigned int sum_count();
	void save();
    ~stat_result();

private:
	entity::matcher_count_map	_stat_map;	
	std::string 	_path;
	std::string 	_name;
};
}
}
#endif
