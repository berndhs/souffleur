TEMPLATE = app

TARGET = bin/souffleur-background-picker

OBJECTS_DIR = picker_tmp/obj
MOC_DIR = picker_tmp/moc

QMAKE_CXXFLAGS += --std=c++0x

LIBS += -lrt

INCLUDEPATH += ./include

HEADERS += \
    include/background-picker.h \
    include/deliberate.h \
    include/version.h

SOURCES = src/background-picker.cpp \
     src/harmattan-wallpaper.cpp \
     src/deliberate.cpp \
     src/version.cpp 

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/souffleur/bin
    INSTALLS += target
}



contains(MEEGO_EDITION,harmattan) {
    icon.files = picker-harmattan.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    INSTALLS += icon
}
