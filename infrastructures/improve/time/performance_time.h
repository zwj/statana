#ifndef __PERFORMANCE_TIME_H__
#define __PERFORMANCE_TIME_H__

namespace improve 
{
    struct performance_time_impl;
	
	/// 
	/// @brief high precision timer
	/// 
    class performance_time
    {
    public:
        performance_time(void);
        ~performance_time(void);
		/// 
		/// @brief   time start;
		/// 
        void start(void);

		/// 
		/// @brief  time end;
		/// 
        void end(void);
		/// 
		/// @brief used microsecond;
		/// 
		/// @return  microsecond;
		/// 
        unsigned int   duration_ms(void) const; // 返回用时；单位：微秒;
    private:
	 performance_time_impl* _impl_ptr;
     //   LARGE_INTEGER _tstart, _tend;
     //   static LARGE_INTEGER _freq;
        
    };
}
#endif
