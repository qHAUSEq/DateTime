#include "DateTime.h"

DateTime::Time::Time()
{
    time_t now = std::time(0);
    tm* ltm = localtime(&now);
    this->hour = ltm->tm_hour;
    this->min = ltm->tm_min;
    this->sec = ltm->tm_sec;
}

DateTime::Time::Time(uint h, uint m, uint s)
{
    this->hour = h;
    this->min = m;
    this->sec = s;
    if (this->isValidT() != true) *this = Time();
}

bool DateTime::Time::isValidT()
{
    return fabs(this->hour) < 24 && fabs(this->min) < 60 && fabs(this->sec) < 60;
}

void DateTime::Time::setHour(uint hour)
{
    this->hour = hour;
    if (!this->isValidT()) this->hour = CurrentTime().hour;
}
void DateTime::Time::setMin(uint min)
{
    this->min = min;
    if (!this->isValidT()) this->min = CurrentTime().min;
}
void DateTime::Time::setSec(uint sec)
{
    this->sec = sec;
    if (!this->isValidT()) this->sec = CurrentTime().sec;
}
int DateTime::Time::getHour()
{
    return this->hour;
}
int DateTime::Time::getMin()
{
    return this->min;
}
int DateTime::Time::getSec()
{
    return this->sec;
}

DateTime::Time DateTime::Time::CurrentTime()
{
    return Time();
}

DateTime::Time DateTime::Time::operator = (Time t)
{
    this->sec = t.sec;
    this->min = t.min;
    this->hour = t.hour;

    return *this;
}
std::ostream& DateTime::operator << (std::ostream& os, Time t)
{
    os << t.hour << ":" << t.min << ":" << t.sec;
    return os;
}
bool DateTime::Time::operator > (Time t)
{
    uint HMS[3]{0,0,0};
    if (this->hour > t.hour)    HMS[0] = 1;
    if (this->min > t.min)      HMS[1] = 1;
    if (this->sec > t.sec)      HMS[2] = 1;
    if (this->hour == t.hour)   HMS[0] = 2;
    if (this->min == t.min)     HMS[1] = 2;
    if (this->sec == t.sec)     HMS[2] = 2;

    if ((HMS[0] == 1 && HMS[1] == 1 && HMS[2] == 1) ||
            (HMS[0] == 1 && HMS[1] == 1 && HMS[2] == 0) ||
            (HMS[0] == 1 && HMS[1] == 0 && HMS[2] == 0) ||
            (HMS[0] == 2 && HMS[1] == 1 && HMS[2] == 1) ||
            (HMS[0] == 2 && HMS[1] == 1 && HMS[2] == 0) ||
            (HMS[0] == 1 && HMS[1] == 2 && HMS[2] == 2) ||
            (HMS[0] == 1 && HMS[1] == 1 && HMS[2] == 2) ||
            (HMS[0] == 2 && HMS[1] == 2 && HMS[2] == 1) ||
            (HMS[0] == 2 && HMS[1] == 2 && HMS[2] == 0) ||
            (HMS[0] == 1 && HMS[1] == 0 && HMS[2] == 1))
    {
        return true;
    }
    return false;
}
bool DateTime::Time::operator < (Time t)
{
    uint HMS[3]{0,0,0};
    if (this->hour < t.hour)    HMS[0] = 1;
    if (this->min < t.min)      HMS[1] = 1;
    if (this->sec < t.sec)      HMS[2] = 1;
    if (this->hour == t.hour)   HMS[0] = 2;
    if (this->min == t.min)     HMS[1] = 2;
    if (this->sec == t.sec)     HMS[2] = 2;

    if ((HMS[0] == 1 && HMS[1] == 1 && HMS[2] == 1) ||
            (HMS[0] == 1 && HMS[1] == 1 && HMS[2] == 0) ||
            (HMS[0] == 1 && HMS[1] == 0 && HMS[2] == 0) ||
            (HMS[0] == 2 && HMS[1] == 1 && HMS[2] == 1) ||
            (HMS[0] == 2 && HMS[1] == 1 && HMS[2] == 0) ||
            (HMS[0] == 1 && HMS[1] == 2 && HMS[2] == 2) ||
            (HMS[0] == 1 && HMS[1] == 1 && HMS[2] == 2) ||
            (HMS[0] == 2 && HMS[1] == 2 && HMS[2] == 1) ||
            (HMS[0] == 2 && HMS[1] == 2 && HMS[2] == 0) ||
            (HMS[0] == 1 && HMS[1] == 0 && HMS[2] == 1))
    {
        return true;
    }
    return false;
}
bool DateTime::Time::operator == (Time t)
{
    return (this->hour == t.hour && this->min == t.min && this->sec == t.sec ? true : false);
}
bool DateTime::Time::operator != (Time t)
{
    return (this->hour != t.hour && this->min != t.min && this->sec != t.sec ? true : false);
}
bool DateTime::Time::operator >= (Time t)
{
    if (this->hour == t.hour && this->min == t.min && this->sec == t.sec) return true;
    if (this->hour > t.hour && this->min > t.min && this->sec > t.sec) return true;

    uint HMS[3]{0,0,0};
    if (this->hour >= t.hour)    HMS[0] = 1;
    if (this->min >= t.min)      HMS[1] = 1;
    if (this->sec >= t.sec)      HMS[2] = 1;

    if ((HMS[0] == 1 && HMS[1] == 1 && HMS[2] == 0)         ||
            (HMS[0] == 1 && HMS[1] == 0 && HMS[2] == 0)     ||
            (HMS[0] == 1 && HMS[1] == 0 && HMS[2] == 1))
    {
        return true;
    }
    return false;
}
bool DateTime::Time::operator <= (Time t)
{
    if (this->hour == t.hour && this->min == t.min && this->sec == t.sec) return true;
    if (this->hour < t.hour && this->min < t.min && this->sec < t.sec) return true;

    uint HMS[3]{0,0,0};
    if (this->hour <= t.hour)    HMS[0] = 1;
    if (this->min <= t.min)      HMS[1] = 1;
    if (this->sec <= t.sec)      HMS[2] = 1;

    if ((HMS[0] == 1 && HMS[1] == 1 && HMS[2] == 0)         ||
            (HMS[0] == 1 && HMS[1] == 0 && HMS[2] == 0)     ||
            (HMS[0] == 1 && HMS[1] == 0 && HMS[2] == 1))
    {
        return true;
    }
    return false;
}
