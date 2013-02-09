
#include "shell-launcher.h"

/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
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

namespace agenda
{

/** AgendaProcess functions */

AgendaProcess::AgendaProcess (QObject *parent)
  :QProcess (parent)
{
  connect (this, SIGNAL (finished(int, QProcess::ExitStatus)), 
          this, SLOT (ProcFinished(int, QProcess::ExitStatus)));
}

void
AgendaProcess::ProcFinished (int exitCode, QProcess::ExitStatus exitStatus)
{
  Q_UNUSED (exitCode)
  Q_UNUSED (exitStatus)
  emit Finished (this);
}

/** ShellLauncher functions */

ShellLauncher::ShellLauncher (QObject *parent)
  :QObject (parent)
{
}

void
ShellLauncher::Launch (const AgendaShell & shell)
{
  AgendaProcess * newProc = new AgendaProcess (this);
  processes.insert (newProc);
  connect (newProc, SIGNAL (Finished (AgendaProcess*)),
           this, SLOT (Finished (AgendaProcess*)));
  newProc->startDetached (shell.Command());
}

void
ShellLauncher::Finished (AgendaProcess *proc)
{
  processes.remove (proc);
  proc->deleteLater ();
}

} // agenda

