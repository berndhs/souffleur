
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
   pollTimer (0),
   pollDelay (5*60),
   nextPoll (0)
{
  pollTimer = new QTimer (this);
  connect (pollTimer, SIGNAL (timeout()), this, SLOT (Poll()));
}

void
AgendaScheduler::Init (DBManager * dbm)
{
  db = dbm;
  if (pollTimer) {
    pollDelay  = 5*60 ;
    pollDelay = Settings().value ("timers/pollsecs",pollDelay).toInt();
    Settings().setValue ("timers/pollsecs",pollDelay);
    pollTimer->start (pollDelay*1000);
    nextPoll = QDateTime::currentDateTime().toTime_t() + pollDelay;
  }
}

void
AgendaScheduler::Start ()
{
  qDebug () << " AgendaScheduler start";
  LoadWarnings (true);
  Poll ();
}

void
AgendaScheduler::Refresh ()
{
  LoadWarnings ();
  Poll ();
}

void
AgendaScheduler::LoadWarnings (bool initial)
{
  quint64 now = QDateTime::currentDateTime().toTime_t();
  future.clear ();
  if (db) {
    int it = db->OpenReadWarnings ();
    if (it >= 0) {
      AgendaWarning warn;
      while (db->ReadNext (it, warn)) {
        if (initial && warn.Time() < now) {
          LoadWarning (past, warn);
        } else {
          LoadWarning (future, warn);
        }
      }
      db->CloseRead (it);
    }
  }
}

void
AgendaScheduler::LoadWarning (EventScheduleMap & sched, 
                              const AgendaWarning & warn)
{
  TimedEvent te (warn.Time(),warn);
  sched.insert (te);
  qDebug () << " loaded " << te.first << " for  " << te.second.Id();
}

void
AgendaScheduler::Poll () 
{
  quint64 now = QDateTime::currentDateTime().toTime_t();
  nextPoll = now + pollDelay;
  qDebug () << " Poll at time " << now;
  EventScheduleMap::iterator it = future.begin();
  quint64 firstTime = it->first;
  if (firstTime <= now) {
    Launch (future);
  } else if (firstTime < nextPoll) {
    qDebug () << "launch single shot in " << 1000 * (firstTime - now);
    QTimer::singleShot (1000 * (firstTime - now), this, SLOT (LaunchFuture ()));
  }
  launchSet.clear ();
}

void
AgendaScheduler::LaunchFuture ()
{
  Launch (future);
}

void
AgendaScheduler::LaunchPast ()
{
  Launch (past);
}

void
AgendaScheduler::Launch (EventScheduleMap & sched)
{
  launchSet.clear ();
  quint64 now = QDateTime::currentDateTime().toTime_t();
  EventScheduleMap::iterator it = sched.begin();
  EventScheduleMap::iterator lastLaunched;
  int launchCount (0);
  qDebug () << "first time " << it->first << " now " << now;
  while (it != sched.end() && it->first <= now) {
    Launch  (it->second.Id(), it->second.IsEvent());
    launchCount++;
    lastLaunched = it;
    it++;
  }
  if (launchCount > 0) {
    EventScheduleMap::iterator first = sched.begin();
    if (first == lastLaunched) {
      qDebug () << " erase first " << first->first;
      future.erase (first);
    } else {
      qDebug () << " erase from " << first->first << " to " << lastLaunched->first;
      sched.erase (first, it); 
    }
  }
  // check it there is more before the next poll time
  it = sched.begin ();
  if (it != sched.end()) {
    quint64 next = it->first;
    // if event is in the future, but before next poll,
    // we need to launch again
    if (next > now && next < nextPoll) {
      now = QDateTime::currentDateTime().toTime_t();
      quint64 delay = next - now;
      QTimer::singleShot (delay * 1000, this, SLOT (LaunchFuture()));
    }
  }
}

void
AgendaScheduler::Launch (const QUuid & uuid, bool isEvent)
{
  AgendaEvent  event;
  if (!launchSet.contains (uuid)) {
    if (db->Read (uuid, event)) {
      launchSet.insert (uuid);
      emit CurrentEvent (event);
qDebug () << " launched event " << uuid ;
qDebug () << "          original event " << isEvent;
      if (isEvent) {
        AgendaShell shell;
        bool hasShell = db->Read (uuid, shell);
qDebug () << " event has command " << hasShell << shell.Command ();
        if (hasShell) {
          emit NewShell (shell);
        }
      }
    }
  }
}

void
AgendaScheduler::Dump ()
{
  EventScheduleMap::iterator  it;
  qDebug () << "Past Sched";
  for (it = past.begin(); it != past.end(); it++) {
    qDebug () << " for t=" << it->first << " uuid " << it->second.Id();
  }
  qDebug () << "Future Sched";
  for (it = future.begin(); it != future.end(); it++) {
    qDebug () << " for t=" << it->first << " uuid " << it->second.Id();
  }
}


} // namespace

