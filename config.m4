dnl $Id$
dnl config.m4 for extension fixapi

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(fixapi, for fixapi support,
[  --with-fixapi             Include fixapi support])

if test "$PHP_FIXAPI" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-fixapi -> check with-path
  SEARCH_FOR="/include/fixapi.h"  # you most likely want to change this
   if test -r $PHP_FIXAPI/$SEARCH_FOR; then # path given as parameter
     FIXAPI_DIR=$PHP_FIXAPI
  fi
  
  if test -z "$FIXAPI_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the fixapi distribution])
  fi

  dnl # --with-fixapi -> add include path
  PHP_ADD_INCLUDE($FIXAPI_DIR/include/)

  PHP_REQUIRE_CXX()
  
  dnl # --with-fixapi -> check for lib and symbol presence
  LIBNAME=fixapi # you may want to change this
  LIBSYMBOL=Fix_Initialize # you most likely want to change this 

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $FIXAPI_DIR/lib, FIXAPI_SHARED_LIBADD)
    AC_DEFINE(HAVE_FIXAPILIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong fixapi lib version or lib not found])
  ],[
    -L$FIXAPI_DIR/lib -lm
  ])
  
  PHP_SUBST(FIXAPI_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, FIXAPI_SHARED_LIBADD)
  
  PHP_NEW_EXTENSION(fixapi, fixapi.cpp, $ext_shared)
fi
