TEMPLATE = app

TARGET = bin/background-picker

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
     src/linux-wallpaper.cpp \
     src/deliberate.cpp \
     src/version.cpp 

target.path = /usr/local/bin
INSTALLS += target
