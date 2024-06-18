/*
 * Author: Arvin Singh
 * Date: 04/04/2024
 * Description: This file contains the declaration of the Date class.
*/


#ifndef DATE_H
#define DATE_H

#include "lib_json.hpp"

using String = std::string;

class Date {
private:
  unsigned int year, month, day;
  bool initialized;

public:
  Date();
  ~Date() = default;

  void setDateFromString(const String& string);
  bool checkValidDate(unsigned int year, unsigned int month, unsigned int day);
  void setInitialised(bool initialised);

  const bool isInitialised() const noexcept;
  String str() const;

  void setDate(unsigned int year, unsigned int month, unsigned int day) noexcept;

  const unsigned int getYear() const noexcept;
  const unsigned int getMonth() const noexcept;
  const unsigned int getDay() const noexcept;
  friend bool operator==(const Date &date1, const Date &date2);
  friend bool operator<(const Date &date1, const Date &date2);
};

#endif // DATE_H
