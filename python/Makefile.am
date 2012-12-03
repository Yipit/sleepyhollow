pyexec_LTLIBRARIES = sleepyhollow.la

sleepyhollow_la_SOURCES = sleepyhollow.cpp
sleepyhollow_la_CXXFLAGS = $(QT_CXXFLAGS)
sleepyhollow_la_CPPFLAGS = $(PYTHON_CFLAGS) -I$(top_srcdir)/include $(QT_CPPFLAGS) \
	-fno-strict-aliasing -fno-common -dynamic
sleepyhollow_la_LDFLAGS = -module -avoid-version -export-symbols-regex initsleepyhollow $(QT_LDFLAGS)
sleepyhollow_la_LIBADD = $(top_builddir)/lib/libhollow.la $(QT_LIBS)

EXTRA_DIST = sleepyhollow.h
