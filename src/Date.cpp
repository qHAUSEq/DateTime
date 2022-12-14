#include "DateTime.h"

DateTime::Date::Date()
{
    time_t now = std::time(0);
    tm* ltm = localtime(&now);
    this->year = 1900 + ltm->tm_year;
    this->month = 1 + ltm->tm_mon;
    this->day = ltm->tm_mday;
}

DateTime::Date::Date(uint y, uint m, uint d)
{
    this->year = y;
    this->month = m;
    this->day = d;
    if (this->isValidD() != true) *this = Date();
}

bool DateTime::Date::isValidD()
{
    if (this->year < 100) this->year += 2000;

    time_t dateInSec = DateInSec();
    tm* date = std::localtime(&dateInSec);
    if (std::mktime(date) == -1) return false;

    return true;
}

bool DateTime::Date::leapYear()
{
    if (this->year % 4 == 0) return true;
    else return false;
}

void DateTime::Date::setYear(uint year)
{
    this->year = year;
}
void DateTime::Date::setMonth(uint month)
{
    this->month = month;
}
void DateTime::Date::setDay(uint day)
{
    this->day = day;
}
int DateTime::Date::getYear()
{
    return this->year;
}
int DateTime::Date::getMonth()
{
    return this->month;
}
int DateTime::Date::getDay()
{
    return this->day;
}

DateTime::Date DateTime::Date::CurrentDate()
{
    return Date();
}
uint64_t DateTime::Date::DateInSec(uint hour, uint min, uint sec)
{
    std::map<int, SecIn>::iterator it;
    uint64_t dateInSec = 0;

    dateInSec = sec;
    dateInSec += min * 60;
    dateInSec += (hour - 3) * 3600;
    dateInSec += (this->day - 2) * 86400;

    for (int i = 1; i < this->month; i++)
    {
        it = this->numberOfDayInMonth.find(i);
        if (it->first == i)
        {
            dateInSec += it->second;
        }
    }
    for (int i = 0; i < this->year - 1970; i++)
    {
        if (i % 4 == 0)
        {
            dateInSec += SecIn::YEAR_LEAP;
        }
        if (i % 4 != 0)
        {
            dateInSec += SecIn::YEAR;
        }
    }

    return dateInSec;
}

DateTime::Date DateTime::Date::operator = (Date d)
{
    this->day = d.day;
    this->month = d.month;
    this->year = d.year;

    return *this;
}
std::ostream& DateTime::operator << (std::ostream& os, Date d)
{
    os << d.day << "." << d.month << "." << d.year;
    return os;
}
bool DateTime::Date::operator > (Date d)
{
    uint DMY[3]{ 0, 0, 0 };
    if (this->day > d.day)       DMY[0] = 1;
    if (this->month > d.month)   DMY[1] = 1;
    if (this->year > d.year)     DMY[2] = 1;
    if (this->day == d.day)      DMY[0] = 2;
    if (this->month == d.month)  DMY[1] = 2;
    if (this->year == d.year)    DMY[2] = 2;

    if ((DMY[0] == 1 && DMY[1] == 1 && DMY[2] == 1)                    ||
            (DMY[0] == 1 && DMY[1] == 1 && DMY[2] == 2)                ||
            (DMY[0] == 1 && DMY[1] == 2 && DMY[2] == 2)                ||
            (DMY[0] == 2 && DMY[1] == 1 && DMY[2] == 2)                ||
            (DMY[0] == 2 && DMY[1] == 2 && DMY[2] == 1)                ||
            (DMY[0] == 2 && DMY[1] == 1 && DMY[2] == 1)                ||
            (DMY[0] == 0 && DMY[1] == 1 && DMY[2] == 1 || DMY[2] == 2) ||
            (DMY[0] == 0 && DMY[1] == 0 && DMY[2] == 1)                ||
            (DMY[0] == 1 && DMY[1] == 0 && DMY[2] == 1))
    {
        return true;
    }
    return false;
}
bool DateTime::Date::operator < (Date d)
{
    uint DMY[3]{ 0, 0, 0 };
    if (this->day < d.day)       DMY[0] = 1;
    if (this->month < d.month)   DMY[1] = 1;
    if (this->year < d.year)     DMY[2] = 1;
    if (this->day == d.day)      DMY[0] = 2;
    if (this->month == d.month)  DMY[1] = 2;
    if (this->year == d.year)    DMY[2] = 2;

    if ((DMY[0] == 1 && DMY[1] == 1 && DMY[2] == 1)                    ||
            (DMY[0] == 1 && DMY[1] == 1 && DMY[2] == 2)                ||
            (DMY[0] == 1 && DMY[1] == 2 && DMY[2] == 2)                ||
            (DMY[0] == 2 && DMY[1] == 1 && DMY[2] == 2)                ||
            (DMY[0] == 2 && DMY[1] == 2 && DMY[2] == 1)                ||
            (DMY[0] == 2 && DMY[1] == 1 && DMY[2] == 1)                ||
            (DMY[0] == 0 && DMY[1] == 1 && DMY[2] == 1 || DMY[2] == 2) ||
            (DMY[0] == 0 && DMY[1] == 0 && DMY[2] == 1)                ||
            (DMY[0] == 1 && DMY[1] == 0 && DMY[2] == 1))
    {
        return true;
    }
    return false;
}
bool DateTime::Date::operator == (Date d)
{
    return (this->year == d.year && this->month == d.month && this->day == d.day ? true : false);
}
bool DateTime::Date::operator != (Date d)
{
    return (this->year != d.year && this->month != d.month && this->day != d.day ? true : false);
}
bool DateTime::Date::operator >= (Date d)
{
    if (this->year == d.year && this->month == d.month && this->day == d.day) return true;
    if (this->year > d.year && this->month > d.month && this->day > d.day) return true;

    uint DMY[3]{ 0, 0, 0 };
    if (this->day >= d.day)       DMY[0] = 1;
    if (this->month >= d.month)   DMY[1] = 1;
    if (this->year >= d.year)     DMY[2] = 1;

    if ((DMY[0] == 0 && DMY[1] == 0 && DMY[2] == 1)                    ||
            (DMY[0] == 0 && DMY[1] == 1 && DMY[2] == 1)                ||
            (DMY[0] == 1 && DMY[1] == 0 && DMY[2] == 1))
    {
        return true;
    }
    return false;
}
bool DateTime::Date::operator <= (Date d)
{
    if (this->year == d.year && this->month == d.month && this->day == d.day) return true;
    if (this->year < d.year && this->month < d.month && this->day < d.day) return true;

    uint DMY[3]{ 0, 0, 0 };
    if (this->day <= d.day)       DMY[0] = 1;
    if (this->month <= d.month)   DMY[1] = 1;
    if (this->year <= d.year)     DMY[2] = 1;

    if ((DMY[0] == 0 && DMY[1] == 0 && DMY[2] == 1)                    ||
            (DMY[0] == 0 && DMY[1] == 1 && DMY[2] == 1)                ||
            (DMY[0] == 1 && DMY[1] == 0 && DMY[2] == 1))
    {
        return true;
    }
    return false;
}
