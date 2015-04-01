dnl $Id$
dnl config.m4 for extension SugarNero

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(SugarNero, for SugarNero support,
dnl Make sure that the comment is aligned:
dnl [  --with-SugarNero             Include SugarNero support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(SugarNero, whether to enable SugarNero support,
Make sure that the comment is aligned:
[  --enable-SugarNero           Enable SugarNero support])

if test "$PHP_SUGARNERO" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-SugarNero -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/SugarNero.h"  # you most likely want to change this
  dnl if test -r $PHP_SUGARNERO/$SEARCH_FOR; then # path given as parameter
  dnl   SUGARNERO_DIR=$PHP_SUGARNERO
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for SugarNero files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SUGARNERO_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SUGARNERO_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the SugarNero distribution])
  dnl fi

  dnl # --with-SugarNero -> add include path
  dnl PHP_ADD_INCLUDE($SUGARNERO_DIR/include)

  dnl # --with-SugarNero -> check for lib and symbol presence
  dnl LIBNAME=SugarNero # you may want to change this
  dnl LIBSYMBOL=SugarNero # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SUGARNERO_DIR/lib, SUGARNERO_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SUGARNEROLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong SugarNero lib version or lib not found])
  dnl ],[
  dnl   -L$SUGARNERO_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SUGARNERO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(SugarNero, SugarNero.c, $ext_shared)
fi
