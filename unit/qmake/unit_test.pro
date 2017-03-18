TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../../geometrize/geometrize.pri)

HEADERS += catch.hpp

SOURCES += ../main.cpp \
           $$files(../tests/*.cpp, true)
