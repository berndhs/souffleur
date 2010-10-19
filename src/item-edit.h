#ifndef ITEM_EDIT_H
#define ITEM_EDIT_H

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
#include "ui_item-edit.h"
#include "agenda-event.h"
#include "agenda-warning.h"
#include "agenda-shell.h"
#include "agenda-repeat.h"

#include <map>

class QAbstractButton;
class QSpinBox;
class QDate;

namespace agenda 
{
class ItemEdit : public QDialog
{
Q_OBJECT

public:

  ItemEdit (QWidget * parent=0);

  void  NewItem ();
  void  NewItem (const QDate & date);

public slots:

  void Save ();

private slots:

  void ChangeWarning (int newState);
  void ChangeRepeat  (int newState);
  void RepeatChanged (QAbstractButton * button);

signals:

  void NewEvent (AgendaEvent event);
  void NewWarning (AgendaWarning warning);
  void NewShell (AgendaShell shell);
  void NewRepeat (AgendaRepeat rep);

private:

  typedef std::map <QAbstractButton*, QSpinBox*> ValueBoxMap;

  void Clear ();
  void ClearExcept (ValueBoxMap & values, QAbstractButton * button);
  QString RepeatKind ();
  int     RepeatValue ();

  Ui_ItemEdit   ui;
  QString       dateForm;

  ValueBoxMap        repeatValues;
  QAbstractButton   *repeatChecked;
  std::map <QAbstractButton *, QString>  repeatKind;

};

} // namespace

#endif

