#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T22:41:17
#
#-------------------------------------------------

QT       -= gui

TARGET = kernel
TEMPLATE = lib
DEFINES += KERNEL_DLL
DESTDIR = ../lib
INCLUDEPATH +=  \
                ../include


SOURCES += \
#kernel.cpp \
    hkernelhandle.cpp \
    hopterm.cpp \
    hkeranalogue.cpp \
    hkerdatabase.cpp \
    hkerdigital.cpp \
    hkergroup.cpp \
    hkerstation.cpp \
    hkerword.cpp \
    hlocktype.cpp \
    hpointterm.cpp \
    huserdb.cpp \
    hworkdatabase.cpp

HEADERS +=  \
    #kernel.h\
    hkernelhandle.h \
    hopterm.h \
    hkeranalogue.h \
    hkerdatabase.h \
    hkerdigital.h \
    hkergroup.h \
    hkerstation.h \
    hkerword.h \
    hlocktype.h \
    hpointterm.h \
    huserdb.h \
    hworkdatabase.h

win32{
    UI_DIR = temp/ui
    MOC_DIR = temp/moc
    OBJECTS_DIR	= temp/obj

    LIBS += ../lib/datafile.lib
}

unix {
    #target.path = /usr/lib
    #INSTALLS += target
}
