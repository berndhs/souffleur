#include "agenda.h"

/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2010, Bernd Stramm
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
#include "deliberate.h"
#include "version.h"
#include "item-edit.h"
#include "agenda-scheduler.h"
#include <QSize>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QCursor>

using namespace deliberate;

namespace agenda
{

Agenda::Agenda (QWidget *parent)
  :QMainWindow (parent),
   initDone (false),
   app (0),
   configEdit (this),
   db (this),
   itemEdit (0),
   scheduler (0),
   dateForm ("ddd yyyy-MM-dd hh:mm:ss"),
   runAgain (false)
{
  mainUi.setupUi (this);
  itemEdit = new ItemEdit (this);
  itemEdit->hide ();
  scheduler = new AgendaScheduler (this);
  Connect ();
}

void
Agenda::Init (QApplication &ap)
{
  app = &ap;
  connect (app, SIGNAL (lastWindowClosed()), this, SLOT (Exiting()));
  db.Start ();
  mainUi.activityList->Init (&db);
  scheduler->Init (&db);
  mainUi.calendarWidget->setVisible (false);
  dateForm = Settings().value ("display/dateform",dateForm).toString();
  Settings().setValue ("display/dateform",dateForm);
  initDone = true;
}

bool
Agenda::Again ()
{
  bool again = runAgain;
  runAgain = false;
  return again;
}

bool
Agenda::Run ()
{
  runAgain = false;
  if (!initDone) {
    Quit ();
    return false;
  }
  qDebug () << " Start Agenda";
  QSize defaultSize = size();
  QSize newsize = Settings().value ("sizes/main", defaultSize).toSize();
  resize (newsize);
  Settings().setValue ("sizes/main",newsize);
  show ();
  mainUi.activityList->Load ();
  if (scheduler) {
    scheduler->Start ();
  }
  return true;
}

void
Agenda::Connect ()
{
  connect (mainUi.actionQuit, SIGNAL (triggered()), this, SLOT (Quit()));
  connect (mainUi.actionSettings, SIGNAL (triggered()),
           this, SLOT (EditSettings()));
  connect (mainUi.actionAbout, SIGNAL (triggered()),
           this, SLOT (About ()));
  connect (mainUi.actionNewItem, SIGNAL (triggered()),
           this, SLOT (NewItem()));
  connect (mainUi.calendarWidget, SIGNAL (clicked(const QDate &)),
           this, SLOT (PickedDate (const QDate &)));
  connect (mainUi.hideButton, SIGNAL (clicked()), 
           this, SLOT (Minimize()));
  connect (mainUi.quitButton, SIGNAL (clicked()),
           this, SLOT (Quit()));
  connect (mainUi.calButton, SIGNAL (clicked()),
           this, SLOT (ToggleCal ()));
  connect (mainUi.actionClearOld, SIGNAL (triggered()),
           this, SLOT (CleanOld ()));
  connect (mainUi.actionRestart, SIGNAL (triggered()),
           this, SLOT (Restart ()));
  connect (itemEdit, SIGNAL (NewEvent (AgendaEvent)),
           this, SLOT (NewEvent (AgendaEvent)));
  connect (itemEdit, SIGNAL (NewWarning (AgendaWarning)),
           this, SLOT (NewWarning (AgendaWarning)));
  connect (scheduler, SIGNAL (CurrentEvent (AgendaEvent)),
           this, SLOT (LaunchEvent (AgendaEvent)));
}

void
Agenda::Minimize ()
{
  showMinimized ();
  QTimer::singleShot (5000,this, SLOT (Popup()));
}

void
Agenda::Restart ()
{
  qDebug () << " restart called ";
  runAgain = true;
  Quit ();
}


void
Agenda::Quit ()
{
  QSize currentSize = size();
  Settings().setValue ("sizes/main",currentSize);
  Settings().sync();
  if (app) {
    app->quit();
  }
  db.Stop ();
}

void
Agenda::EditSettings ()
{
  configEdit.Exec ();
  SetSettings ();
}

void
Agenda::SetSettings ()
{
  Settings().sync ();
}

void
Agenda::NewItem ()
{ 
  itemEdit->NewItem ();
}

void
Agenda::PickedDate (const QDate & date)
{
  itemEdit->NewItem (date);
}

void
Agenda::ToggleCal ()
{
  bool seeit = mainUi.calendarWidget->isVisible ();
  mainUi.calendarWidget->setVisible (!seeit);
  
}

void
Agenda::About ()
{
  QString version (deliberate::ProgramVersion::Version());
  QStringList messages;
  messages.append (version);
  messages.append (configMessages);

  QMessageBox  box;
  box.setText (version);
  box.setDetailedText (messages.join ("\n"));
  QTimer::singleShot (30000, &box, SLOT (accept()));
  box.exec ();
}

void
Agenda::Exiting ()
{
  QSize currentSize = size();
  Settings().setValue ("sizes/main",currentSize);
  Settings().sync();
}

void
Agenda::Popup ()
{
  qDebug () << " popup";
  QMessageBox  box (this);
  box.setWindowTitle ("Agenda Item Popup");
  box.setText ("Agenda Item");
  box.setIcon (QMessageBox::Information);
  QTimer::singleShot (10000, &box, SLOT (accept()));
  box.exec (); 
}

void
Agenda::NewEvent (AgendaEvent event)
{
  qDebug () << " new event " << event.Id() 
            << event.Nick() << event.Time() << event.Description();
  db.Write (event);
  mainUi.activityList->Load ();
  scheduler->Refresh ();
}

void
Agenda::NewWarning (AgendaWarning warning)
{
  qDebug () << " new warning " << warning.Id() << " at " << warning.Time();
  db.Write (warning);
  scheduler->Refresh ();
}

void
Agenda::LaunchEvent (AgendaEvent event)
{
  QMessageBox box (this);
  QStringList mlist;
  mlist << event.Id().toString();
  mlist << event.Nick ();
  mlist << QDateTime::fromTime_t(event.Time()).toString("hh:mm:ss");
  mlist << event.Description ();
  box.setText (mlist.join ("\n"));
  qDebug () << "Launching Event " << mlist;
  QTimer::singleShot (10000, &box, SLOT (accept()));
  box.exec ();
}

void
Agenda::CleanOld ()
{
  QDateTime now = QDateTime::currentDateTime();
  now.addDays (-2);
  db.DeleteOldEvents (now.toTime_t());
  mainUi.activityList->Load ();
}


} // namespace

