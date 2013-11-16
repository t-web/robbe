dnl $Id$
dnl config.m4 for extension robbe

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(robbe, for robbe support,
dnl Make sure that the comment is aligned:
dnl [  --with-robbe             Include robbe support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(robbe, whether to enable robbe support,
Make sure that the comment is aligned:
[  --enable-robbe           Enable robbe support])

if test "$PHP_ROBBE" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-robbe -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/robbe.h"  # you most likely want to change this
  dnl if test -r $PHP_ROBBE/$SEARCH_FOR; then # path given as parameter
  dnl   ROBBE_DIR=$PHP_ROBBE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for robbe files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ROBBE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ROBBE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the robbe distribution])
  dnl fi

  dnl # --with-robbe -> add include path
  dnl PHP_ADD_INCLUDE($ROBBE_DIR/include)

  dnl # --with-robbe -> check for lib and symbol presence
  dnl LIBNAME=robbe # you may want to change this
  dnl LIBSYMBOL=robbe # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ROBBE_DIR/lib, ROBBE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ROBBELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong robbe lib version or lib not found])
  dnl ],[
  dnl   -L$ROBBE_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ROBBE_SHARED_LIBADD)

  PHP_NEW_EXTENSION(robbe, robbe.c, $ext_shared)
fi
