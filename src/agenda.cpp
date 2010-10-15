#include "agenda.h"

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

#include <QApplication>
#include "deliberate.h"
#include "version.h"
#include <QSize>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

using namespace deliberate;

namespace agenda
{

Agenda::Agenda (QWidget *parent)
  :QMainWindow (parent),
   app (0),
   configEdit (this)
{
  mainUi.setupUi (this);
  Connect ();
}

void
Agenda::Init (QApplication &ap)
{
  app = &ap;
}

void
Agenda::Run ()
{
  qDebug () << " Start Agenda";
  if (Settings().contains("sizes/main")) {
    QSize defaultSize = size();
    QSize newsize = Settings().value ("sizes/main", defaultSize).toSize();
    resize (newsize);
  }
  show ();
}

void
Agenda::Connect ()
{
  connect (mainUi.actionQuit, SIGNAL (triggered()), this, SLOT (Quit()));
  connect (mainUi.actionSettings, SIGNAL (triggered()),
           this, SLOT (EditSettings()));
  connect (mainUi.actionAbout, SIGNAL (triggered()),
           this, SLOT (About ()));
}


void
Agenda::Quit ()
{
  QSize currentSize = size();
  Settings().setValue ("sizes/main",currentSize);
  Settings().sync();
  if (app) {
    app->quit();
  }
}

void
Agenda::EditSettings ()
{
  configEdit.Exec ();
  SetSettings ();
}

void
Agenda::SetSettings ()
{
}

void
Agenda::About ()
{
  QString version (deliberate::ProgramVersion::Version());
  QStringList messages;
  messages.append (version);
  messages.append (configMessages);

  QMessageBox  box;
  box.setText (version);
  box.setDetailedText (messages.join ("\n"));
  QTimer::singleShot (30000, &box, SLOT (accept()));
  box.exec ();
}




} // namespace

