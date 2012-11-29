pyexec_LTLIBRARIES = sleepy_hollow.la

sleepy_hollow_la_SOURCES = sleepy_hollow.cpp
sleepy_hollow_la_CXXFLAGS = $(QT_CXXFLAGS)
sleepy_hollow_la_CPPFLAGS = $(PYTHON_CFLAGS) -I$(top_srcdir)/include $(QT_CPPFLAGS) \
	-fno-strict-aliasing -fno-common -dynamic -Wstrict-prototypes
sleepy_hollow_la_LDFLAGS = -module -avoid-version -export-symbols-regex initsleepy_hollow $(QT_LDFLAGS)
sleepy_hollow_la_LIBADD = $(top_builddir)/lib/libhollow.la $(QT_LIBS)
