/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:midoks                                                        |
  +----------------------------------------------------------------------+
*/


extern "C"{
  #ifdef HAVE_CONFIG_H
  #include "config.h"
  #endif

  #include "php.h"
  #include "php_ini.h"
  #include "ext/standard/info.h"
  #include "ext/standard/php_var.h"
  #include "php_libhash.h"
}


zend_class_entry * libhash_murmur3_ce;
zend_class_entry * libhash_murmur3_ce_ns;


/** {{{ proto public \OpenCV\ImgProc::__desctruct(void)
*/
PHP_METHOD(libhash_murmur3, test) {
  
  zend_string *strg;
  strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s", "opencv");

  RETURN_STR(strg);
}
/* }}} */

/** {{{ proto public \OpenCV\ImgProc::__desctruct(void)
*/
PHP_METHOD(libhash_murmur3, __destruct) {
}
/* }}} */

/** {{{ proto private \LibHash\Murmur3::__sleep(void)
*/
PHP_METHOD(libhash_murmur3, __sleep) {
}
/* }}} */

/** {{{ proto private \LibHash\Murmur3::__wakeup(void)
*/
PHP_METHOD(libhash_murmur3, __wakeup) {
}
/* }}} */

/** {{{ proto private \LibHash\Murmur3::__clone(void)
*/
PHP_METHOD(libhash_murmur3, __clone) {
}
/* }}} */

/** {{{ libhash_murmur3_methods
*/
zend_function_entry libhash_murmur3_methods[] = {
  PHP_ME(libhash_murmur3, test,          NULL,                      ZEND_ACC_PUBLIC)
  PHP_ME(libhash_murmur3, __destruct,          NULL,                      ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
  PHP_ME(libhash_murmur3, __clone,             NULL,                      ZEND_ACC_PRIVATE)
  PHP_ME(libhash_murmur3, __sleep,             NULL,                      ZEND_ACC_PRIVATE)
  PHP_ME(libhash_murmur3, __wakeup,            NULL,                      ZEND_ACC_PRIVATE)
  {NULL, NULL, NULL}
};
/* }}} */

/** {{{ LIBHASH_STARTUP_FUNCTION
*/
LIBHASH_STARTUP_FUNCTION(murmur3) {

  // zend_class_entry ce;
  // zend_class_entry ce_ns;

  // LIBHASH_INIT_CLASS_ENTRY(ce, ce_ns, "LibHash_Murmur3", "LibHash\\Murmur3", libhash_murmur3_methods);

  // libhash_murmur3_ce = zend_register_internal_class_ex(&ce, NULL);
  // libhash_murmur3_ce_ns = zend_register_internal_class_ex(&ce_ns, NULL);


  zend_class_entry ce_ns;
  INIT_CLASS_ENTRY(ce_ns, "LibHash\\Murmur3", libhash_murmur3_methods);
  libhash_murmur3_ce_ns = zend_register_internal_class_ex(&ce_ns, NULL);

  return SUCCESS;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
