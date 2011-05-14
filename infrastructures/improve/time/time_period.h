// time_period.h: interface for the time_period class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIME_PERIOD_H__C2A2F84F_17B2_4772_BDCD_4F50A82020AE__INCLUDED_)
#define AFX_TIME_PERIOD_H__C2A2F84F_17B2_4772_BDCD_4F50A82020AE__INCLUDED_




namespace improve
{
//##ModelId=3E8BD0FB019A
  class time_period  
  {
  public:
	  time_period( unsigned long milliseconds );
	  virtual ~time_period();
	  bool is_expired() {	return ( get_elapsed_time() > _milliseconds ); };
	  unsigned long get_elapsed_time();
	  unsigned long get_left_time() ;
  private:
	  static const unsigned long MAX_ULONG ;
	  unsigned int _init_tickcount;
	  unsigned long _milliseconds;

  };
}

#endif // !defined(AFX_TIME_PERIOD_H__C2A2F84F_17B2_4772_BDCD_4F50A82020AE__INCLUDED_)
