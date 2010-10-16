
#include "agenda-event.h"

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

#include <QDebug>

namespace agenda
{

AgendaEvent::AgendaEvent ()
  :uuid (QUuid::createUuid()),
   timestamp (0)
{
}

AgendaEvent::AgendaEvent (const QString & n, quint64 time, const QString & desc)
  :uuid (QUuid::createUuid()),
   nick (n),
   timestamp (time),
   description (desc)
{
}
   

AgendaEvent::AgendaEvent (const AgendaEvent & old)
  :uuid (old.uuid),
   nick (old.nick),
   timestamp (old.timestamp),
   description (old.description)
{
}

AgendaEvent &
AgendaEvent::operator = (const AgendaEvent & other)
{
  if (this != &other) {
    uuid = other.uuid;
    nick = other.nick;
    timestamp = other.timestamp;
    description = other.description;
  }
  return *this;
}

void
AgendaEvent::DebugDump ()
{
  qDebug () << " AgendaEvent ( " 
            << uuid << nick << timestamp << description 
            << " ) ";
}

} // namespace

