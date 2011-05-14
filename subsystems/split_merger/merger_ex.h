#ifndef __MERGER_EX_H__
#define __MERGER_EX_H__
#include <boost/shared_ptr.hpp>

class ivalue_calculator
{/*{{{*/
    public:
        typedef  boost::shared_ptr<ivalue_calculator> s_ptr;
        virtual  int calcu(int first,int accumulate)=0;
        virtual  ~ivalue_calculator(){}
};/*}}}*/
class value_sum : public  ivalue_calculator
{/*{{{*/
    public:
        virtual  int calcu(int first,int accumulate)
        {
            return first + accumulate;
        }

};/*}}}*/
class value_count: public  ivalue_calculator
{/*{{{*/
    virtual  int calcu(int first,int accumulate)
    {
        return ++accumulate;
    }
};/*}}}*/

class if_gt_value_sum: public  ivalue_calculator
{/*{{{*/
    public:
        if_gt_value_sum(int base)
            :_base(base)
        {

        } 
        virtual  int calcu(int first,int accumulate)
        {
            if(first>=  _base)
                return first + accumulate;
            else 
                return accumulate;
        }
    private:
        int _base;
};/*}}}*/

class if_gt_value_count: public  ivalue_calculator
{/*{{{*/
    public:
        if_gt_value_count(int base)
            :_base(base)
        {

        } 
        virtual  int calcu(int first,int accumulate)
        {
            if(first>=  _base)
                return ++accumulate;
            else 
                return accumulate;
        }
    private:
        int _base;
};/*}}}*/

class calculator_parser
{/*{{{*/
    public:
        static ivalue_calculator::s_ptr parse(const char * mode);
};/*}}}*/
#endif
