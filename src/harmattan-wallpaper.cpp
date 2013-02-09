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

#include <QApplication>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QTimer>

#include "deliberate.h"
#include "version.h"
#include "background-picker.h"

using namespace std;
using namespace deliberate;
using namespace agenda;

int
main (int argc, char * argv[])
{
  QCoreApplication::setOrganizationName ("BerndStramm");
  QCoreApplication::setOrganizationDomain ("souffleur.sourceforge.net");
  QCoreApplication::setApplicationName ("souffleur");
  deliberate::ProgramVersion pv ("Souffleur");
  QCoreApplication::setApplicationVersion (pv.Version());
  
  QApplication app (argc, argv,false);
  Settings().setValue ("program",pv.MyName());
  QStringList arglist = app.arguments();
  
  BackgroundPicker picker (BackgroundPicker::System_Harmattan);
  bool ok = QObject::connect (&picker, SIGNAL (quit()),&app,SLOT (quit()));
  
  if (arglist.isEmpty() || arglist.contains ("--random")) {
    picker.run (BackgroundPicker::Run_Random);
  } else if (arglist.contains ("--daily")) {
    picker.run (BackgroundPicker::Run_Daily);
  } else if (arglist.contains ("--hourly")) {
    picker.run (BackgroundPicker::Run_Hourly);
  } else {
    return 1;
  }
  qDebug () << " connected " << ok;
  QTimer::singleShot (15000,&app,SLOT(quit()));
  return app.exec ();
}
