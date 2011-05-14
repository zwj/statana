#ifndef __LINE_ASSEMBLER_H__
#define __LINE_ASSEMBLER_H__

#include <fstream>
#include <string.h>
#include "log_def.h"
#include "shared.h"
#include  <boost/shared_ptr.hpp>
#include  <boost/function.hpp>
#include "boost/tuple/tuple.hpp"
#define LINE_MAX_LEN  512
#define FILED_MAX_LEN 128
class stat_pair
{/*{{{*/
    public:
        typedef boost::shared_ptr<stat_pair> s_ptr;
        typedef std::vector< boost::shared_ptr<stat_pair> >  sptr_vector;
        typedef boost::tuple<bool,bool > conf;
        int value()
        {/*{{{*/
            return _value;
        }/*}}}*/
        inline const char * key()
        {/*{{{*/
            return _key.c_str();
        }/*}}}*/
        inline int keylen()
        {/*{{{*/
            return _key.size();
        }/*}}}*/
        inline void set_value(int v)
        {/*{{{*/
            _value = v;
        }/*}}}*/
        stat_pair();
        const char * bind2key(const char* colstr, const char delimiter=',');
        const char * set_key(const char* key);
        stat_pair * clone();
    private:
        int _value;
        std::string _key;



};/*}}}*/
class ipair_analyzer
{/*{{{*/
    public:
        typedef boost::shared_ptr<ipair_analyzer> s_ptr;
        virtual bool ana_line(stat_pair::sptr_vector& pairs,std::istream& ins)=0;
        virtual void ignore2end(std::istream& ins ) =0; 
        virtual char*  ana_content()=0;
        virtual ~ipair_analyzer(){};
};/*}}}*/
class line_reader
{/*{{{*/
    public:
        line_reader(char delimiter,int maxlen=1204*64)
            :_delimiter(delimiter),
            _maxlen(maxlen)
    {}

        inline void ignore2end(std::istream& ins ) 
        {/*{{{*/
            ins.ignore(_maxlen,'\n');
        }/*}}}*/

        inline void ignore_field(std::istream& ins ) 
        {/*{{{*/
            ins.ignore(_maxlen,_delimiter);
        }/*}}}*/

        inline char* read_field(std::istream& ins,char * buf,int buflen) const
        {/*{{{*/
            memset(buf,0,buflen);
            char ch=0;
            for(int i = 0; i< buflen && !ins.eof(); ++i )
            {
                ins.get(ch);
                if( _delimiter == ch )  
                {
                    break;
                }
                if( ch == '\n')
                {
                    ins.putback(ch);
                    break;
                }
                buf[i]=ch;
            }
            return buf;
        }/*}}}*/
        inline char* read2end(std::istream& ins, char * buf ,int buflen) const
        {/*{{{*/
            memset(buf,0,buflen);
            char ch;
            for(int i = 0; i< buflen && !ins.eof(); ++i )
            {
                ins.get(ch);
                if( ch == '\n')
                {
                    return buf;
                }
                buf[i]=ch;
            }
//            ignore2end();
            return buf;
        }/*}}}*/
        const char _delimiter;
        const int  _maxlen;
};/*}}}*/
class defcols_analyzer : public ipair_analyzer
{/*{{{*/
    public:
//        typedef std::vector< boost::tuple<bool,bool> > flag_vector ;
        defcols_analyzer(const char delimiter,const std::string& extrackey,bool icase=true);
        bool ana_line(stat_pair::sptr_vector& pairs,std::istream& ins);
        void ignore2end(std::istream& ins ) ; 
        char*  ana_content();

    private:
/*{{{*/
        void proc_field(stat_pair::conf,std::istream& ins ,stat_pair::sptr_vector &pair);
        void setup_proc_fun(std::string& colstr);

        void set_pairs_bind_field( stat_pair::sptr_vector &pairs,std::string& sub);
        void clone_pairs_bind_field(stat_pair::sptr_vector &cloned_pairs,
                stat_pair::sptr_vector &pairs,std::string& sub);
        std::vector<stat_pair::conf> _field_flags;
        line_reader _reader;
        bool   _case_sensitive;
        char _content[LINE_MAX_LEN];
/*}}}*/
};/*}}}*/

class fullcols_analyzer : public ipair_analyzer
{/*{{{*/
    public:
        fullcols_analyzer(const char delimiter,bool icase=true);
        bool ana_line(stat_pair::sptr_vector& pairs,std::istream& ins);
        void ignore2end(std::istream& ins ) ; 
        char*  ana_content();
        line_reader _reader;
        bool   _case_sensitive;
        char _content[LINE_MAX_LEN];
};/*}}}*/


#endif
