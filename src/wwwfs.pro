QT -= gui
QT += network

CONFIG += c++14 console link_pkgconfig
CONFIG -= app_bundle

PKGCONFIG += fuse3

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += controller.cpp \
	actions/getattraction.cpp \
	actions/readdiraction.cpp \
	actions/readaction.cpp \
	actions/syncaction.cpp \
	actions/lookupaction.cpp \
	actions/openaction.cpp \
	configurationmanager.cpp \
	fs/folder.cpp \
	fs/file.cpp \
	fs/humanreadablerenderer.cpp \
	fs/contentcontainer.cpp \
	fs/contentitem.cpp \
	fs/rendererbase.cpp \
	http/httpparser.cpp \
	http/contentcache.cpp \
	http/httpclient.cpp \
	http/httpitem.cpp \
	http/icontentcallback.cpp \
	main.cpp \
    http/htmlloader.cpp \
    http/remoteresourceinfo.cpp \
    http/cache/cacheditem.cpp \
    http/cache/cachedpiece.cpp

HEADERS += \
	actions/readdiraction.h \
	actions/openaction.h \
	actions/lookupaction.h \
	actions/getattraction.h \
	actions/readaction.h \
	configurationmanager.h \
	fs/humanreadablerenderer.h \
	fs/folder.h \
	fs/rendererbase.h \
	fs/contentcontainer.h \
	fs/file.h \
	fs/contentitem.h \
	http/httpclient.h \
	http/httpparser.h \
	http/httpitem.h \
	http/contentcache.h \
	controller.h \
    http/htmlloader.h \
    http/remoteresourceinfo.h \
    callback.h \
    callbackwithargument.h \
    icallback.h \
    callbackwithargumentanddata.h \
    http/cache/cacheditem.h \
    http/cache/cachedpiece.h \
    actions/action.h

DEFINES += "FUSE_USE_VERSION=31"

CONFIG(debug, debug|release) {
	# DEBUG ONLY
	QMAKE_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
	QMAKE_CFLAGS   += -fsanitize=address -fno-omit-frame-pointer
	QMAKE_LFLAGS   += -fsanitize=address
}

# HTML parser:

unix: LIBS += -L$$OUT_PWD/../libs/QGumboParser/QGumboParser/ -lQGumboParser

INCLUDEPATH += $$PWD/../libs/QGumboParser/QGumboParser
DEPENDPATH += $$PWD/../libs/QGumboParser/QGumboParser

unix: PRE_TARGETDEPS += $$OUT_PWD/../libs/QGumboParser/QGumboParser/libQGumboParser.a
