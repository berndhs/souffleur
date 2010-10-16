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
#include "agenda-event.h"
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
  void Launch ();
  void Launch (AgendaEvent & event);

signals:

  void CurrentEvent (AgendaEvent event);

private:
 
  typedef std::pair <quint64, AgendaEvent>      TimedEvent;
  typedef std::multimap <quint64, AgendaEvent>  EventScheduleMap;

  void LoadEvents ();
  void LoadEvent  (const AgendaEvent & event);

  DBManager    *db;
  QTimer       *pollTimer;

  EventScheduleMap    schedule;

};

} // namespace

#endif
