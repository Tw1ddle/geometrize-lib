TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../../geometrize/geometrize.pri)

HEADERS += catch.hpp

SOURCES += ../main.cpp \
           $$PWD/../tests/coretest.cpp \
           $$PWD/../tests/modeltest.cpp \
           $$PWD/../tests/scanlinetest.cpp \
           $$PWD/../tests/statetest.cpp \
           $$PWD/../tests/utiltest.cpp \
           $$PWD/../tests/bitmap/bitmaptest.cpp \
           $$PWD/../tests/bitmap/rgbatest.cpp \
           $$PWD/../tests/exporters/shapearrayexportertest.cpp \
           $$PWD/../tests/exporters/shapejsonexportertest.cpp \
           $$PWD/../tests/runner/imagerunneroptionstest.cpp \
           $$PWD/../tests/runner/imagerunnertest.cpp \
           $$PWD/../tests/shape/circletest.cpp \
           $$PWD/../tests/shape/ellipsetest.cpp \
           $$PWD/../tests/shape/rectangletest.cpp \
           $$PWD/../tests/shape/rotatedellipsetest.cpp \
           $$PWD/../tests/shape/rotatedrectangletest.cpp \
           $$PWD/../tests/shape/shapefactorytest.cpp \
           $$PWD/../tests/shape/shapetest.cpp \
           $$PWD/../tests/shape/shapetypestest.cpp \
           $$PWD/../tests/shape/triangletest.cpp
