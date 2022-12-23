
#include "date.hh"

Date::Date() {
    year = month = day = 0;
}

Date::Date(int y, int m, int d) {
    year  = y;
    month = m;
    day   = d;
    gregorian = false;
}

void Date::incrementGregorian(int days) {
	int leap = isLeapGreg(year) ? 1 : 0;

	day += days;

	while (day > lengthsGreg[leap][month - 1]) {
		day -= lengthsGreg[leap][month - 1];

		if (month == 12) {
			month = 0;
			year++;
			leap = isLeapGreg(year) ? 1 : 0;
		}

		month++;
	}
}
void Date::incrementHebrew(int days) {
	day += days;

	while (day > getLengthHebrew(year, month)) {
		day -= getLengthHebrew(year, month);

		if (month == 6) {
			year++;
		} else if (month == countHebrewMonths(year)) {
			month = 0;
		}
		month++;
	}
}

void Date::increment(int days) {
	if (gregorian) {
		incrementGregorian(days);
	} else {
		incrementHebrew(days);
	}
}

/*
 * See the footnote on page 384 of ``Calendrical Calculations, Part II:
 * Three Historical Calendars'' by E. M. Reingold,  N. Dershowitz, and S. M.
 * Clamen, Software--Practice and Experience, Volume 23, Number 4
 * (April, 1993), pages 383-404 for an explanation.
 */
Date *Date::absoluteGregorianDate(long abs) {
	int year, month, day, mlen;
	Date *date = new Date();
	
	// calculate Gregorian date
	long int d0, n400, d1, n100, d2, n4, d3, n1;

	d0   = abs - 1L;
	n400 = d0 / 146097L;
	d1   = d0 % 146097L;
	n100 = d1 /  36524L;
	d2   = d1 %  36524L;
	n4   = d2 /   1461L;
	d3   = d2 %   1461L;
	n1   = d3 /    365L;

	day  = static_cast<int>((d3 % 365L) + 1L);
	year = static_cast<int>(400L * n400 + 100L * n100 + 4L * n4 + n1);
	if (4L == n100 || 4L == n1) {
		date->setDate(year, 12, 31);
	} else {
		year++;
		int leap = isLeapGreg(year) ? 1 : 0;
		month = 1;

		while ((mlen = lengthsGreg[leap][month - 1]) < day) {
			day -= mlen;
			month++;
		}
		date->setDate(year, month, day);
	}

    date->setGregorian(true);

    return date;
}
Date *Date::absoluteHebrewDate(long abs) {
	// calculate Hebrew date
	static int mmap[] = {
            KISLEV, TEVET, SHVAT, ADAR_I, NISAN,
            IYYAR, SIVAN, TAMUZ,
            TISHREI, TISHREI, TISHREI, CHESHVAN
	};
    int year, month, day;
    Date *greg = Date::absoluteGregorianDate(abs);
    Date *hebrew = new Date();

    if (abs >= 10555144L) {
        std::cerr << "RangeError: Date::absoluteHebrewDate(" << abs << ")" << std::endl;
        exit(1);
    }

	year  = greg->getYear() + 3760;
	hebrew->setMonth(7);
	hebrew->setDay(1);
	int temp = 0;
	while (hebrew->setYear(year + 1), abs >= hebrew->toAbsoluteDateFromHebrew()) {
		year++;
	}
    hebrew->setYear(temp);
	if ((year >= 4635) && (year < 10666)) {
		month = mmap[greg->getMonth() - 1];
	} else {
		month = 7;
	}

	while (hebrew->setMonth(month),
		   hebrew->setDay(getLengthHebrew(month, year)),
           hebrew->setYear(year),
		   abs > hebrew->toAbsoluteDateFromHebrew()) {
		month = (month % countHebrewMonths(year)) + 1;
	}
    hebrew->setDay(1);
    day = static_cast<int>(abs - hebrew->toAbsoluteDateFromHebrew() + 1L);
    if (day < 0) {
        std::cerr << "ValueError: Date::absoluteHebrewDate(" << abs << ") < " <<
            hebrew->toAbsoluteDateFromHebrew() << std::endl;
            exit(1);
    }
    hebrew->setDay(day);
    hebrew->setGregorian(false);

    return hebrew;
}

// Days elapsed between Gregorian date 31 December 1 BCE and date. This epoch
// is an imaginary date.
long Date::toAbsoluteDateFromGregorian(int year, int month, int day) {
	int y = year - 1;
	return (
		static_cast<long>(getDaysYearGreg(year, month, day))  // days this year
		+ 365L * static_cast<long>(y)             // days previous years
		+ static_cast<long>(y / 4                 // Julian leap years
			- y    / 100                          // - century years
			+ y    / 400)                         // + Gregorian leap years
	);
}
long Date::toAbsoluteDateFromGregorian(void) {
    return toAbsoluteDateFromGregorian(year, month, day);
}

long Date::toAbsoluteDateFromHebrew(int year, int month, int day) {
    int m;
	long temp = day;
	long ret;
	
    if (month < TISHREI) {
        for (m = TISHREI; m <= countHebrewMonths(year); m++)
            temp += static_cast<long>(getLengthHebrew(year, m));

        for (m = NISAN; m < month; m++)
            temp += static_cast<long>(getLengthHebrew(year, m));
    } else {
        for (m = TISHREI; m < month; m++)
            temp += static_cast<long>(getLengthHebrew(year, m));
    }

	ret = elapsedDaysHebrew(year) - 1373429L + temp;
	return ret;
}
long Date::toAbsoluteDateFromHebrew(void) {
    return toAbsoluteDateFromHebrew(year, month, day);
}
