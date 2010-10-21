
#include "event-table.h"

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

#include "deliberate.h"
#include "db-manager.h"
#include "agenda-event.h"
#include <QTableWidgetItem>
#include <QDateTime>
#include <QDebug>
#include <QAction>
#include <QDesktopServices>
#include <QClipboard>
#include <QMenu>
#include <QUrl>

using namespace deliberate;

namespace agenda
{

EventTable::EventTable (QWidget *parent)
  :QTableWidget (parent),
   db(0),
   dateForm ("ddd yyyy-MM-dd hh:mm:ss")
{
}

void
EventTable::Init (DBManager *dbm)
{
  db = dbm;
  dateForm = Settings().value ("display/dateform",dateForm).toString();
  Settings().setValue ("display/dateform",dateForm);
  int numCols = columnCount();
  for (int c=0; c<numCols; c++) {
    int cw = columnWidth (c);
    QString colname (QString ("sizes/eventwidth%1").arg(c));
    cw = Settings().value (colname,cw).toInt();
    Settings().setValue (colname, cw);
    setColumnWidth (c, cw);
  }
  connect (this, SIGNAL (itemClicked (QTableWidgetItem *)), 
           this, SLOT (CellPicked (QTableWidgetItem *)));
}

void
EventTable::SaveSettings ()
{
  int nc = columnCount ();
  for (int c=0; c<nc; c++) {
    Settings().setValue (QString("sizes/eventwidth%1").arg(c),
                  columnWidth (c));
  }
}

void
EventTable::Load ()
{
  clearContents ();
  setRowCount (0);
  int row (0);
  if (db) {
    int it = db->OpenReadEvents();
    if (it >= 0) {
      AgendaEvent event;
      while (db->ReadNext (it, event)) {
        setRowCount (row+1);
        DisplayEvent (row, event);
        row++;
      }
      db->CloseRead (it);
    }
  }
}

void
EventTable::DisplayEvent (int row, AgendaEvent & event)
{
  QTableWidgetItem *item (0);
  item = new QTableWidgetItem (event.Nick());
  item->setData (Role_Celltype, Cell_Nick);
  item->setData (Role_Uuid, event.Id().toString());
  setItem (row,0,item);
  QString date = QDateTime::fromTime_t (event.Time())
                            .toString(dateForm);
  item = new QTableWidgetItem (date);
  item->setData (Role_Celltype, Cell_Time);
  setItem (row,1,item);
  item = new QTableWidgetItem (event.Description ());
  item->setData (Role_Celltype, Cell_Desc);
  setItem (row,2,item);
}

void
EventTable::CellPicked (QTableWidgetItem *item)
{
  CellType typ = CellType (item->data (Role_Celltype).toInt());
  CellMenu (item);
}

QAction *
EventTable::CellMenu (const QTableWidgetItem *item,
                      const QList<QAction *>  extraActions)
{
  if (item == 0) {
    return 0;
  }
  QMenu menu (this);
  QAction * copyAction = new QAction (tr("Copy Text"),this);
  copyAction->setIcon(QPixmap(":/images/copy.png"));
  QAction * mailAction = new QAction (tr("Mail Text"),this);
  mailAction->setIcon(QPixmap(":/images/mail.png"));
  menu.addAction (copyAction);
  menu.addAction (mailAction);
  if (extraActions.size() > 0) {
    menu.addSeparator ();
  }
  for (int a=0; a < extraActions.size(); a++) {
    menu.addAction (extraActions.at (a));
  }
  
  QAction * select = menu.exec (QCursor::pos());
  if (select == copyAction) {
    QClipboard *clip = QApplication::clipboard ();
    if (clip) {
      clip->setText (item->text());  
    }
    return 0;
  } else if (select == mailAction) {
    QStringList mailBodytotal;
    QString mailBody = item->text();
    QString urltext = tr("mailto:?body=%1")
                      .arg (mailBody);
    QDesktopServices::openUrl (QUrl (urltext));
    return 0;
  } else {
    return select;
  }
}


} // namespace

