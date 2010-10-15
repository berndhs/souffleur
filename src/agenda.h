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
#include <QDateTimeEdit>

class QApplication;
class QDate;


namespace agenda 
{

class Agenda : public QMainWindow
{
Q_OBJECT

public:

  Agenda (QWidget *parent=0);

  void  Init (QApplication &ap);
  void  Run ();

  void  AddConfigMessages (const QStringList & cm) 
           { configMessages.append (cm); }

private slots:

  void Quit ();
  void EditSettings ();
  void SetSettings ();
  void About ();
  void Exiting ();
  void NewItem ();
  void PickedDate (const QDate & date);

private:

  void Connect ();

  QApplication    *app;
  Ui_AgendaMain    mainUi;
 
  ConfigEdit       configEdit;
  QStringList      configMessages;
  QDateTimeEdit    *dateEdit;


};

} // namespace

#endif
