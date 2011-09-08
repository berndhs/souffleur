
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
#include "event-list.h"
#include "deliberate.h"
#include <QHash>
#include <QDateTime>
#include <QDebug>


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
  setRoleNames(roles);
}

void
EventList::Init (DBManager * dbm)
{
  AGENDA_PRETTY_DEBUG << dbm;
  db = dbm;
  dateForm = Settings().value ("display/dateform",dateForm).toString();
  Settings().setValue ("display/dateform",dateForm);
}

int
EventList::rowCount(const QModelIndex &index) const
{
  AGENDA_PRETTY_DEBUG << index;
  Q_UNUSED (index)
  int count = eventList.count();
  AGENDA_PRETTY_DEBUG << " return " << count;
  return count;
}
   
void
EventList::clear ()
{
  AGENDA_PRETTY_DEBUG ;
  if (!eventList.isEmpty()) {
    beginRemoveRows(QModelIndex(),0,rowCount()-1);
    eventList.clear ();
    endRemoveRows();
  }
}

void
EventList::Load ()
{
  AGENDA_PRETTY_DEBUG ;
  clear ();
  if (db) {
    int it = db->OpenReadEvents();
    if (it >= 0) {
      AgendaEvent event;
      while (db->ReadNext (it, event)) {
        addEvent (event);
      }
      db->CloseRead (it);
    }
  }
}

QVariant
EventList::data (const QModelIndex & index, int role) const
{
  AGENDA_PRETTY_DEBUG << index << role;
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
  default:
    break;
  }

  return QVariant();
}

void
EventList::addEvent (const AgendaEvent & event)
{
  beginInsertRows (QModelIndex(), rowCount(), rowCount());
  eventList.append (event);
  endInsertRows ();
}


} // namespace
