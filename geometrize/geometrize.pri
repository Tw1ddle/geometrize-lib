INCLUDEPATH += $$PWD

include($$PWD/../lib/libsimdpp.pri)

HEADERS += $$files($$PWD/geometrize/*.h, true)
SOURCES += $$files($$PWD/geometrize/*.cpp, true)
