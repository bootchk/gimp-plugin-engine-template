dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(gimp-plugin-engine-template, 1.0)

dnl Upgrade to use C++ (previously only C compiler)
dnl to support openCV which deprecated their C-API starting in v3
AC_PROG_CC
AC_PROG_CXX

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()



AC_CONFIG_FILES(Makefile src/Makefile pluginScripts/Makefile)


dnl From gimp-plugin-template/configure.in
GIMP_REQUIRED_VERSION=2.2.0

PKG_CHECK_MODULES(GIMP,
  gimp-2.0 >= $GIMP_REQUIRED_VERSION gimpui-2.0 >= $GIMP_REQUIRED_VERSION)

AC_SUBST(GIMP_CFLAGS)
AC_SUBST(GIMP_LIBS)

GIMP_LIBDIR=`$PKG_CONFIG --variable=gimplibdir gimp-2.0`
AC_SUBST(GIMP_LIBDIR)


dnl From configure.in in the GIMP source code for PyGimp
dnl Note this is not defined in gimp-plugin-template/configure.in because
dnl while C-language and Python-language plugins are installed to the same place,
dnl this definition is only referenced in pluginScripts/Makefile.am

gimpplugindir="$libdir/gimp/2.0"
AC_SUBST(gimpplugindir)


dnl optional if using OpenCV for rendering
PKG_CHECK_MODULES(OPENCV,
		  opencv,
		  HAVE_OPENCV=yes, HAVE_OPENCV=no)

AC_SUBST(OPENCV_CFLAGS)
AC_SUBST(OPENCV_LIBS)

if test "x$HAVE_OPENCV" = "xno"; then
  AC_MSG_ERROR(OpenCV libraries could not be found)
fi
dnl end of optional OpenCV


AC_OUTPUT

