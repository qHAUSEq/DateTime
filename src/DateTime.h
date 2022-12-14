#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <cmath>
#include <cstring>
#include <map>
#include <ostream>

namespace DateTime
{
typedef unsigned int uint;

enum SecIn
{
    MINUTE = 60,				/** Qty sec in min				*/
    HOUR = 3600,				/** Qty sec in hour				*/
    DAY = 86400,				/** Qty sec in one day			*/
    MONTH_28DAYS = 2419200,		/** Qty sec in month (28days)	*/
    MONTH_29DAYS = 2505600,		/** Qty sec in month (29days)	*/
    MONTH_30DAYS = 2592000,		/** Qty sec in month (30days)	*/
    MONTH_31DAYS = 2678400,		/** Qty sec in month (31days)	*/
    YEAR = 31536000,			/** Qty sec in year				*/
    YEAR_LEAP = 31622400		/** Qty sec in leap year		*/
};

class Date
{
public:
    Date();
    Date(uint y, uint m, uint d);

    bool isValidD();
    bool leapYear();

    void setYear(uint year);
    void setMonth(uint month);
    void setDay(uint day);
    int getYear();
    int getMonth();
    int getDay();

    static Date CurrentDate();
    uint64_t DateInSec(uint hour = 1, uint min = 1, uint sec = 1);

    // Can be modified and executed using difftime or comparison in seconds
    bool operator > (Date d);
    bool operator < (Date d);
    bool operator == (Date d);
    bool operator != (Date d);
    bool operator >= (Date d);
    bool operator <= (Date d);
    Date operator = (Date d);
    friend std::ostream& operator << (std::ostream& os, Date d);

protected:
    int year;
    int month;
    int day;
    std::map<int, SecIn> numberOfDayInMonth =
    { {1, MONTH_31DAYS},
      {2, (year % 4 == 0 ? MONTH_29DAYS : MONTH_28DAYS)},
      {3, MONTH_31DAYS}, {4, MONTH_30DAYS}, {5, MONTH_31DAYS},
      {6, MONTH_30DAYS}, {7, MONTH_31DAYS}, {8, MONTH_31DAYS},
      {9, MONTH_30DAYS}, {10, MONTH_31DAYS}, {11, MONTH_30DAYS},
      {12, MONTH_31DAYS} };
};

class Time
{
public:
    Time();
    Time(uint h, uint m, uint s);

    bool isValidT();
    uint64_t TimeInSec();

    void setHour(uint hour);
    void setMin(uint min);
    void setSec(uint sec);
    int getHour();
    int getMin();
    int getSec();

    static Time CurrentTime();

    // Can be modified and executed using difftime or comparison in seconds
    bool operator > (Time t);
    bool operator < (Time t);
    bool operator == (Time t);
    bool operator != (Time t);
    bool operator >= (Time t);
    bool operator <= (Time t);
    Time operator = (Time t);
    friend std::ostream& operator << (std::ostream& os, Time t);

protected:
    int hour;
    int min;
    int sec;
};

class DateTime : public Date, public Time
{
public:
    DateTime();
    DateTime(uint year, uint month, uint day, uint hour, uint min, uint sec);
    DateTime(uint64_t sec);
    DateTime(Date d);
    DateTime(Time t);

    Date date(); // For date comparison operations
    Time time(); // For time comparison operations

    DateTime startOfDay();
    DateTime startOfDay(uint h, uint m, uint s);
    void swap(DateTime forSwap, std::string swapParams = "YMDHMS");
    std::string toString(std::string format = "YMDHMS");
    DateTime addSecs(int64_t seconds); // When you enter a minus value, the number of seconds is subtracted from the date and time

    static DateTime CurrentDateTime();
    static uint64_t DateTimeDurationInSec(DateTime ldt, DateTime rdt);
    uint64_t DateTimeInSec();

    DateTime operator = (DateTime dt);
    friend std::ostream& operator << (std::ostream& os, DateTime dt);
};
}
