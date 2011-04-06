
# check for one particular file of the sources 
AC_INIT(ocaml-ao, 0.2.0, savonet-users@lists.sourceforge.net)

VERSION=$PACKAGE_VERSION
AC_MSG_RESULT([configuring $PACKAGE_STRING])

# Add prefix to compilation variables
# if passed
if test "x$prefix" != "xNONE"; then
	CFLAGS="$CFLAGS -I$prefix/include"
	LDFLAGS="$LDFLAGS -L$prefix/lib"
	CPPFLAGS="$CPPFLAGS -I$prefix/include"
fi

AC_CHECK_OCAML_COMPILERS()

AC_PROG_CC()
AC_CHECK_TOOL([AR],[ar],no)
PKG_PROG_PKG_CONFIG()
#PKG_CHECK_MODULES(libao, ao, , [AC_MSG_ERROR(libao not found.)])

# PKG_CHECK_MODULES loses when you need --libs-only-[lL]
if ! $PKG_CONFIG --exists ao; then
        AC_MSG_ERROR([libao not found])
fi
libao_CFLAGS=`$PKG_CONFIG --cflags ao`
AC_SUBST([libao_CFLAGS])
libao_LIBS=`$PKG_CONFIG --libs-only-l ao`
AC_SUBST([libao_LIBS])
libao_LDFLAGS=`$PKG_CONFIG --libs-only-L ao`
AC_SUBST([libao_LDFLAGS])

# Check if we are using ao version >= 1.0
if ! pkg-config --atleast-version=1.0.0 ao; then
  CFLAGS="$CFLAGS -DAO_PRE_1"
fi

# substitutions to perform
AC_SUBST(VERSION)

# Finally create the Makefile and samples
AC_CONFIG_FILES([Makefile],[chmod a-w Makefile])
AC_CONFIG_FILES([src/META])
AC_CONFIG_FILES([src/Makefile])
AC_OUTPUT
