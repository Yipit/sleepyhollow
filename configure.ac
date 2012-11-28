#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([sleepy-hollow], [0.0.1], [coders@yipit.com])
AC_CONFIG_SRCDIR([lib/webview.cpp])
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
AC_PROG_LIBTOOL

# AutoTroll with Qt.
AT_WITH_QT([+network +webkit +gui])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.

AC_CONFIG_FILES([
        Makefile
        lib/Makefile
        include/Makefile
        include/yipit/Makefile
        include/yipit/hollow/Makefile
])

AC_OUTPUT
