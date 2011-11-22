AC_INIT([jRadio], [0.1.3], [JohannesMarbach@googlemail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CXX
AC_PROG_MAKE_SET

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile data/Makefile src/Makefile])

PKG_CHECK_MODULES([gtkmm], [gtkmm-3.0])
AC_SUBST([gtkmm_CFLAGS])
AC_SUBST([gtkmm_LIBS])

PKG_CHECK_MODULES([libcurl], [libcurl])
AC_SUBST([libcurl_CFLAGS])
AC_SUBST([libcurl_LIBS])

PKG_CHECK_MODULES([libvlc], [libvlc])
AC_SUBST([libvlc_CFLAGS])
AC_SUBST([libvlc_LIBS])

PKG_CHECK_MODULES([libxml], [libxml-2.0])
AC_SUBST([libxml_CFLAGS])
AC_SUBST([libxml_LIBS])

AC_OUTPUT
