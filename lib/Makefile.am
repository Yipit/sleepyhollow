include $(top_srcdir)/build-aux/autotroll.mk

lib_LTLIBRARIES = libhollow.la
libhollow_la_SOURCES = webpage.cpp webpage.moc.cpp manager.cpp manager.moc.cpp hollow.cpp
libhollow_la_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) -I$(top_srcdir)/include
libhollow_la_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
libhollow_la_LDFLAGS = $(QT_LDFLAGS) $(LDFLAGS) -export-symbols-regex '^y_hollow_.*'
libhollow_la_LIBADD = $(QT_LIBS) $(LDADD)

BUILT_SOURCES = webpage.moc.cpp manager.moc.cpp

noinst_PROGRAMS = test-hollow
test_hollow_SOURCES = test-hollow.c
test_hollow_CFLAGS = -I$(top_srcdir)/include
test_hollow_LDADD =  $(QT_LIBS) ./libhollow.la
