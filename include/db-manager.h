#ifndef DB_MANAGER_H
#define DB_MANAGER_H

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
#include <QSqlDatabase>
#include "agenda-event.h"
#include "agenda-warning.h"
#include "agenda-shell.h"
#include "agenda-repeat.h"
#include <QMap>
#include <QObject>

namespace agenda
{
class DBManager : public QObject
{
Q_OBJECT

public:

  DBManager (QObject *parent = 0);
  ~DBManager ();

  void  Start ();
  void  Stop ();
  bool  Running () { return dbRunning; }

  bool  Write (const AgendaEvent & event);
  bool  Write (const AgendaWarning & warning);
  bool  Write (const AgendaShell & shell);
  bool  Write (const AgendaRepeat & repeat);
  bool  Read (const QUuid & id, AgendaEvent & event);
  bool  Read (const QUuid & id, AgendaShell & shell);
  bool  Read (const QUuid & id, AgendaRepeat & repeat);
  void  RemoveWarning (const AgendaWarning & warning);
  void  RemoveAllWarnings (const QUuid & uuid);
  void  RemoveShell (const QUuid & uuid);
  void  RemoveEvent (const QUuid & uuid);
  void  RemoveRepeat (const QUuid & uuid);

  void  RemoveOrphans ();

  int   NumOldEvents (quint64 beforeTime);

  void  DeleteOldEvents (quint64 beforeTime);
  void  DeleteAll (const QUuid & eventId);
  int   OpenReadEvents ();
  bool  ReadNext (int iteratorId, AgendaEvent & event);
  void  CloseRead (int iteratorId);
  int   OpenReadWarnings ();
  bool  ReadNext (int iteratorId, AgendaWarning & warn);

private:

  void StartDB (QSqlDatabase & db,
                    const QString & conName, 
                    const QString & dbFilename);
  void CheckFileExists (const QString & filename);
  void CheckDBComplete (QSqlDatabase & db,
                        const QStringList & elements);
  void CheckEventsComplete (QSqlDatabase & db,
                            const QString & tableName);
  QString ElementType (QSqlDatabase & db, const QString & name);
  void    MakeElement (QSqlDatabase & db, const QString & element);


  QSqlDatabase     eventDB;
  bool             dbRunning;

  QMap <int, QSqlQuery*>  readIterator;
  int                     nextIterator;
};

} // namespace

#endif
