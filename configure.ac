#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([sleepyhollow], [0.1.1], [coders@yipit.com])
AC_CONFIG_SRCDIR([lib/hollow.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign])
AM_PROG_AR

# Being as picky as possible
WARNING_FLAGS="-Wall -Wextra -Werror"
AC_SUBST([WARNING_FLAGS])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LANG([C])
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

# AutoTroll to rule them all! Qt4 does not have the webkitwidgets
# package, so we WILL have a warning when compiling with it
AT_WITH_QT
AT_REQUIRE_QT_VERSION([5.0.0],
        [modules="+network +gui +webkit"],
        [modules="+network +gui +webkit +webkitwidgets"]
)
AT_LOAD_MODULES([$modules])

# Python checks
AM_PATH_PYTHON(2.7.0)
PY_PREFIX=`$PYTHON -c 'import sys ; print sys.prefix'`
PYTHON_LIBS="-lpython$PYTHON_VERSION"
PYTHON_CFLAGS="-I$PY_PREFIX/include/python$PYTHON_VERSION"
AC_SUBST([PYTHON_LIBS])
AC_SUBST([PYTHON_CFLAGS])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.

AC_CONFIG_FILES([
        Makefile
        lib/Makefile
        include/Makefile
        include/hollow/Makefile
        sleepyhollow/Makefile
])

AC_OUTPUT
