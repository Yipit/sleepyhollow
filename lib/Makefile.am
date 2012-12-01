include $(top_srcdir)/build-aux/autotroll.mk

lib_LTLIBRARIES = libhollow.la
libhollow_la_SOURCES = webpage.cpp hollow.cpp
libhollow_la_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) -I$(top_srcdir)/include
libhollow_la_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
libhollow_la_LDFLAGS = $(QT_LDFLAGS) $(LDFLAGS) #-export-symbols-regex '^y_hollow_.*'
libhollow_la_LIBADD = $(QT_LIBS)

BUILT_SOURCES = \
	../include/yipit/hollow/webpage.moc.cpp \
	../include/yipit/hollow/hollow.moc.cpp

libhollow_la_SOURCES += \
	../include/yipit/hollow/webpage.moc.cpp \
	../include/yipit/hollow/hollow.moc.cpp

noinst_PROGRAMS = test-hollow
test_hollow_SOURCES = test-hollow.cpp
test_hollow_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) -I$(top_srcdir)/include
test_hollow_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
test_hollow_LDADD =  $(QT_LIBS) $(LDADD) ./libhollow.la
