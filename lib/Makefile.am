include $(top_srcdir)/build-aux/autotroll.mk

lib_LTLIBRARIES = libhollow.la
libhollow_la_SOURCES = webview.cpp webview.moc.cpp hollow.cpp
libhollow_la_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS) -I$(top_srcdir)/include
libhollow_la_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)
libhollow_la_LDFLAGS = $(QT_LDFLAGS) $(LDFLAGS)
libhollow_la_LIBADD = $(QT_LIBS) $(LDADD)

BUILT_SOURCES = webview.moc.cpp
