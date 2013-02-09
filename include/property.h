#ifndef MSIM_PROPERTY_H
#define MSIM_PROPERTY_H

/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/
 
#include <stdio.h>

namespace agenda {

template <typename T>
  class Property
  {
  public:
     Property(const T & value = T())
       : data(value)
     {}

     const T& operator() () const    { return data; }
     void     set (const T & value)  { data = value; }
     const T& operator= (const T & value) { data = value; return data; }
  private:
    T  data;
  };

}


#endif
