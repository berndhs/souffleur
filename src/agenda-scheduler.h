#ifndef AGENDA_SCHEDULER_H
#define AGENDA_SCHEDULER_H

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
#include <QObject>
#include <QSet>
#include "agenda-event.h"
#include "agenda-warning.h"
#include "agenda-shell.h"
#include <map>

class QTimer;

namespace agenda
{

class DBManager;

class AgendaScheduler : public QObject
{
Q_OBJECT

public:

  AgendaScheduler (QObject *parent=0);
  
  void Init (DBManager *dmb);

  void Start ();
  void Refresh ();

private slots:

  void Poll ();
  void LaunchFuture ();
  void LaunchPast ();
  void Launch (const QUuid & uuid, bool isEvent);

signals:

  void CurrentEvent (AgendaEvent event);
  void NewShell (AgendaShell shell);

private:
 
  typedef std::pair <quint64, AgendaWarning>      TimedEvent;
  typedef std::multimap <quint64, AgendaWarning>  EventScheduleMap;

  void LoadWarnings (bool initial = false);
  void LoadWarning  (EventScheduleMap & sched, const AgendaWarning & event);
  void Launch (EventScheduleMap & sched);

  void Dump ();

  DBManager    *db;
  QTimer       *pollTimer;
  int           pollDelay;
  quint64       nextPoll;

  EventScheduleMap    future;
  EventScheduleMap    past;
  QSet <QUuid>        launchSet;

};

} // namespace

#endif
