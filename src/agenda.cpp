#include "agenda.h"

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
#include "magic-defs.h"
#include "deliberate.h"
#include "version.h"
#include "agenda-scheduler.h"
#include "notify.h"
#include "shell-launcher.h"
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
   db (this),
   itemEdit (nullptr),
   scheduler (nullptr),
   notify (nullptr),
   shellLauncher (nullptr),
   audioAlerter (nullptr),
   dateForm ("ddd yyyy-MM-dd hh:mm:ss"),
   runAgain (false),
   visibleBeforeEvent (false),
   qmlRoot (nullptr),
   events (nullptr),
   pastEvents (nullptr),
   assumePhone (false)
{
  AGENDA_PRETTY_DEBUG ;
  dateForm = Settings().value ("display/dateform", dateForm)
                                  .toString();
  Settings().setValue ("display/dateform",QVariant (dateForm));
  trayIcon = new QSystemTrayIcon (this);  
  trayIcon->setIcon (windowIcon());
  trayIcon->setToolTip (windowTitle());
  scheduler = new AgendaScheduler (this);
  notify = new Notify (this);
  notify->SetTrayIcon (trayIcon);
  shellLauncher = new ShellLauncher (this);
  events = new EventList (this);
  pastEvents = new EventList (this);
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
  scheduler->Init (&db);
  initDone = true;
  if (events) {
    events->Init (&db);
    events->setObjectName("Main_Events");
  }
  if (pastEvents) {
    pastEvents->Init ();
    pastEvents->setObjectName("Past_Events");
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
    context->setContextProperty ("cppPastEventListModel",pastEvents);
    context->setContextProperty ("isPhone",assumePhone);
  }
  AGENDA_PRETTY_DEBUG << " phone ? " << assumePhone;
  QSize normalSize (600,400);
  if (assumePhone) {
    showFullScreen();
    normalSize = size();
  } else {
    showNormal ();
    normalSize = Settings().value ("sizes/main", normalSize).toSize();
    resize (normalSize);
    Settings().setValue ("sizes/main",normalSize);
  }
  AGENDA_PRETTY_DEBUG << __LINE__ << " resize mode " << resizeMode();
  setResizeMode(QDeclarativeView::SizeRootObjectToView);
  AGENDA_PRETTY_DEBUG << __LINE__ << " resize mode " << resizeMode();
  setSource (QUrl ("qrc:///qml/main.qml"));
  setResizeMode(QDeclarativeView::SizeRootObjectToView);
  qmlRoot = qobject_cast<QDeclarativeItem*> (rootObject());
  AGENDA_PRETTY_DEBUG << __LINE__ << " resize mode " << resizeMode();
  AGENDA_PRETTY_DEBUG << " root " << qmlRoot;
  if (qmlRoot) {
    qmlRoot->setProperty("isPhone",assumePhone);
  }
  if (!assumePhone) {
    resize (normalSize);
  }
  AGENDA_PRETTY_DEBUG << " size "  << size() << " normal is " << normalSize;
  AGENDA_PRETTY_DEBUG << __LINE__ << " resize mode " << resizeMode();

  PrepareAbout ();
                        
  Connect ();
  show ();
  if (events) {
    events->Load();
  }
  if (!assumePhone) {
    trayIcon->show ();
  }
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
  bool ok = connect (qmlRoot, SIGNAL (saveNewEvent(const QString &,
                                         const QString &, const QString &, const QString&,
                                         const QString &, bool, qreal)),
           this, SLOT (SaveNewEvent(const QString &, const QString &, const QString &, const QString &,
                                    const QString &, bool, qreal)));
  qDebug () << " connect newEvent " << ok;
  connect (qmlRoot, SIGNAL (deleteEvent (const QString &)),
           this, SLOT (DeleteEvent (const QString&)));
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
  QMetaObject::invokeMethod(qmlRoot,"editSettings");
  SetSettings ();
}

void
Agenda::SetSettings ()
{
  Settings().sync ();
}

void
Agenda::ToggleCal ()
{
}

void
Agenda::PrepareAbout ()
{
  QString version (deliberate::ProgramVersion::Version());
  QStringList messages;
  messages.append (version);
  messages.append (configMessages);
  QMetaObject::invokeMethod (qmlRoot,"setAboutText",
                             Q_ARG (QVariant, QVariant (messages.join("\n"))));
  AGENDA_PRETTY_DEBUG << messages;
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
Agenda::DeleteEvent(const QString &uuid)
{
  db.DeleteAll(QUuid (uuid));
  events->DeleteEvent (uuid);
}

void
Agenda::SaveNewEvent(const QString & uuid,
                     const QString &title, const QString &time, const QString &description,
                     const QString & command, bool audible, qreal repeatMins)
{
  AGENDA_PRETTY_DEBUG << title << time << description << command << " repeat " << repeatMins;
  static QString format[2] = {QString ("yyyy-MM-dd hh:mm:ss"),
                        QString ("yyyy-MM-dd hh:mm"),
                      };
  bool foundGood (false);
  quint64 stamp;
  QString timeBuf (time);
  QRegExp leadingCrap ("\\D+");
  int pos = timeBuf.indexOf (leadingCrap);
  int crapLen = leadingCrap.cap().length();
  qDebug () << "     pos " << pos << " len " << crapLen;
  if (pos == 0) {
    timeBuf.remove (0,crapLen);
  }
  for (int a=0;a<2;a++) {
    QDateTime attempt (QDateTime::fromString (timeBuf, format[a]));
    qDebug () << "       " << attempt.isValid() << " for " << attempt;
    if (attempt.isValid()) {
      foundGood = true;
      stamp = attempt.toTime_t();
      break;
    }
  }
  if (!foundGood) {
    AGENDA_PRETTY_DEBUG << " bad date " << timeBuf;
    return;
  }
  qDebug () << " new event timeBuf " << timeBuf << " using " << dateForm;
  AgendaEvent event;
  if (uuid.isEmpty()) {
    event = AgendaEvent (title, stamp, description, audible);
  } else {
    bool ok = db.Read (QUuid(uuid), event);
    if (!ok) {
      event = AgendaEvent (title, stamp, description, audible);
    } else {
      db.DeleteAll (event.Id());
      event.SetNick (title);
      event.SetDescription (description);
      event.SetAudible (audible);
      event.SetTime (stamp);
    }
  }
  qDebug () << " converted to " << event.Time();
  NewEvent (event);
  if (!command.isEmpty()) {
    AgendaShell shellCommand (event.Id(), command);
    NewShell (shellCommand);
  }
  if (repeatMins > 0) {
    AgendaRepeat repeater (event.Id(),"min",repeatMins);
    NewRepeat(repeater);
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
  
  if (pastEvents) {
    pastEvents->Prepend (event);
  }
  if (notify) {
qDebug () << " Agenda::Launch Event visi " << isVisible () 
          << " mini " << isMinimized();
    bool oldVisi = isVisible ();
    bool oldMini = isMinimized ();
    notify->ShowMessage (event, oldVisi, oldMini);
  }
  AGENDA_PRETTY_DEBUG << audioAlerter;
  if (audioAlerter && event.Audible()) {
    if (assumePhone) {
      audioAlerter->setMedia (QUrl::fromLocalFile("/opt/souffleur/audio/alert.wav"));
    } else {
      audioAlerter->setMedia(QUrl("qrc:///alerts/alert.wav"));
    }
    audioAlerter->setVolume(50);
    QTimer::singleShot (1,audioAlerter,SLOT (play()));
    AGENDA_PRETTY_DEBUG << " alerter ok "  << audioAlerter->isAudioAvailable();
    AGENDA_PRETTY_DEBUG << " media   ok "  << audioAlerter->mediaStatus();
    AGENDA_PRETTY_DEBUG << " media duration "  << audioAlerter->duration();
    AGENDA_PRETTY_DEBUG << " alerter error " << audioAlerter->errorString();
    AGENDA_PRETTY_DEBUG << " media url " << audioAlerter->media().canonicalUrl();
  } 
  QMetaObject::invokeMethod(qmlRoot,"doAlert");
  if (assumePhone) {
    showFullScreen();
    show ();
    raise ();
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
  QMetaObject::invokeMethod (qmlRoot,"showLicense");
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

