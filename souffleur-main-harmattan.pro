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

include ("souffleur-common.pri")

QMAKE_CXXFLAGS += -DDELIBERATE_HARMATTAN=6

PROJECT = souffleur

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/main.qml

contains(MEEGO_EDITION,harmattan) {

    icon.files = harmattan/souffleur.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    INSTALLS += icon

    audiofiles.files += sounds/alert.wav
    audiofiles.path = /opt/souffleur/audio
    INSTALLS += audiofiles


    desktopfile.files = harmattan/souffleur.desktop
    desktopfile.path = /usr/share/applications
    INSTALLS += desktopfile

    target.path = /opt/souffleur/bin
    INSTALLS += target

} else {

    # YES this is the same stuff as for the contains() case above
    # this is here because OBS doesn't have the "harmattan" symbol in MEEGO_EDITION
    # and because this .pro is ONLY for harmattan

    icon.files = harmattan/souffleur.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    INSTALLS += icon

    audiofiles.files += sounds/alert.wav
    audiofiles.path = /opt/souffleur/audio
    INSTALLS += audiofiles


    desktopfile.files = harmattan/souffleur.desktop
    desktopfile.path = /usr/share/applications
    INSTALLS += desktopfile

    target.path = /opt/souffleur/bin
    INSTALLS += target

}
