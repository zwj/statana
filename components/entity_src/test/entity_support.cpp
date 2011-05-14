#include "entity_support.h" 
#include <entity/ip.h>
#include <entity/args_dimension.h>
#include <entity/log_item.h>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_smallint.hpp>
using namespace std;
using namespace boost;
//typedef boost::minstd_rand base_generator_type;
//typedef boost::uniform_smallint<int > randomer;
//
namespace logs
{
    namespace entity
    {	
        support::support()
        {
            create_entity();	
        }

        void support::create_entity()
        {
            base_generator_type generator(42u);
            gen_type die_gen(generator, distribution_type(140, 146));

            //randomer  ip_randomer(140,146);

            for(int i=0 ;i<1000; ++i)
            {
                //int val;
                //ip_randomer(val);
                create_logitem(die_gen);
            }

        }		
        void support::create_logitem(gen_type& die_gen)
        {

            log_item::shared_ptr one(new log_item());
            //ip::shared_ptr one_ip(new ip(192,168,ip_randomer(),ip_randomer()));

            ip one_ip(192,168,140,die_gen());
            time now1 = time::now();
            stringstream buf;
            buf<<"type="<<die_gen();
            args_dimension args(buf.str());
            one->replace(one_ip);
            one->replace(now1);
            one->replace_len(die_gen());
            one->replace(args);

            _logitems.push_back(one);
        }
        log_item::vector* support::logitems()	
        {
            return &_logitems;	
        }
        support::~support()
        {
        }

    }
}
