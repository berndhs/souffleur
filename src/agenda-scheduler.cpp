
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
   pollDelay (5*60)
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
  }
}

void
AgendaScheduler::Start ()
{
  qDebug () << " AgendaScheduler start";
  LoadWarnings ();
  Poll ();
}

void
AgendaScheduler::Refresh ()
{
  LoadWarnings ();
  Poll ();
}

void
AgendaScheduler::LoadWarnings ()
{
  schedule.clear ();
  if (db) {
    int it = db->OpenReadWarnings ();
    if (it >= 0) {
      AgendaWarning warn;
      while (db->ReadNext (it, warn)) {
        LoadWarning (warn);
      }
      db->CloseRead (it);
    }
  }
}

void
AgendaScheduler::LoadWarning (const AgendaWarning & warn)
{
  TimedEvent te (warn.Time(),warn.Id());
  schedule.insert (te);
  qDebug () << " loaded " << te.first << " for  " << te.second;
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
  } else if (firstTime < now + (pollDelay*1000)) {
    qDebug () << "launch single shot in " << 1000 * (firstTime - now);
    QTimer::singleShot (1000 * (firstTime - now), this, SLOT (Launch ()));
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
}

void
AgendaScheduler::Launch (QUuid & uuid)
{
  AgendaEvent  event;
  if (db->Read (uuid, event)) {
    emit CurrentEvent (event);
  }
}


} // namespace

