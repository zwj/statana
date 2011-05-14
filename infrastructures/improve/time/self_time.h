// time.h: interface for the time class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIME_H__18ED8F96_C3FA_4F05_9BA5_BA22F8F814EF__INCLUDED_)
#define AFX_TIME_H__18ED8F96_C3FA_4F05_9BA5_BA22F8F814EF__INCLUDED_

#include <windows.h>
#include <string>
namespace improve
{
//##ModelId=3E8BD0FB017C
  class time  
  {
  public:
	  
    //##ModelId=3E8BD0FB01C3
    static time now();
  
    //##ModelId=3E8BD0FB01CD
    std::string to_simple_str() const;
    //##ModelId=3E8BD0FB01CE
	  virtual ~time();
    //##ModelId=3E8BD0FB01D0
    time(const time& other);
    //##ModelId=3E8BD0FB01D6
    time& operator=(const time& other);
    //##ModelId=3E8BD0FB01D8
    short year() const;
    //##ModelId=3E8BD0FB01DA
    short month() const;
    //##ModelId=3E8BD0FB01DC
    short data() const;
    //##ModelId=3E8BD0FB01E1
    short hour() const;
    //##ModelId=3E8BD0FB01E3
    short minute() const;
    //##ModelId=3E8BD0FB01E5
    short second() const;
    //##ModelId=3E8BD0FB01F4
    short millisecond() const;
  private:
    //##ModelId=3E8BD0FB01C2
    SYSTEMTIME _data;
    //##ModelId=3E8BD0FB01F6
    time(const SYSTEMTIME& set_data);

  };
	
}

#endif // !defined(AFX_TIME_H__18ED8F96_C3FA_4F05_9BA5_BA22F8F814EF__INCLUDED_)
