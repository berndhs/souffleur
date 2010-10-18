#ifndef NOTIFY_H
#define NOTIFY_H

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

#include "agenda-event.h"

#include <QObject>
#include <QSet>
#include <QMessageBox>

namespace agenda
{

class AgendaBox : public QMessageBox
{
Q_OBJECT

public:

  AgendaBox (QWidget *parent=0)
    :QMessageBox (parent) 
    {}

public slots:

  void accept ();
  void reject ();

signals:

  void Done (AgendaBox *);

private:

};

class Notify : public QObject
{
Q_OBJECT

public:

  Notify (QWidget *parent=0);

  void ShowMessage (const AgendaEvent & event);

public slots:

  void BoxDone (AgendaBox *box);

private:
 
  QWidget           *parentWidget;
  QSet <AgendaBox*>  activeBoxes;
} ;

} // namespace

#endif
