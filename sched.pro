#
# schedule application
#

#/****************************************************************
# * This file is distributed under the following license:
# *
# * Copyright (C) 2010, Bernd Stramm
# *
# *  This program is free software; you can redistribute it and/or
# *  modify it under the terms of the GNU General Public License
# *  as published by the Free Software Foundation; either version 2
# *  of the License, or (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License
# *  along with this program; if not, write to the Free Software
# *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
# *  Boston, MA  02110-1301, USA.
# ****************************************************************/


TEMPLATE = app

QT += core gui sql
DEFINES += DELIBERATE_DEBUG=1
UI_DIR = tmp/ui
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
OBJECTS_DIR = tmp/obj
RESOURCES = sched.qrc
TARGET = bin/agenda

FORMS = ui/agenda.ui \
        ui/DebugLog.ui \
        ui/config-edit.ui \
        ui/item-edit.ui \
        

HEADERS = src/schedmain.h \
          src/gpl2.h \
          src/cmdoptions.h \
          src/config-edit.h \
          src/delib-debug.h \
          src/deliberate.h \
          src/version.h \
          src/agenda.h \
          src/db-manager.h \
          src/item-edit.h \
          src/agenda-event.h \


SOURCES = src/schedmain.cpp \
          src/cmdoptions.cpp \
          src/config-edit.cpp \
          src/delib-debug.cpp \
          src/deliberate.cpp \
          src/version.cpp \
          src/agenda.cpp \
          src/db-manager.cpp \
          src/item-edit.cpp \
          src/agenda-event.cpp \


