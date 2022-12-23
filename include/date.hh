
#pragma once

#include <iostream>

#include "hebcal.hh"

class Date {
private:
    int day;
    int month;
    int year;
    bool gregorian;

    void incrementGregorian(int days);
    void incrementHebrew(int days);

public:
    Date();
    Date(int, int, int);
    static Date *absoluteHebrewDate(long abs);
    static Date *absoluteGregorianDate(long abs);

    int getDay() const { return this->day; }
    int getMonth() const { return this->month; }
    int getYear() const { return this->year; }
    bool getGregorian() const { return this->gregorian; }

    void setDate     (int  y, int m, int d) { this->year = y; this->month = m; this->day = d; }
    void setYear     (int  y) { this->year      = y; }
    void setMonth    (int  m) { this->month     = m; }
    void setDay      (int  d) { this->day       = d; }
    void setGregorian(bool g) { this->gregorian = g; }

    void increment(int days);

    static long toAbsoluteDateFromHebrew(int year, int month, int day);
    static long toAbsoluteDateFromGregorian(int year, int month, int day);
    long toAbsoluteDateFromHebrew(void);
    long toAbsoluteDateFromGregorian(void);

};
