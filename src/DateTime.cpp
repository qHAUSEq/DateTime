#include "DateTime.h"

DateTime::DateTime::DateTime()
{
    Date();
    Time();
}
DateTime::DateTime::DateTime(uint year, uint month, uint day, uint hour, uint min, uint sec)
{
    this->year = year;
    this->month = month;
    this->day = day;

    this->hour = hour;
    this->min = min;
    this->sec = sec;

    if (!this->isValidD() || !this->isValidT()) *this = CurrentDateTime();
}
DateTime::DateTime::DateTime(DateTime::Date d)
{
    this->year = d.getYear();
    this->month = d.getMonth();
    this->day = d.getDay();
    this->hour = Time::CurrentTime().getHour();
    this->min = Time::CurrentTime().getMin();
    this->sec = Time::CurrentTime().getSec();
}
DateTime::DateTime::DateTime(DateTime::Time t)
{
    this->hour = t.getHour();
    this->min = t.getMin();
    this->sec = t.getSec();
    this->year = Date::CurrentDate().getYear();
    this->month = Date::CurrentDate().getMonth();
    this->day = Date::CurrentDate().getDay();
}
DateTime::DateTime::DateTime(uint64_t sec)
{
    time_t now = sec;
    tm* ltm = localtime(&now);
    Date(1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    Time(ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

DateTime::Date DateTime::DateTime::date()
{
    return Date(this->year, this->month, this->day);
}
DateTime::Time DateTime::DateTime::time()
{
    return Time(this->hour, this->min, this->sec);
}

DateTime::DateTime DateTime::DateTime::addSecs(int64_t seconds)
{
    // Add and minus sec
    time_t t = this->DateTimeInSec();
    if (seconds > 0) t += seconds;
    if (seconds < 0)
    {
        seconds *= -1;
        t -= seconds;
    }
    if (seconds == 0)
    {
        DateTime dt(this->year, this->month, this->day, this->hour, this->min, this->sec);
        return dt;
    }
    tm* mdt = localtime(&t);

    DateTime dt(mdt->tm_year + 1900, mdt->tm_mon + 1, mdt->tm_mday, mdt->tm_hour, mdt->tm_min, mdt->tm_sec);
    return dt;
}

uint64_t DateTime::DateTime::DateTimeDurationInSec(DateTime ldt, DateTime rdt)
{
    if (rdt.date() > ldt.date()) return -1;
    if (rdt.year == ldt.year && rdt.month > ldt.month) return -1;
    if (rdt.date() == ldt.date() && rdt.time() == ldt.time()) return 0;

    std::map<int, SecIn>::iterator it;
    int64_t timeDuration = 0;

    timeDuration = ldt.sec - rdt.sec;
    timeDuration += (ldt.min - rdt.min) * 60;
    timeDuration += (ldt.hour - rdt.hour) * 3600;
    timeDuration += (ldt.day - rdt.day) * 86400;
    timeDuration += (ldt.year - rdt.year) * YEAR;
    if (ldt.month > rdt.month)
    {
        for (auto i = rdt.month; i < ldt.month; i++)
        {
            it = rdt.numberOfDayInMonth.find(i);
            if (it->first == i)
            {
                timeDuration += it->second;
            }
        }
    }
    else
    {
        for (auto i = ldt.month; i < rdt.month; i++)
        {
            it = ldt.numberOfDayInMonth.find(i);
            if (it->first == i)
            {
                timeDuration -= it->second;
            }
        }
    }
    if (ldt.year % 4 == 0 && ldt.month > 2)
    {
        timeDuration += 86400;
    }

    return fabs(timeDuration);
}
uint64_t DateTime::DateTime::DateTimeInSec()
{
    if (this->isValidD() != true)
    {
        return -1;
    }
    if (this->isValidT() != true)
    {
        return -2;
    }

    std::map<int, SecIn>::iterator it;
    uint64_t dtInSec = 0;

    dtInSec = this->sec;
    dtInSec += this->min * 60;
    dtInSec += (this->hour - 3) * 3600;
    dtInSec += (this->day - 2) * 86400;

    for (int i = 1; i < this->month; i++)
    {
        it = this->numberOfDayInMonth.find(i);
        if (it->first == i)
        {
            dtInSec += it->second;
        }
    }
    for (int i = 0; i < this->year - 1970; i++)
    {
        if (i % 4 == 0)
        {
            dtInSec += SecIn::YEAR_LEAP;
        }
        if (i % 4 != 0)
        {
            dtInSec += SecIn::YEAR;
        }
    }

    return dtInSec;
}

DateTime::DateTime DateTime::DateTime::startOfDay()
{
    return DateTime(this->year, this->month, this->day, 0, 0, 0);
}
DateTime::DateTime DateTime::DateTime::startOfDay(uint h, uint m, uint s)
{
    return DateTime(this->year, this->month, this->day, h, m, s);
}

void DateTime::DateTime::swap(DateTime forSwap, std::string swapParams)
{
    if (!forSwap.isValidD()) return;
    if (!forSwap.isValidT()) return;

    int tmp;

    if (swapParams == "S" || swapParams == "YMDHMS" || swapParams == "HMS")
    {
        tmp = forSwap.getSec();
        forSwap.setSec(this->sec);
        this->sec = tmp;
    }
    if (swapParams == "MIN" || swapParams == "YMDHMS" || swapParams == "HMS")
    {
        tmp = forSwap.getMin();
        forSwap.setMin(this->min);
        this->min = tmp;
    }
    if (swapParams == "H" || swapParams == "YMDHMS" || swapParams == "HMS")
    {
        tmp = forSwap.getHour();
        forSwap.setHour(this->hour);
        this->hour = tmp;
    }
    if (swapParams == "D" || swapParams == "YMDHMS" || swapParams == "YMD")
    {
        tmp = forSwap.getDay();
        forSwap.setDay(this->day);
        this->day = tmp;
    }
    if (swapParams == "M" || swapParams == "YMDHMS" || swapParams == "YMD")
    {
        tmp = forSwap.getMonth();
        forSwap.setMonth(this->month);
        this->month = tmp;
    }
    if (swapParams == "Y" || swapParams == "YMDHMS" || swapParams == "YMD")
    {
        tmp = forSwap.getYear();
        forSwap.setYear(this->year);
        this->year = tmp;
    }
}

std::string DateTime::DateTime::toString(std::string format)
{
    if (format == "YMD" || format == "ymd")
    {
        std::string formatDate = (this->day < 10 ? "0" + std::to_string(this->day) : std::to_string(this->day)) + "."
                + (this->month < 10 ? "0" + std::to_string(this->month) : std::to_string(this->month)) + "."
                + std::to_string(this->year);
        return formatDate;
    }
    if (format == "HMS" || format == "hms")
    {
        std::string formatTime = (this->hour < 10 ? "0" + std::to_string(this->hour) : std::to_string(this->hour)) + ":"
                + (this->min < 10 ? "0" + std::to_string(this->min) : std::to_string(this->min)) + ":"
                + (this->sec < 10 ? "0" + std::to_string(this->sec) : std::to_string(this->sec));
        return formatTime;
    }
    if (format == "YMDHMS" || format == "ymdhms")
    {
        std::string formatDateTime = (this->day < 10 ? "0" + std::to_string(this->day) : std::to_string(this->day)) + "."
                + (this->month < 10 ? "0" + std::to_string(this->month) : std::to_string(this->month)) + "."
                + std::to_string(this->year) + "   "
                + (this->hour < 10 ? "0" + std::to_string(this->hour) : std::to_string(this->hour)) + ":"
                + (this->min < 10 ? "0" + std::to_string(this->min) : std::to_string(this->min)) + ":"
                + (this->sec < 10 ? "0" + std::to_string(this->sec) : std::to_string(this->sec));
        return formatDateTime;
    }
    if (format == "fYMDHMS" || format == "FYMDHMS")
    {
        std::string formatFileDateTime = "-" + std::to_string(this->year - 2000) + std::to_string(this->month)
                + std::to_string(this->day) + "-" + std::to_string(this->hour)
                + std::to_string(this->min) + std::to_string(this->sec);
        return formatFileDateTime;
    }

    return "Invalid format";
}

DateTime::DateTime DateTime::DateTime::CurrentDateTime()
{
    return DateTime();
}

DateTime::DateTime DateTime::DateTime::operator = (DateTime dt)
{
    this->sec = dt.sec;
    this->min = dt.min;
    this->hour = dt.hour;
    this->day = dt.day;
    this->month = dt.month;
    this->year = dt.year;

    return *this;
}
std::ostream& DateTime::operator << (std::ostream& os, DateTime dt)
{
    os << dt.day << "." << dt.month << "." << dt.year << "  " << dt.hour << ":" << dt.min << ":" << dt.sec;
    return os;
}
