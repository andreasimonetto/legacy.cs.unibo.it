dnl Process this file with autoconf to produce a configure script.

AC_INIT(persp_cube2, 0.3)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_PATH_XTRA
AC_PROG_LEX



dnl ***************************************************************************
dnl Internatinalization
dnl ***************************************************************************
GETTEXT_PACKAGE=persp_cube2
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.35.0])

AM_PROG_LIBTOOL

AC_SUBST(PERSP_CUBE2_CFLAGS)
AC_SUBST(PERSP_CUBE2_LIBS)

AC_OUTPUT([
Makefile
src/Makefile
po/Makefile.in
])
