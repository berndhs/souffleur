#ifndef AGENDA_EVENT_LIST_H
#define AGENDA_EVENT_LIST_H

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

#include "agenda-event.h"
#include "agenda-repeat.h"
#include "agenda-shell.h"
#include "db-manager.h"
#include <QAbstractListModel>
#include <QList>
#include <QMap>
#include <QUuid>
#include <QObject>
#include "magic-defs.h"

namespace agenda
{
class EventList : public QAbstractListModel
{
Q_OBJECT

public:

  EventList (QObject *parent=0);

  int rowCount (const QModelIndex & index = QModelIndex()) const;
  QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
  
  Q_INVOKABLE void clear ();
  Q_INVOKABLE void clearEvent (const QString & uuid);

  void Init (DBManager *dbm = nullptr);

  void Load ();
  void SaveSettings ();
  
  void Refresh ();
  
  void DeleteEvent (const QString & uuid);
  
  void Append (const AgendaEvent & event);
  void Prepend (const AgendaEvent & event);

private:

  enum DataType {
    Type_Title = Qt::UserRole+1,
    Type_When = Qt::UserRole+2,
    Type_What = Qt::UserRole+3,
    Type_Audible = Qt::UserRole+4,
    Type_Repeating = Qt::UserRole+5,
    Type_RepeatMinutes = Qt::UserRole+6,
    Type_Uuid = Qt::UserRole+7,
    Type_Command = Qt::UserRole+8
  };

  QList <AgendaEvent>              eventList;
  QMap <QUuid,AgendaRepeat>        repeats;
  QMap <QUuid,AgendaShell>         shells;
  DBManager                       *db;
  QString                          dateForm;


};

} // namespace

#endif
