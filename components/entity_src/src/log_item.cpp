#include <entity/log_item.h>
#include <entity/utility.h>
#include <string>
#include <boost/regex.hpp>
#include <stdexcept> 
#include <sstream>
#include <boost/tokenizer.hpp>
#include <entity/str_dimension.h>
#include <entity/args_dimension.h>
#include <algorithm>
#include <entity/null_dimension.h>
using namespace std;
using namespace boost;
namespace logs
{
    namespace entity
    {
        log_item::log_item()
        {
            _ref.set_basename(item_def::name_of(item_def::REF));
            _cookie.set_basename(item_def::name_of(item_def::COOKIE));
        }



        ostream& operator<<(ostream& out, const log_item& item)
            {/*{{{*/
                out<<item._ip.name()<<":"<<item._ip.value()<<",";
                out<<item._time.name()<<":"<<item._time.value()<<",";
                out<<item._uri.name()<<":"<<item._uri.value()<<",";
                out<<item._status.name()<<":"<<item._status.value()<<",";
                out<<item._len.name()<<":"<<item._len.value()<<",";
                out<<item._ref.name()<<":"<<item._ref.value()<<",";
                out<<item._agent.name()<<":"<<item._agent.value()<<",";
                out<<item._host.name()<<":"<<item._host.value();
                out<<item._cookie.name()<<":"<<item._cookie.value();
                if(item._addition_sptr)
                    out<<","<<item._addition_sptr->name()<<":"<<item._addition_sptr->value();
                return out;
            }/*}}}*/

        ip log_item::get_ip()
        {/*{{{*/
            return _ip;		
        }/*}}}*/
        length log_item::get_len()
        {
            return _len;
        }
        time log_item::get_time()
        {/*{{{*/
            return _time;		
        }/*}}}*/

        bool log_item::is_args_name(const string& name)
        {/*{{{*/
            return name.find(item_def::name_of(item_def::ARG)) != string::npos;
        }/*}}}*/

        bool log_item::is_addi_name(const string& name)
        {/*{{{*/
            return name.find(item_def::name_of(item_def::ADDI)) != string::npos;
        }/*}}}*/
        matcher log_item::get_matcher(const string& name)
        {/*{{{*/
            string father, son;
            if(is_args_name(name))
            {
                return get_args_matcher(name);	

            }
            else if(is_addi_name(name))
            {
                return get_addi_matcher(name);
            }
            else if(utility::split_pair(name, father,son,"."))
            {
                return get_args_matcher(father, son);
            }
            else
            {
                if(!item_def::have_col(name)) 
                {
                    improve::rt_exception err(_WHERE);
                    err.stream()<<"not found " << name<<" dimension";
                    throw err;
                }
                return matcher(item_dim(item_def::col_of(name)));
            }
        }/*}}}*/


        const dimension* log_item::item_dim(item_def::col item_col)
        {/*{{{*/
            switch(item_col)
            {
                case item_def::IP:	
                    return &_ip;
                case item_def::TIME:	
                    return &_time;
                case item_def::PATH:	
                    return &_path;
                case item_def::FILE:	
                    return &_file;
                case item_def::URI:	
                    return &_uri;
                case item_def::ARG:	
                    return &_args;
                case item_def::STATUS:
                    return &_status;
                case item_def::LEN:	
                    return &_len;
                case item_def::REF:	
                    return &_ref;
                case item_def::AGENT:
                    return &_agent;
                case item_def::HOST:
                    return &_host;
                case item_def::COOKIE:
                    return &_cookie;
                case item_def::ADDI:	
                    return _addition_sptr.get();
                default:	
                    REQUIRE(false); 
                    return NULL;
            }
        }/*}}}*/

        matcher log_item::get_matcher(const string& first, const string& second)
        {/*{{{*/
            matcher first_matcher = get_matcher(first);		
            matcher second_matcher = get_matcher(second);		
            first_matcher.merge(second_matcher);
            return first_matcher;
        }/*}}}*/

        matcher		log_item::get_matcher(const std::string& frist,
                const std::string& second,
                const std::string& thrid)
        {/*{{{*/
            matcher first_matcher = get_matcher(frist,second);
            matcher second_matcher = get_matcher(thrid);
            first_matcher.merge(second_matcher);
            return first_matcher;
        }/*}}}*/

        matcher	log_item::get_matcher(const std::string& frist,
                const std::string& second,
                const std::string& thrid,
                const std::string& forth)
        {/*{{{*/

            matcher first_matcher = get_matcher(frist,second);
            matcher second_matcher = get_matcher(thrid,forth);
            first_matcher.merge(second_matcher);
            return first_matcher;
        }/*}}}*/

        matcher	log_item::get_matcher(const std::vector<std::string> svector)
        {/*{{{*/
            const size_t vsize = svector.size();

            switch(vsize)
            {
                case 1:
                    return get_matcher(svector[0]);
                case 2:
                    return get_matcher(svector[0],svector[1]);
                case 3:
                    return get_matcher(svector[0],svector[1],svector[2]);
                case 4:
                    return get_matcher(svector[0],svector[1],svector[2],svector[3]);
                case 5: 
                    {
                        matcher first = get_matcher(svector[0],svector[1],svector[2]);
                        matcher second= get_matcher(svector[3],svector[4]);
                        first.merge(second);
                        return first;
                    }
                case 6: 
                    {
                        matcher first = get_matcher(svector[0],svector[1],svector[2]);
                        matcher second= get_matcher(svector[3],svector[4],svector[5]);
                        first.merge(second);
                        return first;
                    }
                default:
                    throw runtime_error("not support so much unique dimension! ");

            }

        }/*}}}*/

        matcher		log_item::get_addi_matcher(const std::string& name)	
            {/*{{{*/
                if(_logger.isDebugEnabled()) _logger.debugStream()<<WATCH(name) ;
                if(_addition_sptr && _addition_sptr->have_son(name)) 
                {
                    const dimension* son = _addition_sptr->son_dim(name);
                    return matcher(son);
                }
                else
                {
                    return matcher(null_str_dimension::get(name).get());

                }
            }/*}}}*/


        matcher		log_item::get_args_matcher(const std::string& name)	
            {/*{{{*/
                if(!_args.have_son(name)) 
                    return matcher( null_str_dimension::get(name).get());	
                const dimension* son = _args.son_dim(name);
                return matcher(son);
            }/*}}}*/

        matcher		log_item::get_args_matcher(const std::string& father_name, const std::string& son_name)
            {/*{{{*/
                vector_dimension father(father_name);
                std::string son = father.create_son_name(son_name);
                switch(item_def::col_of(father_name))
                {
                    case item_def::COOKIE :
                        father = _cookie;
                        break;
                    case item_def::REF:
                        father = _ref;
                        break;

                    default:
                        REQUIRE(false);
                }
                if(!father.have_son(son)) 
                    return matcher( null_str_dimension::get(son).get());	
                const dimension* son_dim = father.son_dim(son);
                return matcher(son_dim);
            }/*}}}*/

        void	log_item::replace(const ip& new_ip)
            {/*{{{*/
                _ip = new_ip;	
            }/*}}}*/

        void log_item::replace(const args_dimension& new_args)
            {/*{{{*/
                _args = new_args;	
            }/*}}}*/

        void log_item::replace(const time& new_time)
            {/*{{{*/
                _time = new_time;
            }/*}}}*/

        void log_item::replace_len(int new_len)
            {/*{{{*/
                _len = new_len;
            }/*}}}*/
    }
}
