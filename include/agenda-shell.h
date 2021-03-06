#ifndef AGENDA_SHELL_H
#define AGENDA_SHELL_H

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

class AgendaShell 
{
public:

  AgendaShell ()
    :uuid (QUuid::createUuid()), command (QString()) {}
  AgendaShell (const QUuid & id, const QString & cmd)
    :uuid (id), command (cmd) {}

  AgendaShell (const AgendaShell & old)
    :uuid (old.uuid), command (old.command) {}

  AgendaShell & operator= (const AgendaShell &other)
    {
      if (this != &other) {
        uuid = other.uuid;
        command = other.command;
      }
      return *this;
    }
  
  QUuid   Id   () const { return uuid; }
  QString Command () const { return command; }

  void SetId (const QUuid & u) { uuid = u; }
  void SetCommand (QString cmd) { command = cmd; }

private:

  QUuid      uuid;
  QString    command;
};

} // namespace

#endif

