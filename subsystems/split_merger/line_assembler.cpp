#include "line_assembler.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include  "log_def.h"
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost;
typedef vector< string > split_vector_type;

///////////////////////////////////////////////////////////////////////////
//

stat_pair::stat_pair()
:_value(0),_key("")
{/*{{{*/
}/*}}}*/

stat_pair *  stat_pair::clone()
{/*{{{*/
    stat_pair * n =  new stat_pair;
    n->_value  = this->_value;
    n->_key    = this->_key;
    return n;
}/*}}}*/

const char * stat_pair::bind2key(const char* colstr,const char delimiter)
{/*{{{*/
    if( _key.size() == 0)
        _key = colstr;
    else
        _key = _key + delimiter + colstr;
    trim(_key); 
    return _key.c_str();
}/*}}}*/

const char * stat_pair::set_key(const char* key)
{/*{{{*/
    _key= key;
    trim(_key); 
    return _key.c_str();
}/*}}}*/

defcols_analyzer::defcols_analyzer(char delimiter,const std::string& extrackey,bool icase)
:_reader(line_reader(delimiter)),_case_sensitive(icase)
{/*{{{*/

    split_vector_type split_vec; 
    split( split_vec, extrackey, is_any_of(",") ); 
    BOOST_FOREACH( string colstr, split_vec)
    {
        setup_proc_fun(colstr);
    }
}/*}}}*/

void    defcols_analyzer::setup_proc_fun(std::string& colstr)
{/*{{{*/


    unsigned int col = 0 ;
    bool     split_key = false;
    regex pattern("s\\(([0-9]+)\\)");	
    match_results<const char*> what;
    if(regex_search(colstr.c_str(),what,pattern))
    {
        string coldata = string(what[1].first,what[1].second);
        col=lexical_cast<unsigned int>(coldata);
        split_key = true;
    }		
    else
    {
        if( colstr.empty()) return ;
        col=lexical_cast<unsigned int>(colstr);
    }

    for(unsigned int i =1 ; i<= col ; ++i)
    {
        if(_field_flags.size() < i)
        {
            if( i == col )
            {
                _field_flags.push_back(make_tuple(true,split_key));
            }
            else
            {
                _field_flags.push_back(make_tuple(false,false));
            }
        }
    }
}/*}}}*/

void defcols_analyzer::clone_pairs_bind_field(stat_pair::sptr_vector &cloned_pairs,
        stat_pair::sptr_vector &pairs,string& sub)
{/*{{{*/
        BOOST_FOREACH(stat_pair::s_ptr pair, pairs)
        {
            stat_pair::s_ptr cloned = stat_pair::s_ptr(pair->clone()) ;
            cloned->bind2key(sub.c_str());
            cloned_pairs.push_back(cloned);
        }
}/*}}}*/

void defcols_analyzer::set_pairs_bind_field( stat_pair::sptr_vector &pairs,string& sub)
{/*{{{*/
        BOOST_FOREACH(stat_pair::s_ptr pair, pairs)
        {
            pair->bind2key(sub.c_str());
        }
}/*}}}*/

void defcols_analyzer::proc_field(stat_pair::conf conf ,std::istream& ins , stat_pair::sptr_vector &pairs)
{/*{{{*/
    bool need_bind  = conf.get<0>();
    bool need_split = conf.get<1>();
    if(need_bind)
    {/*{{{*/
        split_vector_type split_vec; 
        char *col =  _reader.read_field(ins,_content,LINE_MAX_LEN);

        string col_str(col);
        if(!_case_sensitive)
            to_lower(col_str);
        if( need_split)
        {
            split( split_vec, col_str, is_any_of("|") ); 
        }
        else
        {
            split_vec.push_back(col_str);
        }

        stat_pair::sptr_vector  spliteds;
        BOOST_FOREACH(string sub , split_vec)
        {

            BOOST_FOREACH(stat_pair::s_ptr pair, pairs)
            {
                stat_pair::s_ptr cloned = stat_pair::s_ptr(pair->clone()) ;
                cloned->bind2key(sub.c_str());
                spliteds.push_back(cloned);
            }
        }
        pairs  = spliteds ;

    }/*}}}*/
    else
        _reader.ignore_field(ins);
}/*}}}*/

char * defcols_analyzer::ana_content()
{
    return _content;
}
bool defcols_analyzer:: ana_line(stat_pair::sptr_vector& pairs,std::istream& ins)
{/*{{{*/
    char *p = _reader.read_field(ins,_content,LINE_MAX_LEN);
    if( strlen(p) == 0 )
    {
        return false;
    }
    int value = lexical_cast<int>(p);
    stat_pair::s_ptr pair(new stat_pair());
    pair->set_value(value);
    pairs.push_back(pair);
    BOOST_FOREACH( stat_pair::conf conf, _field_flags)
    {
        proc_field(conf,ins,pairs);
    }
    ignore2end(ins);
    return true;
}/*}}}*/

void defcols_analyzer::ignore2end(std::istream& ins ) 
{/*{{{*/
    _reader.ignore2end(ins);
}/*}}}*/


fullcols_analyzer::fullcols_analyzer(const char delimiter,bool icase)
:_reader(line_reader(delimiter)),_case_sensitive(icase)
{}
bool fullcols_analyzer::ana_line(stat_pair::sptr_vector & pairs,std::istream& ins)
{/*{{{*/
    char *p = _reader.read_field(ins,_content,LINE_MAX_LEN);
    if( strlen(p) == 0 ) 
        return false;
    stat_pair::s_ptr pair(new stat_pair);
    pair->set_value( lexical_cast<int>(p));
    _reader.read2end(ins,_content,LINE_MAX_LEN);
    if(!_case_sensitive)
        to_lower(_content);
    pair->set_key(_content);
    pairs.push_back(pair);
    return true;
}/*}}}*/
void fullcols_analyzer::ignore2end(std::istream& ins ) 
{/*{{{*/
    _reader.ignore2end(ins);
}/*}}}*/

char * fullcols_analyzer::ana_content()
{
    return _content;
}
