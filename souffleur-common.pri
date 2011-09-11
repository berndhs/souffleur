PROJECT = souffleur
TEMPLATE = app

QT += core gui sql webkit declarative
CONFIG += mobility
MOBILITY += systeminfo sensors multimedia

INCLUDEPATH += ./include

CONFIG += lib_pkgconfig
PKGCONFIG += qca
PKGCONFIG += QJson

QMAKE_CXXFLAGS += -std=c++0x

DEFINES += DELIBERATE_DEBUG=0
!include(options.pri) {
  DEFINES += DELIBERATE_OLDNOTIFY=1
}


UI_DIR = tmp/ui
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
OBJECTS_DIR = tmp/obj
RESOURCES = sched.qrc
TARGET = souffleur

FORMS = ui/DebugLog.ui \
        ui/config-edit.ui \
        ui/item-edit.ui \
        ui/helpwin.ui \
        

HEADERS = include/schedmain.h \
          include/gpl2.h \
          include/cmdoptions.h \
          include/config-edit.h \
          include/delib-debug.h \
          include/deliberate.h \
          include/version.h \
          include/agenda.h \
          include/db-manager.h \
          include/item-edit.h \
          include/agenda-event.h \
          include/agenda-shell.h \
          include/agenda-warning.h \
          include/agenda-repeat.h \
          include/event-list.h \
          include/agenda-scheduler.h \
          include/notify.h \
          include/shell-launcher.h \
          include/helpview.h \
          include/property.h \
          include/magic-defs.h \
          include/orientation.h \
          include/date-time-checker.h \


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
          src/event-list.cpp \
          src/agenda-scheduler.cpp \
          src/notify.cpp \
          src/shell-launcher.cpp \
          src/helpview.cpp \
          src/orientation.cpp \
          src/date-time-checker.cpp \


OTHER_FILES += \
    sounds/alert.wav

