#include <iostream>
#include <stdio.h>
#include <sys/times.h>
#include <time.h>

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
#include <string.h>
#include <stdlib.h>

using namespace std;

int
main (int argc, char * argv[])
{

  if (argc < 2) {
    return 1;
  }

  timespec        fineTime;
  unsigned   int seed (0);

  clock_gettime (CLOCK_REALTIME, &fineTime);
  seed = abs (fineTime.tv_sec + fineTime.tv_nsec);
  
  srandom (seed);
  int choice (0);

  choice = random() % (argc-1);
  cout << argv[choice+1] << endl;
  return 0;
}
