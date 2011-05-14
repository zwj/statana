#ifndef _EXCEPTION_DEF_
#define _EXCEPTION_DEF_
#include <stdexcept>
namespace improve
{
    namespace excepton_def
    {
        class config_err{};
        class data_err{};
        class input_err{};
        class app_logic_err{};
    }


    template <class T > 
        class self_runtime_error  : public std::runtime_error
    {
        public:
            self_runtime_error(const char* msg)
                :std::runtime_error(msg)
            {
            }
//            T&  err_def()
//            {
//                return T ;
//            }
    };

    typedef self_runtime_error<excepton_def::config_err>   config_exception;
    typedef self_runtime_error<excepton_def::data_err  >   data_exception;
    typedef self_runtime_error<excepton_def::input_err >   input_exception;
    typedef self_runtime_error<excepton_def::app_logic_err >   app_logic_exception;

}
#endif
