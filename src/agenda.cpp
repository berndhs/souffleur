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
#include "notify.h"
#include "shell-launcher.h"
#include "helpview.h"
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
   notify (0),
   shellLauncher (0),
   helpView (0),
   dateForm ("ddd yyyy-MM-dd hh:mm:ss"),
   runAgain (false)
{
  dateForm = Settings().value ("display/dateform", dateForm)
                                  .toString();
  Settings().setValue ("display/dateform",QVariant (dateForm));
  mainUi.setupUi (this);
  itemEdit = new ItemEdit (this);
  itemEdit->hide ();
  scheduler = new AgendaScheduler (this);
  notify = new Notify (this);
  shellLauncher = new ShellLauncher (this);
  helpView = new HelpView (this);
  Connect ();
}

void
Agenda::Init (QApplication &ap)
{
  app = &ap;
  connect (app, SIGNAL (lastWindowClosed()), this, SLOT (Exiting()));
  Settings().sync();
  db.Start ();
  mainUi.activityList->Init (&db);
  scheduler->Init (&db);
  mainUi.calendarWidget->setVisible (false);
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
  connect (mainUi.actionLicense, SIGNAL (triggered()),
           this, SLOT (License ()));
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
  connect (itemEdit, SIGNAL (NewShell (AgendaShell)),
           this, SLOT (NewShell (AgendaShell)));
  connect (itemEdit, SIGNAL (NewRepeat (AgendaRepeat)),
           this, SLOT (NewRepeat (AgendaRepeat)));
  connect (mainUi.activityList, SIGNAL (DeleteEvent (QUuid)),
           this, SLOT (DeleteWanted (QUuid)));
  connect (scheduler, SIGNAL (CurrentEvent (AgendaEvent)),
           this, SLOT (LaunchEvent (AgendaEvent)));
  connect (scheduler, SIGNAL (NewShell (AgendaShell)),
           this, SLOT (LaunchShell (AgendaShell)));
  connect (scheduler, SIGNAL (Launched (int)),
           this, SLOT (Launched (int)));
}

void
Agenda::Minimize ()
{
  showMinimized ();
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
  CloseCleanup ();
  if (app) {
    app->quit();
  }
}

void
Agenda::closeEvent (QCloseEvent *event)
{
  Q_UNUSED (event)
  CloseCleanup ();
}

void
Agenda::CloseCleanup ()
{
  QSize currentSize = size();
  Settings().setValue ("sizes/main",currentSize);
  mainUi.activityList->SaveSettings ();
  db.Stop ();
  Settings().sync();
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
Agenda::Refresh ()
{
  mainUi.activityList->Load ();
  scheduler->Refresh ();
}

void
Agenda::DeleteWanted (QUuid uuid)
{
  db.DeleteAll (uuid);
  QTimer::singleShot (200,this, SLOT (Refresh()));
}

void
Agenda::NewEvent (AgendaEvent event)
{
  qDebug () << " new event " << event.Id() 
            << event.Nick() << event.Time() << event.Description();
  db.Write (event);
  QTimer::singleShot (200,this, SLOT (Refresh()));
}

void
Agenda::NewWarning (AgendaWarning warning)
{
  qDebug () << " new warning " << warning.Id() << " at " << warning.Time();
  db.Write (warning);
  QTimer::singleShot (200,this, SLOT (Refresh()));
}

void
Agenda::Launched (int howmany)
{
  if (howmany > 0) {
    mainUi.activityList->Load();
  }
}

void
Agenda::NewShell (AgendaShell shell)
{
  qDebug () << " new shell " << shell.Id() << " doing " << shell.Command ();
  db.Write (shell);
}

void
Agenda::NewRepeat (AgendaRepeat repeat)
{
  qDebug () << " new repeat " << repeat.Id() << " kind "
            << repeat.Kind () << " delay " << repeat.Delay ();
  db.Write (repeat);
}

void
Agenda::LaunchEvent (AgendaEvent event)
{
  if (notify) {
    notify->ShowMessage (event);
  }
}

void
Agenda::LaunchShell (AgendaShell shell)
{
qDebug () << " Agenda launch shell with " << shell.Command ();
  if (shellLauncher) {
    shellLauncher->Launch (shell);
  }
}

void
Agenda::CleanOld ()
{
  QDateTime now = QDateTime::currentDateTime();
  now.addDays (-2);
  db.DeleteOldEvents (now.toTime_t());
  QTimer::singleShot (200,this, SLOT (Refresh()));
}

void
Agenda::License ()
{
  if (helpView) {
    helpView->Show ("qrc:/help/LICENSE.txt");
  }
}


} // namespace

