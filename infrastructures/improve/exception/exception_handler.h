#ifndef __EXCEPTION_HANDLER__
#define __EXCEPTION_HANDLER__

#include <csignal>
#include <exception>
#include <iostream>


namespace improve
{

class signal_exception : public std::exception
{/*{{{*/
public:
    virtual std::string report() const = 0;
};/*}}}*/

class int_signal_exception : public signal_exception
{/*{{{*/
public:
    static int get_signal_number() {return SIGINT;}
    std::string report() const {return "int_signal_exception\n";};
};/*}}}*/

template <class signal_exception_class> class signal_translator
{/*{{{*/
private:
    class singleton_translator
    {
    public:
        singleton_translator()
        {
            signal(signal_exception_class::get_signal_number(), signal_handler);
        }

        static void signal_handler(int)
        {
            throw signal_exception_class();
        }
    };

public:
    signal_translator()
    {
        static singleton_translator translator;
    }
};/*}}}*/

class exception_handler
{/*{{{*/
public:
    class singleton_handler
    {
    public:
        singleton_handler()
        {
            std::set_terminate(handler);
        }
        static void handler()
        {
            try
            {
                throw;
            }
            catch (signal_exception& sig_ex)
            {
                std::cout << sig_ex.report().c_str() << std::endl;
            }
        }

    };

public:
    exception_handler()
    {
        static singleton_handler handler;
    }
};/*}}}*/

}

#endif
