#include <improve/time/performance_time.h>
#include <sys/time.h>
namespace improve 
{
    struct performance_time_impl
    {
         struct timeval _tstart;
	 struct timeval _tend;
         struct timezone _tz;
    };
    performance_time::performance_time(void)
    :_impl_ptr(new performance_time_impl)
    {
    }

    performance_time::~performance_time(void)
    {
	delete _impl_ptr;
    }

    void 
    performance_time::start(void)
    {
	gettimeofday(&(_impl_ptr->_tstart), &(_impl_ptr->_tz));
   }
    void 
    performance_time::end(void)
    {
	gettimeofday(&(_impl_ptr->_tend),&(_impl_ptr->_tz));
    }
    unsigned int 
    performance_time::duration_ms(void) const
    {
        double t1, t2;
        t1 =  (double)_impl_ptr->_tstart.tv_sec*1000 + (double)_impl_ptr->_tstart.tv_usec/1000;
        t2 =  (double)_impl_ptr->_tend.tv_sec *1000 + (double)_impl_ptr->_tend.tv_usec/1000;
        return (int)(t2-t1);
    }
}
