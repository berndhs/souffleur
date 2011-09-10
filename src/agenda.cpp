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
#include "magic-defs.h"
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
#include <QDeclarativeContext>
#include <QMediaPlayer>

#define AGENDA_PRETTY_DEBUG qDebug() << "__agenda__" << __PRETTY_FUNCTION__ 

using namespace deliberate;

namespace agenda
{

Agenda::Agenda (QWidget *parent)
  :QDeclarativeView (parent),
   initDone (false),
   app (nullptr),
   trayIcon (nullptr),
   configEdit (this),
   db (this),
   itemEdit (nullptr),
   scheduler (nullptr),
   notify (nullptr),
   shellLauncher (nullptr),
   audioAlerter (nullptr),
   helpView (nullptr),
   dateForm ("ddd yyyy-MM-dd hh:mm:ss"),
   runAgain (false),
   visibleBeforeEvent (false),
   qmlRoot (nullptr),
   events (nullptr),
   assumePhone (false)
{
  AGENDA_PRETTY_DEBUG ;
  dateForm = Settings().value ("display/dateform", dateForm)
                                  .toString();
  Settings().setValue ("display/dateform",QVariant (dateForm));
  trayIcon = new QSystemTrayIcon (this);  
  trayIcon->setIcon (windowIcon());
  trayIcon->setToolTip (windowTitle());
  itemEdit = new ItemEdit (this);
  itemEdit->hide ();
  scheduler = new AgendaScheduler (this);
  notify = new Notify (this);
  notify->SetTrayIcon (trayIcon);
  shellLauncher = new ShellLauncher (this);
  helpView = new HelpView (this);
  events = new EventList (this);
  audioAlerter = new QMediaPlayer (this);
}

void
Agenda::Init (QApplication &ap, bool isPhone)
{
  app = &ap;
  assumePhone = isPhone;
  connect (app, SIGNAL (lastWindowClosed()), this, SLOT (Exiting()));
  Settings().sync();
  db.Start ();
  //mainUi.activityList->Init (&db);
  scheduler->Init (&db);
  //mainUi.calendarWidget->setVisible (false);
  initDone = true;
  if (events) {
    events->Init (&db);
  }
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
  QDeclarativeContext * context = rootContext ();
  if (context) {
    context->setContextProperty ("cppEventListModel",events);
  }
  setSource (QUrl ("qrc:///qml/main.qml"));
  qmlRoot = qobject_cast<QDeclarativeItem*> (rootObject());
  AGENDA_PRETTY_DEBUG << " root " << qmlRoot;
  setResizeMode(QDeclarativeView::SizeRootObjectToView);
  Connect ();
  show ();
  if (events) {
    events->Load();
  }
  if (assumePhone) {
    showFullScreen();
  } else {
    QSize defaultSize = size();
    QSize newsize = Settings().value ("sizes/main", defaultSize).toSize();
    resize (newsize);
    Settings().setValue ("sizes/main",newsize);
  }
  show ();
  trayIcon->show ();
  //mainUi.activityList->Load ();
  if (scheduler) {
    scheduler->Start ();
  }
  QDateTime now = QDateTime::currentDateTime();
  Revive ();
  return true;
}

void
Agenda::Connect ()
{
  connect (qmlRoot, SIGNAL (quit()),this, SLOT(Quit()));
  bool ok = connect (qmlRoot, SIGNAL (saveNewEvent(const QString &, const QString &, const QString&,
                                         const QString &, bool)),
           this, SLOT (SaveNewEvent(const QString &, const QString &, const QString &,
                                    const QString &, bool)));
  qDebug () << " connect newEvent " << ok;
  connect (scheduler, SIGNAL (CurrentEvent (AgendaEvent)),
           this, SLOT (LaunchEvent (AgendaEvent)));
  connect (scheduler, SIGNAL (NewShell (AgendaShell)),
           this, SLOT (LaunchShell (AgendaShell)));
  connect (scheduler, SIGNAL (Launched (int)),
           this, SLOT (Launched (int)));
  connect (scheduler, SIGNAL (CheckAgain (const QDateTime &)),
           this, SLOT (UpdateToolTip (const QDateTime &)));
  connect (notify, SIGNAL (MessageDone(bool, bool)),
           this, SLOT (RestoreVisible (bool, bool)));
  if (!assumePhone) {
    connect (trayIcon,SIGNAL (activated ( QSystemTrayIcon::ActivationReason)),
           this, SLOT (TrayIconActive (QSystemTrayIcon::ActivationReason)));
  }
#if 0
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
  connect (mainUi.actionRevive, SIGNAL (triggered()),
           this, SLOT (Revive ()));
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
#endif
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
  //mainUi.activityList->SaveSettings ();
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
  bool seeit = false; //mainUi.calendarWidget->isVisible ();
 // mainUi.calendarWidget->setVisible (!seeit);
  
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
  events->Refresh ();
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
  AGENDA_PRETTY_DEBUG << event.Id() 
            << event.Nick() << event.Time() << event.Description();
  db.Write (event);
  QTimer::singleShot (200,this, SLOT (Refresh()));
}

void
Agenda::NewWarning (AgendaWarning warning)
{
  AGENDA_PRETTY_DEBUG << warning.Id() << " at " << warning.Time();
  db.Write (warning);
  QTimer::singleShot (200,this, SLOT (Refresh()));
}

void
Agenda::Launched (int howmany)
{
  if (howmany > 0) {
    events->Refresh();
  }
}

void
Agenda::SaveNewEvent(const QString &title, const QString &time, const QString &description,
                     const QString & command, bool audible)
{
  AGENDA_PRETTY_DEBUG << title << time << description << command;
  QString format1 ("yyyy-MM-dd hh:mm:ss");
  QString format2 ("yyyy-MM-dd hh:mm");
  QDateTime attempt (QDateTime::fromString(time,format1));
  quint64 stamp;
  if (attempt.isValid()) {
    stamp = attempt.toTime_t();
  } else {
    attempt = QDateTime::fromString (time,format2);
    if (attempt.isValid()) {
      stamp = attempt.toTime_t();
    } else {
      return; // bad date, give up
    }
  }
  qDebug () << " new event time " << time << " using " << dateForm;
  AgendaEvent event (title, stamp, description, audible);
  qDebug () << " converted to " << event.Time();
  NewEvent (event);
  if (!command.isEmpty()) {
    AgendaShell shellCommand (event.Id(), command);
    NewShell (shellCommand);
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
  AGENDA_PRETTY_DEBUG;
  
  if (notify) {
qDebug () << " Agenda::Launch Event visi " << isVisible () 
          << " mini " << isMinimized();
    bool oldVisi = isVisible ();
    bool oldMini = isMinimized ();
    notify->ShowMessage (event, oldVisi, oldMini);
  }
  AGENDA_PRETTY_DEBUG << audioAlerter;
  if (audioAlerter) {
    if (assumePhone) {
      audioAlerter->setMedia (QUrl::fromLocalFile("/opt/souffleur/audio/alert.wav"));
    } else {
      audioAlerter->setMedia(QUrl("qrc:///alerts/alert.wav"));
    }
    audioAlerter->setVolume(50);
    QTimer::singleShot (1,audioAlerter,SLOT (play()));
    if (assumePhone) {
      showFullScreen();
      show ();
      raise ();
    }
    AGENDA_PRETTY_DEBUG << " alerter ok "  << audioAlerter->isAudioAvailable();
    AGENDA_PRETTY_DEBUG << " media   ok "  << audioAlerter->mediaStatus();
    AGENDA_PRETTY_DEBUG << " media duration "  << audioAlerter->duration();
    AGENDA_PRETTY_DEBUG << " alerter error " << audioAlerter->errorString();
    AGENDA_PRETTY_DEBUG << " media url " << audioAlerter->media().canonicalUrl();
  } else {
    AGENDA_PRETTY_DEBUG << " no alerter !!";
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
Agenda::RestoreVisible (bool oldVisible, bool oldMinimized)
{
  qDebug () << "Agenda::RestoreVisible " << oldVisible << oldMinimized;
  setVisible (oldVisible);
  if (oldMinimized) {
    showMinimized ();
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
Agenda::Revive ()
{
  if (scheduler) {
    scheduler->Revive ();
    Refresh ();
  }
}

void
Agenda::AskRevive (int numOld)
{
  QMessageBox::StandardButton dorevive =
      QMessageBox::question (this, tr("Revive Old Events ?"),
                    tr("There are %1 expired events, \n"
                       "Do you want to bring them up to date?")
                       .arg (numOld),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No);
  if (dorevive == QMessageBox::Yes) {
    Revive ();
  }
}

void
Agenda::License ()
{
  if (helpView) {
    helpView->Show ("qrc:/help/LICENSE.txt");
  }
}

void
Agenda::UpdateToolTip (const QDateTime & nextEvent)
{
  if (trayIcon) {
    trayIcon->setToolTip (windowTitle()
        + tr (" \n next at %1")
         .arg (nextEvent.toString()));
  }
}

void
Agenda::TrayIconActive (QSystemTrayIcon::ActivationReason reason)
{
  Q_UNUSED (reason)
  if (isMinimized()) {
    showNormal ();
  }
  raise ();
}


} // namespace

