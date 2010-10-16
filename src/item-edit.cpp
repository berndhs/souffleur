
#include "item-edit.h"

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
#include <QDate>
#include <QDateTime>

namespace agenda
{

ItemEdit::ItemEdit (QWidget *parent)
  :QDialog (parent)
{
  ui.setupUi (this);

  connect (ui.saveButton, SIGNAL (clicked()), this, SLOT (Save()));
  connect (ui.cancelButton, SIGNAL (clicked()), this, SLOT (reject()));
  ui.saveButton->setDefault (false);
  ui.saveButton->setAutoDefault (false);
  ui.cancelButton->setDefault (false);
  ui.cancelButton->setAutoDefault (false);
}

void
ItemEdit::NewItem ()
{
  ui.titleEdit->clear ();
  ui.whenEdit->setDateTime (QDateTime::currentDateTime().addSecs (1*60*60));
  ui.whatEdit->clear ();
  show ();
}

void
ItemEdit::NewItem (const QDate & date)
{
  ui.titleEdit->clear ();
  ui.whenEdit->setDate (date);
  ui.whatEdit->clear ();
  show ();
}

void
ItemEdit::Save ()
{
  AgendaEvent event;
  event.SetNick (ui.titleEdit->text ());
  event.SetDescription (ui.whatEdit->toPlainText ());
  event.SetTime (ui.whenEdit->dateTime().toTime_t());
  emit NewEvent (event);
  accept ();
}

} // namespace

