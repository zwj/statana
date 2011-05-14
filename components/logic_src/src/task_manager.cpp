#include <logic/common.h>/*{{{*/
#include "logic/task.h"
#include "logic/task_manager.h"
#include <boost/lexical_cast.hpp> 
#include <boost/regex.hpp>
#include <entity/utility.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>
#include <logic/config_error.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
//#include <strstream>
#include <boost/algorithm/string.hpp>
#include <entity/apache_anar.h>
#include <entity/utility.h>
#include <improve/time.h>
#include <unistd.h>
#include <log4cpp/Category.hh>/*}}}*/
#include <glob.h>

using namespace boost;
using namespace std;
using namespace boost::posix_time; 
using namespace boost::filesystem;
using namespace logs::entity;
using namespace log4cpp;
namespace logs
{	
namespace logic
{

namespace lable
{/*{{{*/
	static const std::string source("source");
	static const std::string argvar("argsvar");
	static const std::string task("task");
	static const std::string activity("activity") ;
	static const std::string count("count");
	static const std::string path("path");
	static const std::string file("file");
	static const std::string format("logformat");
	static const std::string analyser("analyser");
    static const string apache_analyser("APACHE");
	static const std::string logvar("logvar");

}/*}}}*/

const int  BUF_LEN = 1024;

task_manager::task_manager(const string& out_dir)
:_begin_pos(0),_proc_count(0),/*{{{*/
_proc_pos(0),		
_file_pos(0),		
_is_crash(0),		
_current_task_id(1)
{
    _out_path=out_dir;
	create_directory(_out_path);
}/*}}}*/

task_manager::~task_manager()
{
    unlink(".file");
    unlink(".filelist");
}

bool
task_manager::is_plan_ready(const string& plan_name)
{/*{{{*/
	filesystem::path file_path(plan_name);
	return (filesystem::exists(file_path) && !filesystem::is_directory(file_path));	
}/*}}}*/
void
task_manager::set_override_datasrc(const str_vector& src_files)
{/*{{{*/
    _src_file_patterns.clear();
    _src_file_patterns.insert(_src_file_patterns.begin(), src_files.begin(), src_files.end());
}/*}}}*/
void
task_manager::strip_comment(string& line)
{/*{{{*/
    bool in_double_quote = false;
    bool in_single_quote = false;
    bool escaped = false;
    for (size_t i=0 ; i<line.length() ; ++i)
    {
        char ch = line[i];
        switch (ch)
        {
        case '\'': in_single_quote = !in_single_quote; break;
        case '\"': in_double_quote = !in_double_quote; break;
        case '\\': escaped = !escaped; break;
        case '#':
            if (in_single_quote || in_double_quote || escaped)
                continue;
            else
                line.erase(i, line.length() - i);
                return;
            break;
        default:
            if (escaped)
                escaped = false;
            break;
        }
    }
}/*}}}*/
void 
task_manager::load_plan(const string& plan_name, bool is_crash)
{/*{{{*/
    fstream plan_file(plan_name.c_str(),fstream::in);
    _is_crash = is_crash;
    if(!plan_file)
    {
        config_error error(_WHERE);
        error.stream()<<"config file load error file:["<<plan_name<<"]"<<endl;
        throw error;
    }
    string line;
    int len=1024;
    char buf[len];
    while(plan_file)
    {
        memset(buf,len,0);
        plan_file.getline(buf,len);
        line +=string(buf);
        strip_comment(line);
        if(line.rfind(';') != string::npos)
        {
            string sec_name= get_section_name(line);
            _plan_items.insert(make_pair(utility::unify(sec_name),line));
            if(_logger.isDebugEnabled())
                _logger.debugStream()<<"sec_name:["<<sec_name<<"]"<<line;
            line="";
        }
    }
    init_plan();
    
}/*}}}*/
log_analyser::shared_ptr
task_manager::create_analyser(const std::string& analyser,
                                const std::string& format)
{/*{{{*/
    if (lable::apache_analyser== analyser)
    {
        return log_analyser::shared_ptr(new apache_analyser(format));
    }
    throw runtime_error("unkown analyser type--"+analyser);
}/*}}}*/
string 
task_manager::get_sec_value(const std::string& sec_name,const std::string& express)
{/*{{{*/

    const string val_pattern("[^:]+:(.+);?");
	static regex pattern (val_pattern);
	match_results<const char*> what;
	if(regex_search(express.c_str(),what,pattern))
    {
       return 	string(what[1].first,what[1].second);
    }
	config_error error(_WHERE);
    error.stream()<<" get section value error! express:"<<express;
    throw error;
}/*}}}*/
void 
task_manager::init_plan()
{/*{{{*/
	load_datasource();
    string express = get_sec_express(lable::task);
    get_task_setting(express);
    express = get_sec_express(lable::argvar);
    setting_argsvar(express);
    string format_exp = get_sec_express(lable::format);
    string format_value =  get_sec_value(lable::format,format_exp);
    string anar_exp = get_sec_express(lable::analyser);
    string anar_value =  get_sec_value(lable::analyser,anar_exp);
    log_analyser::shared_ptr one = create_analyser(utility::unify(anar_value),format_value);
    express = get_sec_express(lable::logvar);
    setting_logvar(one, express);
    _acty_factory.set_analyser(one);

    if(_is_crash) recover_from_crash();
}/*}}}*/
string
task_manager::get_argsvar_express(const string& express)
{/*{{{*/
    const string task_pattern("argsvar\\s*:\\s*(.+);?");
	static regex pattern (task_pattern);
	match_results<const char*> what;
	if(regex_search(express.c_str(),what,pattern))
    {
       string argsvar = 	string(what[1].first,what[1].second);
       trim(argsvar);
       return  argsvar;
    }
	config_error error(_WHERE);
    error.stream()<<"get argsvar error express:"<<express;	
	throw error;
}/*}}}*/

string
task_manager::get_logvar_express(const string& express)
{/*{{{*/
    const string task_pattern("logvar\\s*:\\s*(.+);?");
	static regex pattern (task_pattern);
	match_results<const char*> what;
	if(regex_search(express.c_str(),what,pattern))
    {
       string logvar = 	string(what[1].first,what[1].second);
       trim(logvar);
       return  logvar;
    }
	config_error error(_WHERE);
    error.stream()<<"get logvar error express:"<<express;	
	throw error;
}/*}}}*/

void 
task_manager::setting_argsvar(const string& express)
{/*{{{*/
    string argsvar = get_argsvar_express(express);/*{{{*/
	typedef tokenizer<char_separator<char> > char_tokenizer;
  	char_separator<char> sep(",");
    char_tokenizer tokens(argsvar, sep);
	char_tokenizer::iterator tok_iter = tokens.begin();
  	for (int i=0 ;tok_iter != tokens.end(); ++tok_iter,++i)
    {
        string first,second;
        if(utility::split_pair(*tok_iter,first,second," "))
        {
            dimension::shared_ptr defult_dim = _dim_factory.create_default_dim(first,second);
            args_dimension::add_defined_dim(defult_dim);
        }
        else
        {
            config_error error(_WHERE);
            error.stream()<<"analyse argsvar fair  error fair:"<<*tok_iter;	
            throw error;
        }
    }/*}}}*/
}/*}}}*/

void 
task_manager::setting_logvar(log_analyser::shared_ptr analyser, const string& express)
{/*{{{*/
    string argsvar = get_logvar_express(express);/*{{{*/
	typedef tokenizer<char_separator<char> > char_tokenizer;
  	char_separator<char> sep(",");
    char_tokenizer tokens(argsvar, sep);
	char_tokenizer::iterator tok_iter = tokens.begin();
  	for (int i=0 ;tok_iter != tokens.end(); ++tok_iter,++i)
    {
        string first,second;
        if(utility::split_pair(*tok_iter,first,second," "))
        {
            analyser->set_parse_proc(first, second);
        }
        else
        {
            config_error error(_WHERE);
            error.stream()<<"analyse logvar fair  error fair:"<<*tok_iter;	
            throw error;
        }
    }/*}}}*/
}/*}}}*/

void 
task_manager::get_task_setting(const string& express)
{/*{{{*/
    const string task_pattern("task\\s*:\\s*(\\d+)");
	static regex pattern (task_pattern);
	match_results<const char*> what;
	if(regex_search(express.c_str(),what,pattern))
    {
       string count = 	string(what[1].first,what[1].second);
        _proc_count= boost::lexical_cast<int>(count);
        return ;
    }
	config_error error(_WHERE);
    error.stream()<<" analyse task  error! express:"<<express;
    throw error;
}/*}}}*/

string
task_manager::get_sec_express(const string& sec_name)
{/*{{{*/
    string name = utility::unify(sec_name);
    string express = _plan_items[name];
    erase_all(express, string(";"));
    return express;
}/*}}}*/
string 
task_manager::get_section_name(const string& line)
{/*{{{*/
    const string rex_pattern("\\s*([^:]+)\\s*:.+");
	static regex pattern (rex_pattern);
	match_results<const char*> what;
	if(regex_search(line.c_str(),what,pattern))
    { 
	    return utility::unify(string(what[1].first,what[1].second));
    }
	config_error error(_WHERE);
    error.stream()<<" analyse section name error! express:"<<line;
    throw error;
}/*}}}*/
void
task_manager::load_datasource()
{/*{{{*/
    string path_pattern;

    // read source pattern from configuration file
    string express = get_sec_express(lable::source);

    // make compatible for old style "path:file" expression
    const string rex_pattern_compatible("^\\s*source\\s*:\\s*(.+)\\s*:\\s*(.+)$");
    static regex pattern_compatible(rex_pattern_compatible);
    express = regex_replace(express, pattern_compatible, "source:\\1\\2");

    const string rex_pattern("^\\s*source\\s*:\\s*(.+)$");
    static regex pattern(rex_pattern);
    match_results<const char*> what;
    if(regex_search(express.c_str(), what, pattern))
    { 
        path_pattern = string(what[1].first,what[1].second);
        trim(path_pattern);
    }

    get_src_files(path_pattern);
}/*}}}*/

task::shared_ptr 
task_manager::create_task()
{/*{{{*/

    REQUIRE(_istream);
	task::shared_ptr cur_task(new task(create_task_id()));
    if(_is_crash && _proc_pos == 1)
    {
        _istream->seekg(_file_pos);
        _is_crash = false;
    }
    _acty_factory.set_load_param(_istream,_proc_count);
    strstrmap::iterator it_pos = _plan_items.begin();
    for(;it_pos!=_plan_items.end();++it_pos)
    {
        string sec_name = it_pos->first;
        if(sec_name.find('@')!= string::npos)
        {
            activity::shared_ptr 
                    cur_acty= _acty_factory.create_acty(it_pos->second,_out_path);		
            cur_task->register_activity(cur_acty);	
        }
    }
    ofstream file(".file", ios::trunc);
    file << "position: " << _istream->tellg() << endl;
    return cur_task;
}/*}}}*/

bool
task_manager::have_task()
{/*{{{*/
	return  (_istream && _istream->good()) ||
			(_files.size() >0 && (_files.size() > _proc_pos ) );
}/*}}}*/

bool
task_manager::recover_from_crash()
{/*{{{*/
    _files.clear();

    ifstream file(".file", ios::in);
    ifstream file_list(".filelist", ios::in);
    if(!file.good() || !file_list.good())
        return false;

    char buf[BUF_LEN];
    while(!file_list.eof())
    {
        file_list.getline(buf, BUF_LEN);
        const string file_name = buf;
        if(!file_name.empty())
        {
            _files.push_back(file_name);
        }
    }
    file.getline(buf, BUF_LEN);

    const string val_pattern("^position:\\s(.+)");
	static regex pattern (val_pattern);
	match_results<const char*> what;
	if(regex_search(buf,what,pattern))
    {
        _file_pos = boost::lexical_cast<int>(string(what[1].first,what[1].second));
    }

    return true;
}/*}}}*/

task::shared_ptr 
task_manager::assign_task()
{/*{{{*/
	if(_files.size()>0 && !(_istream && _istream->good()))	
	{
		proc_next_file();
	}
	return create_task();
}/*}}}*/
void
task_manager::proc_next_file()
{/*{{{*/
	REQUIRE(_files.size()>0);
	REQUIRE(_proc_pos<= _files.size());	
	_proc_pos++;
	string file_name =  _files[_proc_pos-1];	
    Category& perf_logger= Category::getInstance("performance"); 
    perf_logger.infoStream()<<"begin file:"<<file_name;
	_istream = istream_sptr(new ifstream(file_name.c_str()));	

	ofstream file_list(".filelist", ios::trunc);
    for(size_t i = _proc_pos-1; i < _files.size(); i++)
    {
        file_list << _files[i] << endl;
    }
}/*}}}*/


string
task_manager::trans2regex(string& express)
{/*{{{*/
	unsigned int pos = express.find('.');
	if(pos != string::npos)
		express.replace(pos,1,"\\.");
	pos = express.find('*');
	if(pos != string::npos)
		express.replace(pos,1,".*");
	return express;
}/*}}}*/

void
task_manager::get_src_files(const string& path_pattern)
{/*{{{*/
    using namespace boost::filesystem;

    _src_file_patterns.push_back(path_pattern);
    for (size_t i=0 ; i<_src_file_patterns.size() ; ++i)
    {
        glob_t glob_result;
        glob(_src_file_patterns[i].c_str(), 0, NULL, &glob_result);
        for (size_t j=0 ; j<glob_result.gl_pathc; ++j)
        {
            string path_str(glob_result.gl_pathv[j]);
            path file_path(path_str);
            if (is_directory(file_path) || !exists(file_path))
                continue;
            if (find(_files.begin(), _files.end(), path_str) != _files.end())
                continue;
            cout<<path_str<<endl;
            _files.push_back(path_str);
        }
        globfree(&glob_result);
    }

    if (_files.size() == 0)
    {
        config_error error(_WHERE);
        error.stream()<<"ERROR: expanded source file patterns point to no valid files!";
        throw error;
    }

    /*
    cout<<"process following files: "<<endl;
    for (size_t i=0 ; i<_files.size() ; ++i)
    {
        cout<<_files[i]<<endl;
    }
    cout.flush();
    */
}/*}}}*/

unsigned int 
task_manager::create_task_id()
{
	return _current_task_id++;	
}

}
}

