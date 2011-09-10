#ifndef AGENDA_EVENT_H
#define AGENDA_EVENT_H

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

#include <QString>
#include <QUuid>

namespace agenda
{

class AgendaEvent 
{
public:

  AgendaEvent ();
  AgendaEvent (const QString & n, quint64 time, const QString & desc, bool playSound);

  AgendaEvent (const AgendaEvent & old);
  AgendaEvent & operator= (const AgendaEvent &other);
  
  QUuid   Id   () const { return uuid; }
  QString Nick () const { return nick; }
  quint64 Time () const { return timestamp; }
  QString Description () const { return description; }
  bool    Audible () const { return audible; }

  void SetId (const QUuid & u) { uuid = u; }
  void SetNick (const QString & n) { nick = n; }
  void SetTime (quint64 t) { timestamp = t; }
  void SetDescription (const QString & d) { description = d; }
  void SetAudible (bool a) { audible = a; }

  void DebugDump ();


private:

  QUuid      uuid;
  QString    nick;
  quint64    timestamp;
  QString    description;
  bool       audible;
};

} // namespace

#endif

