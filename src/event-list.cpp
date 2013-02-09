
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
#include "magic-defs.h"
#include "event-list.h"
#include "deliberate.h"
#include <QHash>
#include <QDateTime>
#include <QDebug>
#include <QUuid>

#define AGENDA_PRETTY_DEBUG qDebug() << "__agenda__" << __PRETTY_FUNCTION__ 

using namespace deliberate;

namespace agenda
{

EventList::EventList (QObject *parent)
  :QAbstractListModel (parent),
   db (nullptr),
   dateForm ("ddd yyyy-MM-dd hh:mm:ss")
{
  AGENDA_PRETTY_DEBUG ;
  QHash<int, QByteArray> roles;
  roles[Type_When] = "eventWhen";
  roles[Type_Title] = "eventTitle";
  roles[Type_What] = "eventWhat";
  roles[Type_Audible] = "eventAudible";
  roles[Type_Repeating] = "eventRepeating";
  roles[Type_RepeatMinutes] = "eventRepeatMinutes";
  roles[Type_Uuid] = "eventUuid";
  roles[Type_Command] = "eventCommand";
  setRoleNames(roles);
}

void
EventList::Init (DBManager * dbm)
{
  db = dbm;
  dateForm = Settings().value ("display/dateform",dateForm).toString();
  Settings().setValue ("display/dateform",dateForm);
}

int
EventList::rowCount(const QModelIndex &index) const
{
  Q_UNUSED (index)
  int count = eventList.count();
  return count;
}

void
EventList::Refresh ()
{
  beginResetModel ();
  Load ();
  endResetModel ();
}
   
void
EventList::clear ()
{
  if (!eventList.isEmpty()) {
    beginRemoveRows(QModelIndex(),0,rowCount()-1);
    eventList.clear ();
    endRemoveRows();
  }
}

void
EventList::Load ()
{
  clear ();
  if (db) {
    int it = db->OpenReadEvents();
    if (it >= 0) {
      AgendaEvent event;
      while (db->ReadNext (it, event)) {
        AgendaRepeat repeat;
        if (db->Read (event.Id(),repeat)) {
          if (event.Id() == repeat.Id()) {
            repeats[event.Id()] = repeat;
          }
        }
        AgendaShell  shell;
        if (db->Read (event.Id(),shell)) {
          if (event.Id() == shell.Id()) {
            shells[event.Id()] = shell;
          }
        }
        Append (event);
      }
      db->CloseRead (it);
    }
  }
}

QVariant
EventList::data (const QModelIndex & index, int role) const
{
  int row = index.row();
  if (row < 0 || row > eventList.count()) {
    return QVariant();
  }
  switch (role) {
  case Type_Title:
    return QVariant (eventList.at(row).Nick());
    break;
  case Type_When:
    return QVariant (QDateTime::fromTime_t(eventList.at(row).Time())
                                .toString(dateForm));
    break;
  case Type_What:
    return QVariant (eventList.at(row).Description());
    break;
  case Type_Audible:
    return QVariant (eventList.at(row).Audible());
    break;
  case Type_Repeating:
    return QVariant (repeats.contains(eventList.at(row).Id()));
    break;
  case Type_RepeatMinutes:
    if (repeats.contains (eventList.at(row).Id())) {
      qDebug () << __PRETTY_FUNCTION__ << " repeat seconds " << repeats[eventList.at(row).Id()].Delay();
      return QVariant (repeats[eventList.at(row).Id()].DelayMinutes());
    } else {
      qDebug () << __PRETTY_FUNCTION__ << " NO REPEAT seconds";
      return QVariant (0);
    }
    break;
  case Type_Uuid:
    return QVariant (eventList.at(row).Id().toString());
    break;
  case Type_Command:
    if (shells.contains (eventList.at(row).Id())) {
      return QVariant (shells[eventList.at(row).Id()].Command());
    } else {
      return QVariant (QString());
    }
    break;
  default:
    break;
  }

  return QVariant();
}


void
EventList::Append (const AgendaEvent & event)
{
  beginInsertRows (QModelIndex(), rowCount(), rowCount());
  eventList.append (event);
  endInsertRows ();
}

void
EventList::Prepend (const AgendaEvent & event)
{
  beginInsertRows (QModelIndex(), 0, 0);
  eventList.prepend (event);
  endInsertRows ();
}

void
EventList::clearEvent (const QString & uuid)
{
  DeleteEvent (uuid);
}

void
EventList::DeleteEvent (const QString & uuid)
{
  QUuid id (uuid);
  for (int r=0; r<eventList.count(); r++) {
    if (eventList.at(r).Id() == id) {
      beginRemoveRows (QModelIndex(),r,r);
      eventList.removeAt(r);
      shells.remove (id);
      repeats.remove (id);
      endRemoveRows();
    }
  }
}


} // namespace
