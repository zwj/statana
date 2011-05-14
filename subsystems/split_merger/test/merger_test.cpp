#include <iostream> 
#include <sstream> 
#include <boost/test/unit_test.hpp>
#include "../line_assembler.h"
#include "../merger.h"
#include "../log_def.h"
#include <stdlib.h>
using namespace std;

void split_merger_impl(const char  * model,const char* outname);

BOOST_AUTO_TEST_CASE( test_defcols_analyzer)
{/*{{{*/
    const char* data= "6,150005,2001,58.66.46.127\n"
        "2,150|1005,2001,219.141.95.190\n"
        "3,150003,2001,dkfjqpowurp9quejfaskdfj;alsdkjfa;lsdkfja;sdkfja;slkdfjwpiurepqwekjf;alskjfdp32ijraskdfj;aslkdjf;akjfepqwiejf;asdkjfpqijef;akfdjpqiewjf;akjdfpqwiejf;asdkjf;laskdjfa;sldkjfa;sdkjfpqwieurpasdkjf;sjfdasjfd;pijfepweijfaksdfjpewijfaksdfja;iefpqiewjf;akjfda;skdffffffffffffffffffeiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii2uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuzssseeerr233333333333333333333333333333333333333333333333333333333333333333333333333333wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww8888888888888888888888888888888833333333333333333333333333333a;jf;askjfuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu\n"
        "9,150006,2001,219.141.95.190\n"
        ;
    stringstream ins(data);

    defcols_analyzer  anaer(',',"s(1),2");
    stat_pair::sptr_vector pairs;
    anaer.ana_line(pairs,ins);
    BOOST_CHECK_EQUAL(pairs[0]->value(),6);
    BOOST_CHECK_EQUAL(pairs[0]->key(),"150005,2001");

    pairs.clear();
    anaer.ana_line(pairs,ins);
    BOOST_CHECK_EQUAL(pairs[0]->value(),2);
    BOOST_CHECK_EQUAL(pairs[0]->key(),"150,2001");
    BOOST_CHECK_EQUAL(pairs[1]->value(),2);
    BOOST_CHECK_EQUAL(pairs[1]->key(),"1005,2001");


    pairs.clear();
    anaer.ana_line(pairs,ins);
    BOOST_CHECK_EQUAL(pairs[0]->value(),3);
    BOOST_CHECK_EQUAL(pairs[0]->key(),"150003,2001");



    pairs.clear();
    anaer.ana_line(pairs,ins);
    BOOST_CHECK_EQUAL(pairs[0]->value(),9);
    BOOST_CHECK_EQUAL(pairs[0]->key(),"150006,2001");



    defcols_analyzer  anaer2(',',"1,3");
    stringstream ins2(data);

    pairs.clear();
    anaer2.ana_line(pairs,ins2);
    BOOST_CHECK_EQUAL(pairs[0]->value(),6);
    BOOST_CHECK_EQUAL(pairs[0]->key(),"150005,58.66.46.127");

    pairs.clear();
    anaer2.ana_line(pairs,ins2);
    BOOST_CHECK_EQUAL(pairs[0]->value(),2);
    BOOST_CHECK_EQUAL(pairs[0]->key(),"150|1005,219.141.95.190");

}/*}}}*/


BOOST_AUTO_TEST_CASE(test_mode)
{/*{{{*/
    ivalue_calculator::s_ptr ptr = calculator_parser::parse("\'sum gt 10\'");
    BOOST_CHECK_EQUAL(typeid(*(ptr.get())).name(),  typeid(if_gt_value_sum).name());
    ivalue_calculator::s_ptr ptr1 = calculator_parser::parse("\"sum\"");
    BOOST_CHECK_EQUAL(typeid(*(ptr1.get())).name(),  typeid(value_sum).name());
    ivalue_calculator::s_ptr ptr2 = calculator_parser::parse("count");
    BOOST_CHECK_EQUAL(typeid(*(ptr2.get())).name(),  typeid(value_count).name());
}/*}}}*/
void split_merger_impl(ipair_analyzer* analyzer_ptr ,const char  * model,const char* outname)
{/*{{{*/
    ivalue_calculator::s_ptr calculator_ptr = calculator_parser::parse(model);
    split_merger merger(analyzer_ptr,calculator_ptr.get());
    const char * prj_name  = getenv("LOG_HOME");
    str_vector file_list = analyse_filelist(string(prj_name) + 
                                        "/subsystems/split_merger/test/data/*.stat"); 
    src_fileset src_set(file_list);
    str_vector srcfiles = src_set.get_expanded_file_list();
    stringstream path;
    path <<prj_name <<  "/subsystems/split_merger/test/data/" << outname;
    ofstream outs(path.str().c_str());
    merger.run(src_set, outs);
    outs.close();
}/*}}}*/
BOOST_AUTO_TEST_CASE(test_main)
{/*{{{*/
    defcols_analyzer  anaer(',',"s(1),2",false);
    BasicConfigurator::configure();
    Category& root_logger = Category::getRoot();
    root_logger.setPriority(log4cpp::Priority::DEBUG);
    split_merger_impl(&anaer,"sum gt 5 ", "sum_gt_5.rst");
    split_merger_impl(&anaer,"count gt 5 ", "count_gt_5.rst");
    split_merger_impl(&anaer,"sum", "sum.rst");
    split_merger_impl(&anaer,"count ", "count.rst");

    fullcols_analyzer  anaer2(',');
    split_merger_impl(&anaer2,"sum gt 5 ", "f_sum_gt_5.rst");
    split_merger_impl(&anaer2,"count gt 5 ", "f_count_gt_5.rst");
    split_merger_impl(&anaer2,"sum", "f_sum.rst");
    split_merger_impl(&anaer2,"count ", "f_count.rst");

}/*}}}*/
