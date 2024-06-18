/*
 * Author: Arvin Singh
 * Date: 05/04/2024
 * Description: This file contains the implementation of the Date class.
*/


#include "date.h"
#include <string>
#include <sstream>


// Default constructor to create an unitialised date.
Date::Date() : year(0), month(0), day(0), initialized(false) {}


/*
    * Function to set the date from a string
    * @param dateString: The string to set the date from
    * @throws std::invalid_argument: If the date string is not in the correct format
    * @throws std::invalid_argument: If the date is not valid
    
*/
void Date::setDateFromString(const String& dateString) {
    if (dateString.empty()) {
        // Reset the object to its uninitialised state
        this->initialized = false;
    } else {
        // Parse the date string
        if (dateString.size() != 10 || dateString[4] != '-' || dateString[7] != '-') {
            throw std::invalid_argument("Invalid date format. Valid format is YYYY-MM-DD.");
        }
        unsigned int rYear = std::stoi(dateString.substr(0, 4));
        unsigned int rMonth = std::stoi(dateString.substr(5, 2));
        unsigned int rDay = std::stoi(dateString.substr(8, 2));
        if (!checkValidDate(rYear, rMonth, rDay)) {
            this->initialized = false;
            throw std::invalid_argument("Invalid date.");
        }
        else {
            setDate(rYear, rMonth, rDay);
        }
    }
}

/*
    * Function to check if a date is valid
    * @param year: The year of the date
    * @param month: The month of the date
    * @param day: The day of the date
    * @return bool: True if the date is valid, false otherwise
*/
bool Date::checkValidDate(unsigned int year, unsigned int month, unsigned int day) {
    if (year < 0 || month < 1 || month > 12 || day < 1) {
        return false;
    }
    switch (month) {
        case 1:
            return day <= 31;
        case 2:
            return day <= ((year % 4 == 0 || year % 400 == 0) ? 29 : 28);
        case 3:
            return day <= 31;
        case 4:
            return day <= 30;
        case 5:
            return day <= 31;
        case 6:
            return day <= 30;
        case 7:
            return day <= 31;
        case 8:
            return day <= 31;
        case 9:
            return day <= 30;
        case 10:
            return day <= 31;
        case 11:
            return day <= 30;
        case 12:
            return day <= 31;
        default:
            return false;
    }
}

// Function to set iniialized state.
// A Date object should only be set to initialised if it is a valid date.
void Date::setInitialised(bool initialised) {
    this->initialized = initialised;
}

// Function to get the initialised state.
const bool Date::isInitialised() const noexcept {
    return initialized;
}

// Function to return the date as a string.
String Date::str() const {
    if (!initialized) {
        return "";
    }
    std::stringstream ss;
    ss << year << "-" << month << "-" << day;
    return ss.str();
}

// Function to set the date.
void Date::setDate(unsigned int year, unsigned int month, unsigned int day) noexcept {
    this->year = year;
    this->month = month;
    this->day = day;
    this->initialized = true;
}

// Function to get the year.
const unsigned int Date::getYear() const noexcept {
    return year;
}

// Function to get the month.
const unsigned int Date::getMonth() const noexcept {
    return month;
}

// Function to get the day.
const unsigned int Date::getDay() const noexcept {
    return day;
}

// Overloaded == function to check if two dates are equal.
bool operator==(const Date &date1, const Date &date2) {
    return date1.year == date2.year && date1.month == date2.month && date1.day == date2.day;
}

// Overloaded < function to check if dates 1 is less than date 2.
bool operator<(const Date &date1, const Date &date2) {
    if (date1.year < date2.year) {
        return true;
    } else if (date1.year == date2.year) {
        if (date1.month < date2.month) {
            return true;
        } else if (date1.month == date2.month) {
            return date1.day < date2.day;
        }
    }
    return false;
}
