#ifndef AGENDA_H
#define AGENDA_H

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
#include <QMainWindow>
#include "ui_agenda.h"
#include "config-edit.h"
#include "db-manager.h"
#include "agenda-event.h"
#include "agenda-warning.h"
#include "agenda-shell.h"
#include "helpview.h"

class QApplication;
class QDate;


namespace agenda 
{

class ItemEdit;
class AgendaScheduler;
class Notify;
class ShellLauncher;

class Agenda : public QMainWindow
{
Q_OBJECT

public:

  Agenda (QWidget *parent=0);

  void  Init (QApplication &ap);
  bool  Run ();
  bool  Again ();

  void  AddConfigMessages (const QStringList & cm) 
           { configMessages.append (cm); }

private slots:

  void Quit ();
  void Restart ();
  void EditSettings ();
  void SetSettings ();
  void About ();
  void License ();
  void Exiting ();
  void NewItem ();
  void PickedDate (const QDate & date);
  void ToggleCal ();
  void Minimize ();
  void CleanOld ();

  void NewEvent (AgendaEvent event);
  void NewWarning (AgendaWarning warning);
  void NewShell (AgendaShell shell);
  void NewRepeat (AgendaRepeat repeat);
  void LaunchEvent (AgendaEvent event);
  void LaunchShell (AgendaShell shell);

  void Launched (int howmany);

private:

  void Connect ();

  bool             initDone;
  QApplication    *app;
  Ui_AgendaMain    mainUi;
 
  ConfigEdit       configEdit;
  QStringList      configMessages;

  DBManager        db;
  ItemEdit        *itemEdit;
  AgendaScheduler *scheduler;
  Notify          *notify;
  ShellLauncher   *shellLauncher;
  deliberate::HelpView        *helpView;
  QString          dateForm;
  bool             runAgain;


};

} // namespace

#endif
