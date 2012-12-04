sleepyhollowdir = $(pkgpythondir)
sleepyhollow_PYTHON = __init__.py wrappers.py

pyexecdir = $(pkgpythondir)
pyexec_LTLIBRARIES = _sleepyhollow.la

_sleepyhollow_la_SOURCES = sleepyhollow.cpp
_sleepyhollow_la_CXXFLAGS = $(QT_CXXFLAGS)
_sleepyhollow_la_CPPFLAGS = $(PYTHON_CFLAGS) -I$(top_srcdir)/include $(QT_CPPFLAGS) \
	-fno-strict-aliasing -fno-common -dynamic
_sleepyhollow_la_LDFLAGS = -module -avoid-version -export-symbols-regex init_sleepyhollow $(QT_LDFLAGS)
_sleepyhollow_la_LIBADD = $(top_builddir)/lib/libhollow.la $(QT_LIBS)

EXTRA_DIST = sleepyhollow.h
