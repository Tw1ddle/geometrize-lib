TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../../geometrize.pri)

SOURCES += ../main.cpp

HEADERS += catch.hpp \
    $$PWD/../tests/*.h \
    $$PWD/../tests/bitmap/*.h \
    $$PWD/../tests/exporters/*.h \
    $$PWD/../tests/runner/*.h \
    $$PWD/../tests/shape/*.h
