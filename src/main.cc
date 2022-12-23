
#include <iostream>

#include "date.hh"

int main(void) {
	long abs = Date::toAbsoluteDateFromGregorian(2022, 12, 23);
	Date *date = Date::absoluteHebrewDate(abs);

	std::cout << "Today is " << date->getDay() << " " <<
		monthsHebrew[isLeapHebrew(date->getYear()) ? 1 : 0][date->getMonth() - 1] << " " <<
		date->getYear() << std::endl;

	return 0;
}
