#include "entity/apache_anar.h"
#include "entity/utility.h"
#include "entity/vector_dim.h"
#include "improve/utility/utility.h" 
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/regex.hpp>
using namespace std;
using namespace boost;
using namespace improve;
namespace logs
{
namespace entity
{

    //enum ARGS{H, L,U,T,R,S,B,REF, AGENT};
    //logformat:%h %l %u %t %r %>s %b %{Referer}i %{User-agent}i 
    const size_t args_len = 15;
    const string apache_analyser::_s_args_names[]=
        {"%H","%L","%U","%T","%R","%>S","%B","%{REFERER}I","%{USER-AGENT}I","%V","%I","%O","%{U}C","%{ADDI}N",""};
    const size_t apache_analyser::_dim_args_map[]={0, 3, 4, 4, 4, 4, 5, 6, 7, 8, 9, 12, 13};
    const int  BUF_LEN = 1024; 
    //const int  MAX_LEN = 0xffff;

    apache_analyser::apache_analyser(const string& express,char sep,bool opt)
    :_sep(sep),_opt(opt)
    {/*{{{*/
        analyse_format(express);
        apache_analyser::mem_fun f = apache_analyser::mem_fun(&apache_analyser::proc_end);
        _proc_funs.push_back(make_pair("END", make_pair(f, true)));
    }/*}}}*/
    void 
    apache_analyser::set_proc(const string& field)
    {/*{{{*/
        string flag = utility::unify(field);
        apache_analyser::mem_fun f;
        ARGS arg = args_type(flag);
        switch(arg)
        {
            case apache_analyser::H :
                f = apache_analyser::mem_fun(&apache_analyser::proc_ip);
                break;
            case apache_analyser::L :
                f = apache_analyser::mem_fun(&apache_analyser::proc_ignore);
                break;
            case apache_analyser::U :
                f = apache_analyser::mem_fun(&apache_analyser::proc_ignore);
                break;
            case apache_analyser::T :
                f = apache_analyser::mem_fun(&apache_analyser::proc_time);
                break;
            case apache_analyser::R :
                f = apache_analyser::mem_fun(&apache_analyser::proc_req);
                break;
            case apache_analyser::S :
                f = apache_analyser::mem_fun(&apache_analyser::proc_status);
                break;
            case apache_analyser::B :
                f = apache_analyser::mem_fun(&apache_analyser::proc_len);
                break;
            case apache_analyser::REF :
                f = apache_analyser::mem_fun(&apache_analyser::proc_referar);
                break;
            case apache_analyser::AGENT :
                f = apache_analyser::mem_fun(&apache_analyser::proc_agent);
                break;
            case apache_analyser::V :
                f = apache_analyser::mem_fun(&apache_analyser::proc_host);
                break;
            case apache_analyser::I :
                f = apache_analyser::mem_fun(&apache_analyser::proc_ignore);
                break;
            case apache_analyser::O :
                f = apache_analyser::mem_fun(&apache_analyser::proc_ignore);
                break;
            case apache_analyser::COOKIE :
                f = apache_analyser::mem_fun(&apache_analyser::proc_cookie);
                break;
            case apache_analyser::ADDI :
                f = apache_analyser::mem_fun(&apache_analyser::proc_addi);
                break;
            case apache_analyser::IGNORE :
                f = apache_analyser::mem_fun(&apache_analyser::proc_ignore);
                break;
            default:
                f = apache_analyser::mem_fun(&apache_analyser::proc_ignore);
        }
        erase_all(flag,string("%"));
        _proc_funs.push_back(make_pair(flag, make_pair(f, false)));
    }/*}}}*/


    void apache_analyser::set_parse_proc(const std::string& type, const std::string& field)
    {/*{{{*/
        string flag = args_name(utility::unify(field));
        fun_vector::iterator it_pos = _proc_funs.begin();
        for(;it_pos !=_proc_funs.end();++it_pos)
        {
            string name = (*it_pos).first;
            if(name == flag)
            {
                (*it_pos).second.second = true;
//                break;
            }
        }
    }/*}}}*/
    apache_analyser::ARGS apache_analyser::args_type(const string& field)
    {/*{{{*/
        string unify_field = field;
        to_upper(unify_field);
        for(size_t i = 0; i<args_len; ++i)
        {
            if(_s_args_names[i] == unify_field )
            {
                return (apache_analyser::ARGS)i;
            }
        }
        throw runtime_error("no define the field:" + field);
    }/*}}}*/
    void apache_analyser::analyse_format(const string& express)
    {/*{{{*/
        //%h,%l,%u,%t,%r,%>s,%b,%{Referer}i,%{User-agent} 
        set_separator(express);
        string str;
        str.push_back(_sep);

        char_separator<char> sep(str.c_str());
        char_tokenizer tokens(express, sep);
        char_tokenizer::iterator tok_iter = tokens.begin();
        for(;tok_iter != tokens.end(); ++tok_iter)
        {
            set_proc(*tok_iter); 
        }
    }/*}}}*/
    void apache_analyser::set_separator(const string& express)
    {/*{{{*/
        unsigned int pos = express.find_last_of("%");
        _sep = express.at(pos-1);
    }/*}}}*/
    apache_analyser::~apache_analyser()
    {/*{{{*/
    }/*}}}*/

    const string apache_analyser::args_name(const string& val)
    {/*{{{*/
        size_t index = _dim_args_map[item_def::col_of(val)];
        string name= _s_args_names[index];
        erase_all(name,string("%"));
        return name;
    }/*}}}*/

    void apache_analyser::proc_end(std::istream& ins, log_item& item) const 
    {/*{{{*/
        ins.ignore(MAX_LEN,'\n');
        ins.peek();
    }/*}}}*/
    
    void apache_analyser::proc_ip(std::istream& ins, log_item& item) const 
    {/*{{{*/
        item._ip = ip(read_field(ins));  
    }/*}}}*/

    void apache_analyser::proc_time(std::istream& ins, log_item& item) const 
    {/*{{{*/
        string time_str = read_field(ins);
        if(time_str[0] == '[')
        {
            vector<string> params;
            time_str.erase(0,1);
            time_str.erase(time_str.find_last_of(" "));

            char_separator<char> sep("/:");
            char_tokenizer tokens(time_str, sep);
            char_tokenizer::iterator tok_iter = tokens.begin();
            for(;tok_iter != tokens.end(); ++tok_iter)
            {
                params.push_back(*tok_iter);
            }
            static string month [] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
            int i = 0;
            for(; i < 12; i++)
            {
                if(month[i] == params[1]) break;
            }
            gregorian::date date(    atoi(params[2].c_str()),
                                     i+1,
                                     atoi(params[0].c_str()));
            posix_time::time_duration td(   atoi(params[3].c_str()),
                                            atoi(params[4].c_str()),
                                            atoi(params[5].c_str()));
            posix_time::ptime p( date, td );
            item._time = time(p);

        }
        else
            item._time = time(time_str);
    }/*}}}*/

    void apache_analyser::proc_ignore(std::istream& ins, log_item& item) const 
    {/*{{{*/
        ignore_field(ins);
    }/*}}}*/

    void apache_analyser::proc_len(std::istream& ins, log_item& item) const 
    {/*{{{*/
        string val = read_field(ins);
        trim(val);
        item._len.set_value(val);
    }/*}}}*/

    void apache_analyser::proc_status(std::istream& ins, log_item& item) const 
    {/*{{{*/
        string val = read_field(ins);
        trim(val);
        item._status.set_value(val);
    }/*}}}*/

    string apache_analyser::read_field(std::istream& ins) const
    {/*{{{*/
        char buf[BUF_LEN];
        memset(buf,0,BUF_LEN);
        char ch;
        std::vector<char> ch_vec;
        for(int i = 0; i< BUF_LEN && !ins.eof(); ++i )
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
           buf[i]=ch;
        }
        return string(buf);
    }/*}}}*/

    string apache_analyser::get_url(const string& req_str) const
    {/*{{{*/

        char_separator<char> sep(" ");
        char_tokenizer tokens(req_str, sep);
        char_tokenizer::iterator tok_iter = tokens.begin();
        int i =0;
        string url="";
        for (;tok_iter != tokens.end(); ++tok_iter,++i)
        {
            if(i==1)
            { 
                url= *tok_iter;
                return url;
            }
        }
        return url;
    }/*}}}*/

    void apache_analyser::proc_req(std::istream& ins, log_item& item) const 
    {/*{{{*/
        string req_str = read_field(ins);
        string url = get_url(req_str);
        proc_url(url,item);
    }/*}}}*/

    void apache_analyser::proc_cookie(std::istream& ins, log_item& item) const 
    {/*{{{*/
        string cookie = read_field(ins);
        char ch = '\"';
        cookie = str_utility::trim(cookie, ch);

        if(cookie != "-") proc_cookie_imp(cookie, item);
    }/*}}}*/

    void apache_analyser::proc_referar(std::istream& ins, log_item& item) const
    {/*{{{*/

        string refer= read_field(ins);
        char ch = '\"';
        refer = str_utility::trim(refer, ch);

        const string tok_pattern("https?://([^/]*)/");
        static regex pattern (tok_pattern);
        match_results<const char*> what;
        if(regex_search(refer.c_str(),what,pattern))
        {
            string value = string(what[1].first,what[1].second);
            to_lower(value); 
            dimension::shared_ptr dim_ptr = 
                dimension::shared_ptr(new str_dimension(item._ref.create_son_name("host"), value));
            item._ref.add_son(dim_ptr);

            string::iterator qust_mark_pos = find(refer.begin(), refer.end(), '?');
            string uri =  string(refer.begin(), qust_mark_pos);
            dim_ptr = dimension::shared_ptr(new str_dimension(item._ref.create_son_name("uri"), uri));
            item._ref.add_son(dim_ptr);
        }

    }/*}}}*/

    void apache_analyser::proc_agent(std::istream& ins, log_item& item) const
    {/*{{{*/
        string agent = read_field(ins);
        char ch = '\"';
        agent = str_utility::trim(agent, ch);
        item._agent = str_dimension(item_def::name_of(item_def::AGENT), agent); 
    }/*}}}*/

    void apache_analyser::proc_host(std::istream& ins, log_item& item) const
    {/*{{{*/
        string host = read_field(ins);
        to_lower(host);
	    item._host = str_dimension(item_def::name_of(item_def::HOST), host);

    }/*}}}*/

    void apache_analyser::proc_addi(std::istream& ins, log_item& item) const
    {/*{{{*/
        static const char* names[]={"1","2","3","4","5","6","7","8","9"};
        string data= read_field(ins);
        if(!item._addition_sptr) 
        {
            item._addition_sptr.reset( new vector_dimension());
        }
        const char* subname = names[item._addition_sptr.get()->count()];
        string son_name = item._addition_sptr->create_son_name(subname);
        item._addition_sptr->add_son(dimension::shared_ptr(  new str_dimension(son_name,data)));
    }/*}}}*/

    void apache_analyser::analyse(std::istream& ins, log_item& item) const
    {/*{{{*/
        fun_vector::const_iterator it_pos = _proc_funs.begin();
        for(;it_pos !=_proc_funs.end();++it_pos)
        {
            bool is_parse = (*it_pos).second.second;
            if(is_parse || !_opt)
            {
                mem_fun f = (*it_pos).second.first;
                f(this,ins,item);
            }
            else
                proc_ignore(ins, item);
            
           
        }
    }/*}}}*/

    void apache_analyser::proc_cookie_imp(const std::string& cookie,log_item& item) const
    {/*{{{*/
        const char* cookie_pattern =     "(?:([a-zA-Z]*)=)([^;\\s]*)";
        const char* sub_cookie_pattern = "(?:([a-zA-Z]*)=)([^:\\s]*)";
        match_result cookies;

        get_match_results(cookie, cookie_pattern, cookies);
        match_result::iterator it_pos = cookies.begin();
        for(; it_pos != cookies.end(); ++it_pos)
        {
            string cookie_name = (*it_pos).first;
            string cookie_value = (*it_pos).second;
            {
                match_result sub_cookie;
                get_match_results(cookie_value, sub_cookie_pattern, sub_cookie);
                match_result::iterator itor = sub_cookie.begin();
                for(; itor != sub_cookie.end(); ++itor)
                {
                    string name = cookie_name + "-" + (*itor).first;
                    string value = (*itor).second;
                    dimension::shared_ptr dim_ptr = 
                        dimension::shared_ptr(new str_dimension(item._cookie.create_son_name(name), value));
                    item._cookie.add_son(dim_ptr);
                }
            }
        }
    }/*}}}*/

    void apache_analyser::get_match_results(const string& regex_string,
                                       const char* regex_pattern, 
                                       match_result& match) const
    {/*{{{*/
        boost::regex pattern(regex_pattern);
        boost::cmatch what;
        const char* begin = regex_string.c_str();
        const char* end = regex_string.c_str() + regex_string.size();
        while(regex_search(begin, end, what, pattern))
        {
            string name = string(what[1].first,what[1].second);
            string value = string(what[2].first,what[2].second);
            match.push_back(make_pair(name, value));

            begin = what[2].second;
        }
    }/*}}}*/

}
}
