#ifndef EVENT_TABLE_H
#define EVENT_TABLE_H

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

#include <QTableWidget>
#include <QVector>

namespace agenda
{

class DBManager;
class AgendaEvent;

class EventTable : public QTableWidget
{
Q_OBJECT

public:

  EventTable (QWidget *parent=0);

  void Init (DBManager *dbm);

  void Load ();
  void SaveSettings ();

private:

  enum DataRole {
      Role_Celltype = Qt::UserRole+1,
      Role_Uuid     = Qt::UserRole+2
      };

  enum CellType {
      Cell_None = 0,
      Cell_Nick = 1,
      Cell_Time = 2,
      Cell_Desc = 3
      };

  void DisplayEvent (int row, AgendaEvent & event);

  DBManager   *db;
  QString      dateForm;

};

} // namespace

#endif

