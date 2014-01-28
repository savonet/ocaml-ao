
# check for one particular file of the sources 
AC_INIT([ocaml-ao],[0.2.0],[savonet-users@lists.sourceforge.net])

VERSION=$PACKAGE_VERSION
AC_MSG_RESULT([configuring $PACKAGE_STRING])

AC_BASE_CHECKS()

PKG_PROG_PKG_CONFIG()
PKG_CONFIG_CHECK_MODULE([ao])

# Check if we are using ao version >= 1.0
if ! $PKG_CONFIG --atleast-version=1.0.0 ao; then
  CFLAGS="$CFLAGS -DAO_PRE_1"
fi

case "$host" in
   *-mingw*) LIBS="$LIBS dl";
esac

# substitutions to perform
AC_SUBST(VERSION)

# Finally create the Makefile and samples
AC_CONFIG_FILES([Makefile],[chmod a-w Makefile])
AC_CONFIG_FILES([src/META])
AC_CONFIG_FILES([src/Makefile])
AC_OUTPUT
