#ifndef AGENDA_H
#define AGENDA_H

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
#include <QDeclarativeView>
#include <QSystemTrayIcon>
#include <QDateTime>
#include "db-manager.h"
#include "agenda-event.h"
#include "agenda-warning.h"
#include "agenda-shell.h"
#include "event-list.h"

#include <QDeclarativeItem>

class QApplication;
class QDate;
class QMediaPlayer;
class QEvent;


namespace agenda 
{

class ItemEdit;
class AgendaScheduler;
class Notify;
class ShellLauncher;

class Agenda : public QDeclarativeView
{
Q_OBJECT

public:

  Agenda (QWidget *parent=0);

  void  Init (QApplication &ap, bool isPhone=false);
  bool  Run ();
  bool  Again ();

  void  AddConfigMessages (const QStringList & cm) 
           { configMessages.append (cm); }


protected:

  bool event (QEvent *event);
  void closeEvent ( QCloseEvent *event);

private slots:

  void Quit ();
  void Restart ();
  void EditSettings ();
  void SetSettings ();
  void About ();
  void License ();
  void Exiting ();
  void ToggleCal ();
  void Minimize ();
  void CleanOld ();
  void Revive ();

  void Refresh ();

  void DeleteWanted (QUuid uuid);

  void NewEvent (AgendaEvent event);
  void NewWarning (AgendaWarning warning);
  void NewShell (AgendaShell shell);
  void NewRepeat (AgendaRepeat repeat);
  void LaunchEvent (AgendaEvent event);
  void LaunchShell (AgendaShell shell);

  void Launched (int howmany);
  void RestoreVisible (bool oldVisible, bool oldMinimized);

  void UpdateToolTip (const QDateTime & nextEvent);
  void TrayIconActive (QSystemTrayIcon::ActivationReason reason);
  
  void SaveNewEvent (const QString & uuid,
                     const QString & title, 
                     const QString & time, 
                     const QString & description,
                     const QString & command,
                     bool  audible,
                     qreal repeatMins);
  void DeleteEvent (const QString & uuid);

private:

  void Connect ();
  void CloseCleanup ();
  void AskRevive (int numOld);
  void PrepareAbout ();

  bool             initDone;
  QApplication    *app;
  QSystemTrayIcon *trayIcon;
 
  QStringList      configMessages;

  DBManager        db;
  ItemEdit        *itemEdit;
  AgendaScheduler *scheduler;
  Notify          *notify;
  ShellLauncher   *shellLauncher;
  QMediaPlayer    *audioAlerter;
  QString          dateForm;
  bool             runAgain;
  bool             visibleBeforeEvent;

  QDeclarativeItem * qmlRoot;
  EventList        * events;
  EventList        * pastEvents;
  
  bool             assumePhone;


};

} // namespace

#endif
