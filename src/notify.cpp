
#include "notify.h"

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

#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "deliberate.h"
#define DELIBERATE_QT_SIGNALS signals
#undef signals

using namespace deliberate;

namespace agenda
{


QString Notify::appName (QString::fromUtf8("Souffleur"));

/** AgendaBox is the window that pops up from the notifier */

AgendaBox::AgendaBox (QWidget *parent, bool oldVisible, bool oldMinimized)
  :QMessageBox (parent),
   wasVisible (oldVisible),
   wasMinimized (oldMinimized)
{
}

bool
AgendaBox::WasVisible ()
{
  return wasVisible;
}

bool
AgendaBox::WasMinimized ()
{
  return wasMinimized;
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
   showTime (10),
   trayIcon (0)
{
  showTime = Settings().value ("timers/showtime",showTime).toInt();
  Settings().setValue ("timers/showtime",showTime);
}

void
Notify::SetTrayIcon (QSystemTrayIcon * tray)
{
  trayIcon = tray;
}

void
Notify::SetAppName (const QString & name)
{
  appName = name;
}

void
Notify::ShowMessage (const AgendaEvent & event, bool oldVisible, bool oldMinimized)
{
  Q_UNUSED (event)
  Q_UNUSED (oldVisible)
  Q_UNUSED (oldMinimized)

  QStringList mlist;
  mlist << event.Nick ();
  mlist << QDateTime::fromTime_t(event.Time()).toString("hh:mm:ss");
  mlist << event.Description ();

  #if 0
  AgendaBox * box = new AgendaBox (parentWidget, oldVisible, oldMinimized);
  activeBoxes.insert (box);
  connect (box, SIGNAL (Done(AgendaBox*)), this, SLOT (BoxDone (AgendaBox*)));
  box->setText (mlist.join ("\n"));
  qDebug () << "Launching Event " << mlist;
  QTimer::singleShot (showTime * 1000, box, SLOT (accept()));
  box->setWindowModality (Qt::NonModal);
  box->setFocus (Qt::PopupFocusReason);
  box->show ();
  box->activateWindow ();
  #endif

  if (trayIcon) {
    trayIcon->showMessage (appName,
               mlist.join ("\n"),
               QSystemTrayIcon::Information,
               showTime * 1000); 
  }
}

void
Notify::BoxDone (AgendaBox *box)
{
  if (box) {
    bool oldVisible = box->WasVisible();
    bool oldMinimized = box->WasMinimized ();
    activeBoxes.remove (box);
    disconnect (box, 0,0,0);
    box->deleteLater ();
    emit MessageDone (oldVisible, oldMinimized);
  }
}

} // namespace

