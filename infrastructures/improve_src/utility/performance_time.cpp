#include "../performance_time.h"
#include <winbase.h>
namespace improve 
{
//##ModelId=3DE5B7060143
    LARGE_INTEGER performance_time::_freq;
//##ModelId=3DE5B706014B
    performance_time::performance_time(void)
    {
    }

//##ModelId=3DE5B706014D
    performance_time::~performance_time(void)
    {
    }

//##ModelId=3DE5B706014F
    void 
    performance_time::start(void)
    {
        static int first = 1;
        if(first) 
        {
            QueryPerformanceFrequency(&_freq);
            first = 0;
        }
        QueryPerformanceCounter(&_tstart);

    }
//##ModelId=3DE5B7060151
    void 
    performance_time::end(void)
    {
        QueryPerformanceCounter(&_tend);

    }
//##ModelId=3DE5B7060156
    unsigned int 
    performance_time::duration_ms(void) const
    {
        return 1000*((double)_tend.QuadPart -
                        (double)_tstart.QuadPart)/((double)_freq.QuadPart);

    }
}
