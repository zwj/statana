#ifndef __APA_ANA_H__
#define __APA_ANA_H__
#include "log_analyser.h" 
#include "base_anar.h" 
#include  <boost/function.hpp>
#include <vector>
#include <sstream>
#include "log_item.h"
namespace logs
{
namespace entity
{
    const int  MAX_LEN = 0xffff;
    typedef boost::tokenizer<boost::char_separator<char> > char_tokenizer;
    class apache_analyser: public log_analyser,  public base_analyser 
    {
       public:
           typedef boost::function3<void, const apache_analyser*, std::istream&, log_item&> mem_fun;
           typedef std::vector<std::pair<std::string, std::pair<mem_fun, bool> > >  fun_vector;
           typedef std::vector<std::pair<std::string, std::string> >  match_result;
           enum ARGS{H, L,U,T,R,S,B,REF, AGENT,V,I,O,COOKIE,ADDI,IGNORE};
           void analyse(std::istream& ins,log_item& item)const ;
           void set_parse_proc(const std::string& type, const std::string& field);
           virtual ~apache_analyser();
           apache_analyser(const std::string& express,char sep=' ',bool opt=true);
       private:

            void set_proc(const std::string& field);
            apache_analyser::ARGS args_type(const std::string& field);
            void analyse_format(const std::string& express);
            static const std::string args_name(const std::string& val);
            void proc_ip(std::istream& ins, log_item& item)const ;
            void proc_time(std::istream& ins, log_item& item) const ;
            void proc_req(std::istream& ins, log_item& item) const ;
            void proc_status(std::istream& ins, log_item& item)const ;
            void proc_len(std::istream& ins, log_item& item)const ;
            void proc_referar(std::istream& ins, log_item& item)const ;
            void proc_agent(std::istream& ins, log_item& item) const;
            void proc_host(std::istream& ins, log_item& item) const;
            void proc_cookie(std::istream& ins, log_item& item) const;
            void proc_addi(std::istream& ins, log_item& item) const ;
            void proc_ignore(std::istream& ins, log_item& item)const ;
            void proc_end(std::istream& ins, log_item& item) const ;
            inline std::string read_field(std::istream& ins)const ;
            inline void ignore_field(std::istream& ins) const
            {/*{{{*/
                char ch;
                std::vector<char> ch_vec;
                for(int i = 0; i< MAX_LEN && !ins.eof(); ++i )
                {
                   ins.get(ch);
                   switch(ch)
                   {
                       case '[' :
                           ch_vec.push_back(ch);
                           break;
                       case ']' :
                           if(!ch_vec.empty() && ch_vec.back() == '[')
                               ch_vec.pop_back();
                           break;
                       case '"' :
                           if(!ch_vec.empty() && ch_vec.back() == '"')
                               ch_vec.pop_back();
                           else
                               ch_vec.push_back(ch);
                           break;
                       default :
                           break;
                   }

                   if(ch_vec.empty() && ch == _sep) break;
                   if(ch == '\n')  
                   {
                       ins.putback(ch);
                       break;
                   }
                }
            }/*}}}*/
            std::string get_url(const std::string& req_str) const ;
            void proc_cookie_imp(const std::string& cookie,log_item& item) const ;
            void set_separator(const std::string& express);
            void get_match_results(const std::string& regex_string, 
                    const char* regex_pattern, match_result& match) const;

            fun_vector _proc_funs;
            char _sep;
            bool _opt;
            static const std::string _s_args_names[];
            static const size_t _dim_args_map[];


    };
}
}
#endif 
