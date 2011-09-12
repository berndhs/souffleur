#ifndef AGENDA_REPEAT_H
#define AGENDA_REPEAT_H

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

class AgendaRepeat 
{
public:

  AgendaRepeat ()
    :uuid (QUuid::createUuid()), kind (QString()), delay (0) {}
  AgendaRepeat (const QUuid & id, QString k, quint64 d)
    :uuid (id), kind (k), delay (d) {}

  AgendaRepeat (const AgendaRepeat & old)
    :uuid (old.uuid), kind (old.kind), delay (old.delay) {}

  AgendaRepeat & operator= (const AgendaRepeat &other)
    {
      if (this != &other) {
        uuid = other.uuid;
        kind = other.kind;
        delay = other.delay;
      }
      return *this;
    }
  
  QUuid   Id   () const { return uuid; }
  QString Kind () const { return kind; }
  quint64 Delay () const { return delay; }
  quint64 DelayMinutes () const;

  void SetId (const QUuid & u) { uuid = u; }
  void SetKind (const QString & k) { kind = k; }
  void SetDelay (const quint64 d) { delay = d; }

private:

  QUuid      uuid;
  QString    kind;
  quint64    delay;

};

} // namespace

#endif

