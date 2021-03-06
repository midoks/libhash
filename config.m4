dnl $Id$
dnl config.m4 for extension libhash

dnl If your extension references something external, use with:

PHP_ARG_WITH(libhash, for libhash support,
Make sure that the comment is aligned:
[  --with-libhash             Include libhash support])


PHP_ARG_ENABLE(libhash, whether to enable libhash support,
Make sure that the comment is aligned:
[  --enable-libhash           Enable libhash support])

if test "$PHP_LIBHASH" != "no"; then

  dnl # --with-libhash -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/libhash.h"  # you most likely want to change this
  dnl if test -r $PHP_LIBHASH/$SEARCH_FOR; then # path given as parameter
  dnl   LIBHASH_DIR=$PHP_LIBHASH
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for libhash files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       LIBHASH_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$LIBHASH_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the libhash distribution])
  dnl fi

  dnl # --with-libhash -> add include path
  dnl PHP_ADD_INCLUDE($LIBHASH_DIR/include)

  dnl # --with-libhash -> check for lib and symbol presence
  dnl LIBNAME=libhash # you may want to change this
  dnl LIBSYMBOL=libhash # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $LIBHASH_DIR/$PHP_LIBDIR, LIBHASH_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_LIBHASHLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong libhash lib version or lib not found])
  dnl ],[
  dnl   -L$LIBHASH_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  
  PHP_SUBST(LIBHASH_SHARED_LIBADD)

  PHP_ADD_LIBRARY(stdc++,"",LIBHASH_SHARED_LIBADD)
  PHP_SUBST(LIBHASH_SHARED_LIBADD)
  PHP_REQUIRE_CXX()

  PHP_NEW_EXTENSION(libhash, libhash.cpp \
      src/murmur3.cpp \
      src/city.cpp, 
  $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)

  PHP_ADD_BUILD_DIR([$ext_builddir/src])
fi
