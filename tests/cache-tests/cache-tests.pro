QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_cachetest.cpp \
	../../src/http/cache/cacheditem.cpp \
	../../src/http/cache/cachedpiece.cpp

HEADERS += ../../src/http/cache/cacheditem.h
	../../src/http/cache/cachedpiece.h

