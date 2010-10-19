
#include "notify.h"

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

#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "deliberate.h"

using namespace deliberate;

namespace agenda
{

/** AgendaBox is the window that pops up from the notifier */

AgendaBox::AgendaBox (QWidget *parent)
  :QMessageBox (parent)
{
}

void
AgendaBox::accept ()
{
  QMessageBox::accept ();
  emit Done (this);
}

void
AgendaBox::reject ()
{
  QMessageBox::reject ();
  emit Done (this);
}

/** Notify is the notifier */

Notify::Notify (QWidget *parent)
  :QObject (parent),
   parentWidget (parent),
   showTime (10)
{
  showTime = Settings().value ("timers/showtime",showTime).toInt();
  Settings().setValue ("timers/showtime",showTime);
}

void
Notify::ShowMessage (const AgendaEvent & event)
{
  AgendaBox * box = new AgendaBox (parentWidget);
  activeBoxes.insert (box);
  connect (box, SIGNAL (Done(AgendaBox*)), this, SLOT (BoxDone (AgendaBox*)));
  QStringList mlist;
  mlist << event.Id().toString();
  mlist << event.Nick ();
  mlist << QDateTime::fromTime_t(event.Time()).toString("hh:mm:ss");
  mlist << event.Description ();
  box->setText (mlist.join ("\n"));
  qDebug () << "Launching Event " << mlist;
  QTimer::singleShot (showTime * 1000, box, SLOT (accept()));
  box->show ();
}

void
Notify::BoxDone (AgendaBox *box)
{
  if (box) {
    activeBoxes.remove (box);
    disconnect (box, 0,0,0);
    box->deleteLater ();
  }
}

} // namespace

