
#include "schedmain.h"

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
#if 0
#include <QSystemDeviceInfo>
#endif
#include <QFont>
#include "deliberate.h"
#include "version.h"
#include "cmdoptions.h"
#include "agenda.h"
#include "notify.h"

#include "magic-defs.h"
#include "date-time-checker.h"
#include "orientation.h"

#include <qdeclarative.h>

QTM_USE_NAMESPACE

int
main (int argc, char *argv[])
{
  QCoreApplication::setOrganizationName ("BerndStramm");
  QCoreApplication::setOrganizationDomain ("bernd-stramm.com");
  QCoreApplication::setApplicationName ("souffleur");
  deliberate::ProgramVersion pv ("Souffleur");
  QCoreApplication::setApplicationVersion (pv.Version());
  QSettings  settings;
  deliberate::InitSettings ();
  deliberate::SetSettings (settings);
  settings.setValue ("program",pv.MyName());
 
  agenda::Notify::SetAppName (pv.MyName());
  
  char staticUri[] = "moui.geuzen.utils.static";
  
  qmlRegisterType<agenda::DateTimeChecker> (staticUri,1,0,"DateTimeChecker");
  qmlRegisterType<geuzen::OrientationWatcher> (staticUri,1,0,"GeuzenOrientation");

  QApplication  app (argc, argv);
  
  

  QStringList  configMessages;

  deliberate::CmdOptions  opts ("souffleur");
  opts.AddSoloOption ("debug","D",
          QObject::tr("show Debug log window"));
  opts.AddSoloOption ("phone","P",
          QObject::tr("use phone settings"));
  opts.AddSoloOption ("notphone","C",
          QObject::tr("use computer (non-phone) settings"));
  opts.AddStringOption ("logdebug","L",QObject::tr("write Debug log to file"));


  bool optsOk = opts.Parse (argc, argv);
  if ((opts.SeenOpt ("phone") && opts.SeenOpt("notphone")) || !optsOk) {
    opts.Usage ();
    exit(1);
  }
  if (opts.WantHelp ()) {
    opts.Usage ();
    exit (0);
  }
#if 0
  QSystemDeviceInfo sdi;

  QString imsi = sdi.imsi();
  QString imei = sdi.imei();
  bool isPhone (!(imsi.isEmpty() || imei.isEmpty()));
#else
  bool isPhone (false);
#endif

  isPhone |= opts.SeenOpt("phone");
  
  qDebug () << __PRETTY_FUNCTION__ << " phone ? " << isPhone;

  pv.CLIVersion ();
  configMessages.append (QString("Built on %1 %2")
                         .arg (__DATE__).arg(__TIME__));
  configMessages.append (QObject::tr("Build with Qt %1").arg(QT_VERSION_STR));
  configMessages.append (QObject::tr("Running with Qt %1").arg(qVersion()));
  for (int cm=0; cm<configMessages.size(); cm++) {
    deliberate::StdOut () << configMessages[cm] << endl;
  }
  if (opts.WantVersion ()) {
    exit (0);
  }
  bool showDebug = opts.SeenOpt ("debug");

  qDebug () << __PRETTY_FUNCTION__ << " gcc version " << GCC_VERSION;
  
  int result;
  
  if (isPhone) {
    QFont normalFont = app.font();
    normalFont.setPointSize (normalFont.pointSize() + 8);
    app.setFont (normalFont);
  }
  
  agenda::Agenda   agenda;
  
  agenda.setWindowIcon(QIcon(":/icons/windowicon.png"));
  app.setWindowIcon (agenda.windowIcon());
  agenda.Init (app,isPhone);
  agenda.AddConfigMessages (configMessages);
  
  agenda.Run ();
  result = app.exec ();
  qDebug () << " QApplication exec finished ";
  return result;
}
