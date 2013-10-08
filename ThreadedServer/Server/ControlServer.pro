TARGET += Server
TEMPLATE = app

CONFIG += console qt

QT += network widgets

HEADERS = \
    Server.h \
    Protocol.h \
    CommandLineApp.h \
    Client.h \
    version.h \
    ClientThread.h \
    QLogger.h
SOURCES = \
    main.cpp \
    Server.cpp \
    Protocol.cpp \
    CommandLineApp.cpp \
    Client.cpp \
    ClientThread.cpp \
    QLogger.cpp

