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


SOURCES += kernel.cpp \
    kerdatabase.cpp \
    kerword.cpp \
    kergroup.cpp \
    kerdigital.cpp \
    keranalogue.cpp \
    userdb.cpp \
    workdatabase.cpp \
    glossary.cpp \
    pointterm.cpp \
    kerstation.cpp \
    locktype.cpp \
    hkernelhandle.cpp

HEADERS += kernel.h\
    kerdatabase.h \
    kerword.h \
    kergroup.h \
    kerdigital.h \
    keranalogue.h \
    userdb.h \
    workdatabase.h \
    glossary.h \
    pointterm.h \
    kerstation.h \
    locktype.h \
    kernelapi.h \
    hkernelhandle.h

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
