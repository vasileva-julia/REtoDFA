INCLUDEPATH += /usr/src/gtest/include/gtest
LIBS += -lgtest

INCLUDEPATH += ../app
QT += widgets

CONFIG += c++1z

HEADERS += \
    ../app/alphabet.h \
    ../app/dfa.h \
    ../app/syntaxtree.h

SOURCES += \
    main.cpp \
    ../app/alphabet.cpp \
    ../app/dfa.cpp \
    ../app/syntaxtree.cpp
