/*
Copyright (C) 2018 Juraj Andrássy
repository https://github.com/jandrassy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _UNOWIFIDEVEDSERIAL1_H_
#define _UNOWIFIDEVEDSERIAL1_H_

#include <Arduino.h>

class CStringBuilder : public Print {

  char* buffer;
  size_t size;
  size_t pos;

public:
  CStringBuilder(char* _buffer, size_t _size);

  void reset();

  size_t length();

  void setLength(size_t l);

  virtual size_t write(uint8_t b);

  void printf(const char *fmt, ... );

  void printf(const __FlashStringHelper *fmt, ... );

};

#endif
