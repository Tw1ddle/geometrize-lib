INCLUDEPATH += $$PWD

HEADERS += $$files($$PWD/geometrize/*.h, true)
SOURCES += $$files($$PWD/geometrize/*.cpp, true)

*-g++ {
    QMAKE_CXXFLAGS += -pthread
    LIBS += -pthread
}
