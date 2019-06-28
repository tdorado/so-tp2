#include <time.h>
#include <stdio.h>
#include <systemCall.h>

#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x02
#define HOUR_REGISTER 0x04
#define WEEKDAY_REGISTER 0x06
#define DAY_REGISTER 0x07
#define MONTH_REGISTER 0x08
#define YEAR_REGISTER 0x09

static int UTC = -3;

static const int monthsDays[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
static const int lastDayMonths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int getSecond()
{
    return (int)systemCall(GET_TIME, SECONDS_REGISTER, 0, 0, 0, 0, 0);
}

int getMinute()
{
    return (int)systemCall(GET_TIME, MINUTES_REGISTER, 0, 0, 0, 0, 0);
}

int getHour()
{
    return (int)systemCall(GET_TIME, HOUR_REGISTER, 0, 0, 0, 0, 0);
}

int getWeekDay()
{
    return (int)systemCall(GET_TIME, WEEKDAY_REGISTER, 0, 0, 0, 0, 0);
}

int getDay()
{
    return (int)systemCall(GET_TIME, DAY_REGISTER, 0, 0, 0, 0, 0);
}

int getMonth()
{
    return (int)systemCall(GET_TIME, MONTH_REGISTER, 0, 0, 0, 0, 0);
}

int getYear()
{
    return (int)systemCall(GET_TIME, YEAR_REGISTER, 0, 0, 0, 0, 0) + 2000;
}

void getAllTimesForUTC(int times[7], int actualUTC)
{
    int hour, weekDay, day, month, year, flag = 0;

    times[0] = getSecond();
    times[1] = getMinute();

    hour = getHour() + actualUTC;
    weekDay = getWeekDay();
    day = getDay();
    month = getMonth();
    year = getYear();

    if (hour < 0)
    {
        hour = 24 + hour;
        flag = -1;
    }
    else if (hour >= 24)
    {
        hour = hour - 24;
        flag = 1;
    }
    times[2] = hour;
    if (flag == -1)
    {
        if (weekDay > 1)
        {
            weekDay--;
        }
        else
        {
            weekDay = 7;
        }
        if (day > 1)
        {
            day--;
        }
        else
        {
            if (month > 1)
            {
                month--;
            }
            else
            {
                year--;
                month = 12;
            }
            day = lastDayMonths[month - 1];
            if (year % 4 == 0 && month == 2)
            { // Leap Year
                day = 29;
            }
        }
    }
    else if (flag == 1)
    {
        if (weekDay < 7)
        {
            weekDay++;
        }
        else
        {
            weekDay = 1;
        }
        if ((day < lastDayMonths[month - 1]) || (year % 4 == 0 && month == 2 && day < 29)) //Last condition is leap year
        {
            day++;
        }
        else
        {
            day = 1;
            if (month < 12)
            {
                month++;
            }
            else
            {
                month = 1;
                year++;
            }
        }
    }
    times[3] = weekDay;
    times[4] = day;
    times[5] = month;
    times[6] = year;

    return;
}

long int time(long int *t)
{
    int times[7] = {getSecond(), getMinute(), getHour(), getWeekDay(), getDay(), getMonth(), getYear()};
    long int result = 0;

    result += times[0];                //Add actual seconds
    result += times[1] * 60;           //Add seconds of minute
    result += times[2] * 60 * 60;      //Add seconds of hour
    result += times[4] * 24 * 60 * 60; //Add seconds of day

    int year = times[6];
    int month = times[5];

    result += monthsDays[month - 1] * 24 * 60 * 60; //Add seconds of month without leaps
    if ((year % 4) == 0 && month > 2)               //if year is leap and we are past febrary adds one day in seconds
    {
        result += 24 * 60 * 60;
    }

    int leapDays = ((int)year / (int)4 - (int)1970 / (int)4) - 1; //Calculate total leap days
    result += leapDays * 24 * 60 * 60;                            //Add seconds of leap days
    year -= 1970;
    result += year * 365 * 24 * 60 * 60; //Adds seconds of year without leaps

    if (t != NULL)
    {
        *t = result;
    }
    return result;
}

void getAllTimes(int times[7])
{
    getAllTimesForUTC(times, UTC);
}

int getTimeUTC()
{
    return UTC;
}

void setTimeUTC(int newUTC)
{
    if (newUTC >= -12 || newUTC <= 12)
        UTC = newUTC;
}