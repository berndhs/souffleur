#ifndef AGENDA_WARNING_H
#define AGENDA_WARNING_H

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

class AgendaWarning 
{
public:

  AgendaWarning ()
    :uuid (QUuid::createUuid()), timestamp (0), isEvent (false) {}
  AgendaWarning (const QUuid & id, quint64 time, bool forReal = false)
    :uuid (id), timestamp (time), isEvent (forReal) {}

  AgendaWarning (const AgendaWarning & old)
    :uuid (old.uuid), timestamp (old.timestamp), isEvent (old.isEvent) {}

  AgendaWarning & operator= (const AgendaWarning &other)
    {
      if (this != &other) {
        uuid = other.uuid;
        timestamp = other.timestamp;
        isEvent = other.isEvent;
      }
      return *this;
    }
  
  QUuid   Id   () const { return uuid; }
  quint64 Time () const { return timestamp; }
  bool    IsEvent () const { return isEvent; }

  void SetId (const QUuid & u) { uuid = u; }
  void SetTime (quint64 t) { timestamp = t; }
  void SetIsEvent (bool is) { isEvent = is; }

private:

  QUuid      uuid;
  quint64    timestamp;
  bool       isEvent;
};

} // namespace

#endif

