QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
LIBS += -lsqlite3

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bingo_struct.cpp \
    db.cpp \
    game.cpp \
    game_struct.cpp \
    main.cpp \
    bingo.cpp \
    manager.cpp \
    stat.cpp

HEADERS += \
    bingo.h \
    bingo_struct.h \
    db.h \
    game.h \
    game_struct.h \
    manager.h \
    stat.h

FORMS += \
    bingo.ui \
	game.ui \
	manager.ui \
	stat.ui

## Copy data files
#copydata.commands = $(COPY_DIR) \
#					$$PWD/db.sqlite \
#					$$PWD/saves/ \
#					$$OUT_PWD
#first.depends = $(first) copydata
#export(first.depends)
#export(copydata.commands)
#QMAKE_EXTRA_TARGETS += first copydata

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
