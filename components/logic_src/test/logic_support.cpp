#include "logic_support.h" 
#include <entity_src/test/entity_support.h> 
#include <entity/ip.h>
#include <entity/log_item.h>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_smallint.hpp>
using namespace boost;
typedef boost::minstd_rand base_generator_type;
typedef boost::uniform_smallint<base_generator_type> randomer;
using namespace logs::entity;
namespace logs
{
namespace logic
{

test_inputer::test_inputer(log_item::vector* items)
:logitem_container("test"),_items(items),_name("test")
{}

inputer::iterator 
test_inputer::begin()
{
	return _items->begin();
}	
inputer::iterator
test_inputer::end()
{
	return _items->end();
}
bool
test_inputer::ready()
{
	return true;
}
std::string
test_inputer::name()
{
	return _name;		
}

}
}
