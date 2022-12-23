
#pragma once

#include "hebcal.hh"
#include "date.hh"

// Gregorian calendar
extern const char *monthsGreg[12];
extern const char *daysGregShort[7];
extern const int lengthsGreg[2][12];

extern bool isLeapGreg(int year);
extern int getLengthGreg(int year, int month);
extern int getDaysYearGreg(int year, int month, int day);

// Hebrew calendar

#define NISAN 1
#define IYYAR 2
#define SIVAN 3
#define TAMUZ 4
#define AV 5
#define ELUL 6
#define TISHREI 7
#define CHESHVAN 8
#define KISLEV 9
#define TEVET 10
#define SHVAT 11
#define ADAR_I 12
#define ADAR_II 13

extern const char *monthsHebrew[2][14];

extern bool isLeapHebrew(int year);
extern int countHebrewMonths(int year); 
extern int getLengthHebrew(int year, int month);
extern bool isLongCheshvan(int year);
extern bool isShortKislev(int year);
extern long elapsedDaysHebrew(int year);
