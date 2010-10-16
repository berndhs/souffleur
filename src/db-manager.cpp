
#include "db-manager.h"

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

#include <QDesktopServices>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QVariant>
#include <QDebug>
#include "deliberate.h"

using namespace deliberate;

namespace agenda
{

DBManager::DBManager (QObject *parent)
  :QThread (parent),
   dbRunning (false),
   nextIterator (1)
{
}

DBManager::~DBManager ()
{
  if (dbRunning) {
    Stop ();
  }
}


void
DBManager::Start ()
{
  QThread::start();
  QString dataDir = QDesktopServices::storageLocation
                    (QDesktopServices::DataLocation);
  QString eventBasename = dataDir + QDir::separator() + QString ("events.sql");
  eventBasename = Settings().value ("database/events",eventBasename).toString();
  Settings().setValue ("database/events",eventBasename);
  Settings().sync ();
  

  StartDB (eventDB, "eventBaseCon", eventBasename);

  QStringList  eventElements;
  eventElements << "events"
                << "uniqueid";

  CheckDBComplete (eventDB, eventElements);
}

void
DBManager::Stop ()
{
  if (dbRunning) {
    dbRunning = false;
    eventDB.close ();
    QThread::exit ();
  }
}

void
DBManager::StartDB (QSqlDatabase & db,
                    const QString & conName, 
                    const QString & dbFilename)
{
  db = QSqlDatabase::addDatabase ("QSQLITE", conName);
  CheckFileExists (dbFilename);
  db.setDatabaseName (dbFilename);
  bool ok = db.open ();
qDebug () << " open db " << ok 
          << " dbname " << db.databaseName ()
          << " file " << dbFilename;
}

void
DBManager::CheckFileExists (const QString & filename)
{
  QFileInfo info (filename);
  if (!info.exists ()) {
    QDir  dir (info.absolutePath ());
    dir.mkpath (info.absolutePath ());
    QFile  file (filename);
    file.open (QFile::ReadWrite);
    file.write (QByteArray (""));
    file.close();
  }
}

void
DBManager::CheckDBComplete (QSqlDatabase & db,
                            const QStringList & elements)
{
  QString eltName, kind;
qDebug () << " checking DB for elements " << elements;
  for (int e=0; e<elements.size(); e++) {
    eltName = elements.at(e);
    kind = ElementType (db, eltName).toUpper();
qDebug () << " element " << eltName << " is kind " << kind;
    if (kind != "TABLE" && kind != "INDEX") {
      MakeElement (db, eltName);
    }
  }
}

QString
DBManager::ElementType (QSqlDatabase & db, const QString & name)
{
  QSqlQuery query (db);
  QString pat ("select * from main.sqlite_master where name=\"%1\"");
  QString cmd = pat.arg (name);
  bool ok = query.exec (cmd);
  if (ok && query.next()) {
    QString tipo = query.value(0).toString();
    return tipo;
  }
  return QString ("none");
}

bool
DBManager::Write (const AgendaEvent & event)
{
  QSqlQuery insert (eventDB);
  insert.prepare ("insert or replace into events "
                  " (eventid, nick, time, description) "
                  " values (?, ?, ?, ?)");
  insert.bindValue (0,QVariant (event.Id ()));
  insert.bindValue (1,QVariant (event.Nick ()));
  insert.bindValue (2,QVariant (event.Time ()));
  insert.bindValue (3,QVariant (event.Description ()));
  bool ok = insert.exec ();
  qDebug () << " db insert " << ok << insert.executedQuery();
  return ok;
}

void
DBManager::MakeElement (QSqlDatabase & db, const QString & element)
{
  QString filename (QString (":/schemas/%1.sql").arg (element));
  QFile  schemafile (filename);
  schemafile.open (QFile::ReadOnly);
  QByteArray  createcommands = schemafile.readAll ();
  schemafile.close ();
  QString cmd (createcommands);
  QSqlQuery query (db);
  query.prepare (cmd);
  bool ok = query.exec ();
qDebug () << " tried " << ok << " to create element with " 
          << query.executedQuery ();
}

int
DBManager::OpenReadEvents ()
{
  QSqlQuery *reader = new QSqlQuery (eventDB);
  int it = nextIterator;
  nextIterator++;
  bool ok = reader->exec (QString("select eventid, nick, time, description "
                        " from events where 1 "
                        " order by time ASC "));
  if (ok) {
    readIterator [it] = reader;
    return it;
  } else {
    return -1;
  }
}

void
DBManager::CloseReadEvents (int iteratorId)
{
  if (readIterator.contains (iteratorId)) {
    QSqlQuery *reader = readIterator [iteratorId];
    if (reader) {
      reader->clear();
      delete reader;
    }
    readIterator.remove (iteratorId);
  } 
}

bool
DBManager::ReadNext (int iteratorId, AgendaEvent & event)
{
  if (readIterator.contains (iteratorId)) {
    QSqlQuery * reader = readIterator [iteratorId];
    if (reader->next ()) {
      QUuid id = QUuid (reader->value (0).toString());
      QString nick = reader->value(1).toString();
      quint64 time = reader->value(2).toULongLong ();
      QString desc = reader->value(3).toString();
      event.SetId (id);
      event.SetNick (nick);
      event.SetTime (time);
      event.SetDescription (desc);
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void
DBManager::DeleteOldEvents (quint64 beforeTime)
{
  QSqlQuery erase (eventDB);
  QString cmd (QString("delete from events where time < %1")
                     .arg (beforeTime));
  erase.exec (cmd);
}


} // namespace

