
#include "agenda-scheduler.h"

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

#include "db-manager.h"
#include "deliberate.h"
#include <QTimer>
#include <QDateTime>
#include <QDebug>

using namespace std;
using namespace deliberate;

namespace agenda
{

AgendaScheduler::AgendaScheduler (QObject *parent)
  :QObject (parent),
   db (0),
   pollTimer (0)
{
  pollTimer = new QTimer (this);
  connect (pollTimer, SIGNAL (timeout()), this, SLOT (Poll()));
}

void
AgendaScheduler::Init (DBManager * dbm)
{
  db = dbm;
  if (pollTimer) {
    int pollDelay (5*60);
    pollDelay = Settings().value ("timers/pollsecs",pollDelay).toInt();
    Settings().setValue ("timers/pollsecs",pollDelay);
    pollTimer->start (pollDelay*1000);
  }
}

void
AgendaScheduler::Start ()
{
  qDebug () << " AgendaScheduler start";
  LoadEvents ();
  Poll ();
}

void
AgendaScheduler::Refresh ()
{
  LoadEvents ();
  Poll ();
}

void
AgendaScheduler::LoadEvents ()
{
  schedule.clear ();
  if (db) {
    int it = db->OpenReadEvents();
    if (it >= 0) {
      AgendaEvent event;
      while (db->ReadNext (it, event)) {
        LoadEvent (event);
      }
      db->CloseReadEvents (it);
    }
  }
}

void
AgendaScheduler::LoadEvent (const AgendaEvent & event)
{
  TimedEvent te (event.Time(),event);
  schedule.insert (te);
  qDebug () << " loaded " << te.first;
  te.second.DebugDump ();
}

void
AgendaScheduler::Poll () 
{
  quint64 now = QDateTime::currentDateTime().toTime_t();
  qDebug () << " Poll at time " << now;
  EventScheduleMap::iterator it = schedule.begin();
  quint64 firstTime = it->first;
  if (firstTime <= now) {
    Launch ();
  } else {
    QTimer::singleShot (1000* (firstTime - now), this, SLOT (Launch ()));
  }
}

void
AgendaScheduler::Launch ()
{
  quint64 now = QDateTime::currentDateTime().toTime_t();
  EventScheduleMap::iterator it = schedule.begin();
  EventScheduleMap::iterator lastLaunched;
  int launchCount (0);
  while (it != schedule.end() && it->first < now) {
    Launch  (it->second);
    launchCount++;
    lastLaunched = it;
    it++;
  }
  if (launchCount > 0) {
    EventScheduleMap::iterator first = schedule.begin();
    if (first == lastLaunched) {
      qDebug () << " erase first " << first->first;
      schedule.erase (first);
    } else {
      qDebug () << " erase from " << first->first << " to " << lastLaunched->first;
      schedule.erase (first, it); 
    }
  }
  QTimer::singleShot (500,this,SLOT (Poll()));
}

void
AgendaScheduler::Launch (AgendaEvent & event)
{
  emit CurrentEvent (event);
}


} // namespace

