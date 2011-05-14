#ifndef __TEST_SUPPORT_H__
#define __TEST_SUPPORT_H__
#include <entity/log_item.h>
#include <logic/activity.h>

namespace logs
{
namespace logic
{

 class test_inputer : public logitem_container
 {
 public:
	test_inputer(entity::log_item::vector* items); 
	iterator begin();
	iterator end();
	bool 	ready();
	std::string name();
 private:
	entity::log_item::vector*  _items;
	std::string _name;
 };


}
}
#endif
