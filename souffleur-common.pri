PROJECT = souffleur
TEMPLATE = app

QT += core gui sql webkit declarative
CONFIG += mobility
MOBILITY += sensors multimedia

INCLUDEPATH += ./include

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

#FORMS = ui/DebugLog.ui \
#        ui/config-edit.ui \
#        ui/helpwin.ui \
        

HEADERS = include/schedmain.h \
          include/gpl2.h \
          include/cmdoptions.h \
          include/deliberate.h \
          include/version.h \
          include/agenda.h \
          include/db-manager.h \
          include/agenda-event.h \
          include/agenda-shell.h \
          include/agenda-warning.h \
          include/agenda-repeat.h \
          include/event-list.h \
          include/agenda-scheduler.h \
          include/notify.h \
          include/shell-launcher.h \
          include/property.h \
          include/magic-defs.h \
          include/orientation.h \
          include/date-time-checker.h \


SOURCES = src/schedmain.cpp \
          src/cmdoptions.cpp \
          src/deliberate.cpp \
          src/version.cpp \
          src/agenda.cpp \
          src/db-manager.cpp \
          src/agenda-event.cpp \
          src/agenda-repeat.cpp \
          src/event-list.cpp \
          src/agenda-scheduler.cpp \
          src/notify.cpp \
          src/shell-launcher.cpp \
          src/orientation.cpp \
          src/date-time-checker.cpp \


OTHER_FILES += \
    sounds/alert.wav









