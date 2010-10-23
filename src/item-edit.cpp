
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
#include <QAbstractButton>
#include <QSpinBox>
#include "deliberate.h"
#include <QDebug>

using namespace deliberate;

namespace agenda
{

ItemEdit::ItemEdit (QWidget *parent)
  :QDialog (parent)
{
  ui.setupUi (this);

  repeatValues [ui.minCheck] = ui.repeatMins;
  repeatValues [ui.hourCheck] = ui.repeatHours;
  repeatValues [ui.dayCheck] = ui.repeatDays;
  repeatValues [ui.monthCheck] = ui.repeatMonths;
  repeatKind [ui.minCheck] = "min";
  repeatKind [ui.hourCheck] = "hour";
  repeatKind [ui.dayCheck] = "day";
  repeatKind [ui.monthCheck] = "month";
  repeatChecked = ui.norepeatCheck;
  connect (ui.saveButton, SIGNAL (clicked()), this, SLOT (Save()));
  connect (ui.cancelButton, SIGNAL (clicked()), this, SLOT (reject()));
  connect (ui.warnOrRepeatGroup, SIGNAL (buttonClicked (QAbstractButton *)),
           this, SLOT (ArrangeFrames (QAbstractButton *)));
  connect (ui.repeatGroup, SIGNAL (buttonClicked (QAbstractButton *)),
           this, SLOT (RepeatChanged (QAbstractButton*)));
  ui.saveButton->setDefault (false);
  ui.saveButton->setAutoDefault (false);
  ui.cancelButton->setDefault (false);
  ui.cancelButton->setAutoDefault (false);
  dateForm = Settings().value ("display/dateform",dateForm).toString();
  Settings().setValue ("display/dateform",dateForm);
  ui.whenEdit->setDisplayFormat (dateForm);
}

void
ItemEdit::NewItem ()
{
  Clear ();
  ui.whenEdit->setDateTime (QDateTime::currentDateTime().addSecs (1*60*60));
  show ();
}

void
ItemEdit::NewItem (const QDate & date)
{
  Clear ();
  ui.whenEdit->setDate (date);
  show ();
}

void
ItemEdit::Clear ()
{
  ui.titleEdit->clear ();
  ui.whatEdit->clear ();
  ui.shortCheck->setChecked (false);
  ui.longCheck->setChecked (false);
  ui.onceCheck->setChecked (true);
  ui.norepeatCheck->setChecked (true);
  repeatChecked = ui.norepeatCheck;
  ui.warningFrame->hide ();
  ui.repeatFrame->hide ();
  ui.shortMinutes->setValue (0);
  ui.longHours->setValue (0);
  ui.repeatMins->setValue (0);
  ui.repeatHours->setValue (0);
  ui.repeatDays->setValue (0);
  ui.repeatMonths->setValue (0);
  ValueBoxMap::iterator  it;
  for (it=repeatValues.begin(); it != repeatValues.end(); it++) {
    if (it->second) {
      it->second->setValue (0);
    }
  }
}

void
ItemEdit::Save ()
{
  AgendaEvent event;
  event.SetNick (ui.titleEdit->text ());
  event.SetDescription (ui.whatEdit->toPlainText ());
  event.SetTime (ui.whenEdit->dateTime().toTime_t());
  emit NewEvent (event);
  if (ui.shortCheck->isChecked()) {
    AgendaWarning warn (event.Id(), 
                       event.Time() 
                       - qRound64(60.0 * ui.shortMinutes->value()));
    emit NewWarning (warn);
  }
  if (ui.longCheck->isChecked()) {
    AgendaWarning warn (event.Id(), 
                       event.Time() 
                         - qRound64 (60.0 * 60.0 * ui.longHours->value()));
    emit NewWarning (warn);
  }
  if (ui.shellCheck->isChecked()) {
    QString command = ui.shellEdit->text().trimmed ();
    if (command.length() > 0) {
      AgendaShell shell (event.Id(), ui.shellEdit->text());
      emit NewShell (shell);
    }
  }
  QString repeatKind = RepeatKind ();
  if (repeatKind.length() > 0) {
    int repeatDelay = RepeatValue ();
    if (repeatDelay > 0) {
      AgendaRepeat rep (event.Id(), repeatKind, repeatDelay);
      emit NewRepeat (rep);
    }
  }
  accept ();
}


void
ItemEdit::ArrangeFrames (QAbstractButton *button)
{
  Q_UNUSED (button)
  if (ui.repeatCheck->isChecked ()) {
    ui.repeatFrame->show ();
    ui.warningFrame->hide ();
  } else if (ui.warnCheck->isChecked ()) {
    ui.repeatFrame->hide ();
    ui.warningFrame->show ();
  } else {
    ui.repeatFrame->hide ();
    ui.warningFrame->hide ();
  }
}

void
ItemEdit::RepeatChanged (QAbstractButton * button)
{
  qDebug () << " Repeat Check changed " << button;
  repeatChecked = button;
  ClearExcept (repeatValues, button);
}

void
ItemEdit::ClearExcept (ValueBoxMap & values, QAbstractButton * button)
{
  ValueBoxMap::iterator  it;
  for (it=values.begin(); it != values.end(); it++) {
    if (it->second) {
      if (it->first != button) {
        it->second->setValue (0);
      }
    }
  }
}

QString
ItemEdit::RepeatKind ()
{
  if (repeatChecked) {
    if (repeatKind.find (repeatChecked) != repeatKind.end()) {
      return repeatKind [repeatChecked];
    }
  }
  return QString ();
}

int
ItemEdit::RepeatValue ()
{
  if (repeatChecked) {
    if (repeatValues.find (repeatChecked) != repeatValues.end()) {
      return repeatValues [repeatChecked]->value ();
    }
  }
  return -1;
}

} // namespace

