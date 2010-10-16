
#include "event-table.h"

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
#include "agenda-event.h"
#include <QTableWidgetItem>
#include <QDateTime>
#include <QDebug>

namespace agenda
{

EventTable::EventTable (QWidget *parent)
  :QTableWidget (parent)
{
}

void
EventTable::Init (DBManager *dbm)
{
  db = dbm;
}

void
EventTable::Load ()
{
  clearContents ();
  setRowCount (0);
  int row (0);
  if (db) {
    int it = db->OpenReadEvents();
    if (it >= 0) {
      AgendaEvent event;
      while (db->ReadNext (it, event)) {
        setRowCount (row+1);
        DisplayEvent (row, event);
        row++;
      }
      db->CloseReadEvents (it);
    }
  }
}

void
EventTable::DisplayEvent (int row, AgendaEvent & event)
{
  qDebug () << " display event " ; event.DebugDump ();
  QTableWidgetItem *item (0);
  item = new QTableWidgetItem (event.Nick());
  item->setData (Role_Celltype, Cell_Nick);
  item->setData (Role_Uuid, event.Id().toString());
  setItem (row,0,item);
  QString date = QDateTime::fromTime_t (event.Time()).toString();
  item = new QTableWidgetItem (date);
  item->setData (Role_Celltype, Cell_Time);
  setItem (row,1,item);
  item = new QTableWidgetItem (event.Description ());
  item->setData (Role_Celltype, Cell_Desc);
  setItem (row,2,item);
}


} // namespace

