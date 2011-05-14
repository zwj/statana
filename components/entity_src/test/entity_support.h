#ifndef __ENTITY_SUPPORT_H__
#define __ENTITY_SUPPORT_H__
#include <entity/log_item.h>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/variate_generator.hpp>
typedef boost::minstd_rand base_generator_type;
typedef boost::uniform_smallint<int> distribution_type;
typedef boost::variate_generator<base_generator_type&, distribution_type> gen_type;
namespace logs
{
namespace entity
{
class support
{
public: 		
	support();		
	~support();		
	
	log_item::vector* logitems();	
private:
	void create_entity();
	log_item::vector 	_logitems;

	void create_logitem(gen_type& ip_randomer);
};

}
}
#endif
