/*
   Hebcal - A Jewish Calendar Generator

   Copyright (C) 1994-2004  Danny Sadinoff
   Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.

   https://github.com/hebcal/hebcal

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Danny Sadinoff can be reached at
   danny@sadinoff.com
 */

#include "hebcal.hh"

// Gregorian calendar
const char *monthsGreg[12] = {
	"January", "February", "March",    "April",    "May",      "June",
	"July",    "August",   "September", "October", "November", "December"
};
const char *daysGregShort[7] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};
const int lengthsGreg[2][12] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

bool isLeapGreg(int year) {
	if ((!(year % 400)) || (!((year % 4) && !!(year % 100)))) {
		return true;
	}
	return false;
}

int getLengthGreg(int year, int month) {
	if ((month < 1) || (month > 12)) {
		return 0;
	}
	if (isLeapGreg(year)) {
		return lengthsGreg[1][month - 1];
	} else {
		return lengthsGreg[0][month - 1];
	}
}

// Days within the year, with 1 January as 1 and 31 December 2000 as 366.
int getDaysYearGreg(int year, int month, int day) {
	int days = day + 31 * (month - 1);
	if (month > 2) {
		days -= (4 * month + 23) / 10;
		if (isLeapGreg(year)) {
			days++;
		}
	}
	return days;
}

// Hebrew calendar
const char *monthsHebrew[2][14] = {
	{"Nisan", "Iyar", "Sivan", "Tamuz", "Av", "Elul", "Tishrei",
	"Cheshvan", "Kislev", "Tevet", "Sh'vat", "Adar", "Nisan"},
	{"Nisan", "Iyar", "Sivan", "Tamuz", "Av", "Elul", "Tishrei",
	"Cheshvan", "Kislev", "Tevet", "Sh'vat", "Adar I", "Adar II", "Nisan"}
};

bool isLeapHebrew(int year) {
        return ((1L + static_cast<long>(year) * 7L) % 19L < 7L);
}
int countHebrewMonths(int year) {
        return isLeapHebrew(year) ? 13 : 12;
}
int getLengthHebrew(int year, int month) {
	bool leap = isLeapHebrew(year);
	bool cheshvanLong = isLongCheshvan(year);
	bool kislevShort = isShortKislev(year);
	if ( month == IYYAR ||
         month == TAMUZ ||
         month == ELUL ||
         month == TEVET ||
         month == ADAR_II ||
         (month == ADAR_I && leap) ||
         (month == CHESHVAN && !cheshvanLong) ||
         (month == KISLEV && kislevShort)) {
		return 29;
	}
	return 30;
}

int getDaysYearHebrew(int year) {
	return static_cast<int>(elapsedDaysHebrew(year + 1) - elapsedDaysHebrew(year));
}
bool isLongCheshvan(int year) {
	return ((getDaysYearHebrew(year) % 10) == 5);
}

bool isShortKislev(int year) {
	return ((getDaysYearHebrew(year) % 10) == 3);
}

long elapsedDaysHebrew(int year) {
	long yearl, m_elapsed, p_elapsed, h_elapsed, parts, day, alt_day;

    yearl = static_cast<long>(year);
    m_elapsed = 235L * ((yearl - 1L) / 19L) +
        12L * ((yearl - 1L) % 19L) +
        ((((yearl - 1L) % 19L) * 7L) + 1L) / 19L;

    p_elapsed = 204L + (793L * (m_elapsed % 1080L));

    h_elapsed = 5L + (12L * m_elapsed) +
        793L * (m_elapsed / 1080L) +
        p_elapsed / 1080L;

    parts = (p_elapsed % 1080L) + 1080L * (h_elapsed % 24L);

    day = 1L + 29L * m_elapsed + h_elapsed / 24L;

    if ((parts >= 19440L) ||
        ((2L == (day % 7L)) && (parts >= 9924L) && !(isLeapHebrew(year))) ||
        ((1L == (day % 7L)) && (parts >= 16789L) && isLeapHebrew(year - 1)))
        alt_day = day + 1L;
    else
        alt_day = day;

    if ((alt_day % 7L) == 0L ||
        (alt_day % 7L) == 3L ||
        (alt_day % 7L) == 5L)
        return alt_day + 1L;
    else
        return alt_day;
}
