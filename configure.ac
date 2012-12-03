#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([sleepy-hollow], [0.0.1], [coders@yipit.com])
AC_CONFIG_SRCDIR([lib/hollow.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall])
AM_PROG_AR

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LANG([C])
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

# AutoTroll with Qt.
AT_WITH_QT([+network +webkit +gui])

dnl python checks
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
        include/yipit/Makefile
        include/yipit/hollow/Makefile
        python/Makefile
])

AC_OUTPUT
