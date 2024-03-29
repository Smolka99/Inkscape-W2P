dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.64)
AC_INIT([libcroco],[0.6.99],[https://gitlab.com/inkscape/libcroco/-/issues],[libcroco])
AC_CONFIG_SRCDIR([src/cr-input.c])
AC_CANONICAL_HOST

# gnu strictness to generate the INSTALL file
AM_INIT_AUTOMAKE([1.13 tar-ustar dist-xz no-dist-gzip -Wno-portability foreign])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

if test -z "$enable_maintainer_mode"; then
  enable_maintainer_mode=yes
fi
AM_MAINTAINER_MODE([enable])

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

LIBCROCO_MAJOR_VERSION=0
LIBCROCO_MINOR_VERSION=6
LIBCROCO_MICRO_VERSION=99

LIBCROCO_CURRENT=3
LIBCROCO_REVISION=1
LIBCROCO_AGE=0

#LIBCROCO_VERSION_INFO=`expr $LIBCROCO_MAJOR_VERSION + $LIBCROCO_MINOR_VERSION`:$LIBCROCO_MICRO_VERSION:$LIBCROCO_MINOR_VERSION
LIBCROCO_VERSION_INFO="$LIBCROCO_CURRENT:$LIBCROCO_REVISION:$LIBCROCO_AGE"
LIBCROCO_VERSION=$LIBCROCO_MAJOR_VERSION.$LIBCROCO_MINOR_VERSION.$LIBCROCO_MICRO_VERSION
LIBCROCO_VERSION_NUMBER=`expr $LIBCROCO_MAJOR_VERSION \* 10000 + $LIBCROCO_MINOR_VERSION \* 100 + $LIBCROCO_MICRO_VERSION`

AC_SUBST(LIBCROCO_MAJOR_VERSION)
AC_SUBST(LIBCROCO_MINOR_VERSION)
AC_SUBST(LIBCROCO_MICRO_VERSION)
AC_SUBST(LIBCROCO_VERSION)
AC_SUBST(LIBCROCO_VERSION_INFO)
AC_SUBST(LIBCROCO_VERSION_NUMBER)
AC_SUBST(LIBCROCO_CURRENT)
AC_SUBST(LIBCROCO_AGE)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_CPP
AC_PROG_MAKE_SET
AC_STDC_HEADERS
AC_ISC_POSIX

LT_INIT([])

CROCO_LIB=libcroco-$LIBCROCO_MAJOR_VERSION.$LIBCROCO_MINOR_VERSION.la
AC_SUBST(CROCO_LIB)

case "$host" in
  *-*-cygwin*|*-*-mingw*)
    platform_win32=yes
    ;;
  *)
    platform_win32=no
    ;;
esac
AM_CONDITIONAL(PLATFORM_WIN32, [test "$platform_win32" = "yes"])

GTK_DOC_CHECK([1.0])

dnl **************************************************************
dnl check for the different --enable-option=val  
dnl messages issued by the user
dnl ***************************************************************
G_DISABLE_CHECKS=0
AC_ARG_ENABLE(checks,
		AC_HELP_STRING([--enable-checks=yes|no],
	 	[enables runtime safety checks. Default=yes]),
		WITH_CHECKS=$enableval,
		WITH_CHECKS="yes")

if test "$WITH_CHECKS" = "no" ; then
	G_DISABLE_CHECKS=1
fi
AC_SUBST(G_DISABLE_CHECKS)

dnl ************************************************
dnl end of check of the different --enable-feature options
dnl *************************************************

##############
# Dependencies
##############

GLIB_REQUIRED=2.0
LIBXML_REQUIRED=2.4.23

PKG_CHECK_MODULES([CROCO],[
  glib-2.0 >= $GLIB_REQUIRED
  libxml-2.0 >= $LIBXML_REQUIRED])

##########################################################
# Check for -Bsymbolic-functions linker flag used to avoid
# intra-library PLT jumps, if available.
##########################################################

AC_ARG_ENABLE([Bsymbolic],
  [AS_HELP_STRING([--disable-Bsymbolic],
                  [disable linking with -Bsymbolic])],
  [],[enable_Bsymbolic=yes])

BSYMBOLIC_LDFLAG=
if test "$enable_Bsymbolic" != "no"; then
  CC_CHECK_LDFLAGS(["-Wl,-Bsymbolic-functions"],
    [BSYMBOLIC_LDFLAG="-Wl,-Bsymbolic-functions"],
    [if test "$enable_Bsymbolic" = "auto"; then
       AC_MSG_WARN([-Bsymbolic not supported by ld; disabling])
       enable_Bsymbolic=no
     else
       AC_MSG_ERROR([-Bsymbolic requested but not supported by ld. Use --disable-Bsymbolic to disable])
     fi])
fi

AC_SUBST([BSYMBOLIC_LDFLAG])

##############################################
# Check whether MSVC toolset is explicitly set
##############################################

AM_CONDITIONAL(MSVC_BASE_NO_TOOLSET_SET, [test x$MSVC_BASE_TOOLSET = x])
AM_CONDITIONAL(MSVC_NO_TOOLSET_SET, [test x$MSVC_TOOLSET = x])

###################
# Compiler warnings
###################

CC_CHECK_CFLAGS_APPEND([ \
  -Wall -Wextra \
  -Wunused -Wreturn-type -Wswitch -Wcomment -Wtrigraphs \
  -Wchar-subscripts -Wparentheses -Winline -Wredundant-decls \
  -Wformat-nonliteral -Werror=format-security \
  -Wsign-compare -Werror=implicit-function-declaration \
  -Wpointer-arith -Wwrite-strings -Wstrict-prototypes \
  -Wcast-align -Wimplicit -Wuninitialized \
  -Wmissing-prototypes -Wmissing-declarations -Wnested-externs \
  -Wpacked -Wmissing-format-attribute -Wshadow \
  -Wstrict-aliasing=2 -Winit-self -Wunsafe-loop-optimizations \
  -Wdeclaration-after-statement -Wold-style-definition \
  -Wno-missing-field-initializers -Wno-unused-parameter \
  -fno-common -Wno-switch-enum])

##########################################

AC_OUTPUT([
Makefile
libcroco.pc
libcroco.spec
libcroco-zip
croco-config
config.h.win32
win32/Makefile
win32/vs9/Makefile
win32/vs9/croco-version-paths.vsprops
win32/vs10/Makefile
win32/vs10/croco-version-paths.props
win32/vs11/Makefile
win32/vs12/Makefile
win32/vs14/Makefile
win32/vs15/Makefile
win32/vs16/Makefile
docs/Makefile
docs/examples/Makefile
docs/reference/Makefile
tests/Makefile
tests/test-inputs/Makefile
tests/test-output-refs/Makefile
global-test-vars.sh
src/Makefile
src/libcroco-config.h
csslint/Makefile

])

dnl =======================================================================
echo "
	=====================================================================
	      LIBCROCO, GNOME CSS2 PARSING AND MANIPULATION TOOLKIT $VERSION
	=====================================================================

	prefix:				      :	${prefix}
	source code location:		      :	${srcdir}
	compiler: 			      :	${CC}
	cflags:				      :	${CFLAGS}

	Maintainer mode:		      :	${USE_MAINTAINER_MODE}
"
