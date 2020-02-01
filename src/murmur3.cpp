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

// https://pypi.org/project/murmurhash3

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

#if defined(_MSC_VER)

// typedef unsigned char uint8_t;
// typedef unsigned long uint32_t;
// typedef unsigned __int64 uint64_t;

#include <stdlib.h>
#define ROTL32(x,y) _rotl(x,y)
#define ROTL64(x,y) _rotl64(x,y)
#define BIG_CONSTANT(x) (x)

#else // defined(_MSC_VER)
#include <stdint.h>

inline uint32_t rotl32(uint32_t x, int8_t r) {
    return (x << r) | (x >> (32 - r));
}

inline uint64_t rotl64(uint64_t x, int8_t r) {
    return (x << r) | (x >> (64 - r));
}

#define ROTL32(x,y) rotl32(x,y)
#define ROTL64(x,y) rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

#endif // if defined(_MSC_VER)


//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

inline uint32_t fmix32(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

inline uint64_t fmix(uint64_t k) {
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xff51afd7ed558ccd);
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
    k ^= k >> 33;

    return k;
}

//-----------------------------------------------------------------------------



void MurmurHash3_x86_32(const void *key, int len, uint32_t seed, void *out) {
    const uint8_t *data = (const uint8_t *)key;
    const int nblocks = len / 4;

    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    //----------
    // body

    const uint32_t *blocks = (const uint32_t *)(data + nblocks * 4);

    for (int i = -nblocks; i; i++) {
        uint32_t k1 = blocks[i];

        k1 *= c1;
        k1 = ROTL32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = ROTL32(h1, 13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    //----------
    // tail

    const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);

    uint32_t k1 = 0;

    switch (len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = ROTL32(k1, 15);
        k1 *= c2;
        h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;

    h1 = fmix32(h1);

    *(uint32_t *)out = h1;
}

void MurmurHash3_x64_128(const void *key, const int len,
                         const uint32_t seed, void *out) {
    const uint8_t *data = (const uint8_t *)key;
    const int nblocks = len / 16;

    uint64_t h1 = seed;
    uint64_t h2 = seed;

    const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
    const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

    //----------
    // body

    const uint64_t *blocks = (const uint64_t *)(data);

    for (int i = 0; i < nblocks; i++) {
        uint64_t k1 = blocks[i * 2 + 0];
        uint64_t k2 = blocks[i * 2 + 1];

        k1 *= c1;
        k1    = ROTL64(k1, 31);
        k1 *= c2;
        h1 ^= k1;

        h1 = ROTL64(h1, 27);
        h1 += h2;
        h1 = h1 * 5 + 0x52dce729;

        k2 *= c2;
        k2    = ROTL64(k2, 33);
        k2 *= c1;
        h2 ^= k2;

        h2 = ROTL64(h2, 31);
        h2 += h1;
        h2 = h2 * 5 + 0x38495ab5;
    }

    //----------
    // tail

    const uint8_t *tail = (const uint8_t *)(data + nblocks * 16);

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch (len & 15) {
    case 15:
        k2 ^= ((uint64_t)tail[14]) << 48;
    case 14:
        k2 ^= ((uint64_t)tail[13]) << 40;
    case 13:
        k2 ^= ((uint64_t)tail[12]) << 32;
    case 12:
        k2 ^= ((uint64_t)tail[11]) << 24;
    case 11:
        k2 ^= ((uint64_t)tail[10]) << 16;
    case 10:
        k2 ^= ((uint64_t)tail[9]) << 8;
    case    9:
        k2 ^= ((uint64_t)tail[ 8]) << 0;
        k2 *= c2;
        k2    = ROTL64(k2, 33);
        k2 *= c1;
        h2 ^= k2;
    case 8:
        k1 ^= ((uint64_t)tail[7]) << 56;
    case 7:
        k1 ^= ((uint64_t)tail[6]) << 48;
    case 6:
        k1 ^= ((uint64_t)tail[5]) << 40;
    case 5:
        k1 ^= ((uint64_t)tail[4]) << 32;
    case 4:
        k1 ^= ((uint64_t)tail[3]) << 24;
    case 3:
        k1 ^= ((uint64_t)tail[2]) << 16;
    case 2:
        k1 ^= ((uint64_t)tail[1]) << 8;
    case 1:
        k1 ^= ((uint64_t)tail[0]) << 0;
        k1 *= c1;
        k1 = ROTL64(k1, 31);
        k1 *= c2;
        h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;
    h2 ^= len;

    h1 += h2;
    h2 += h1;

    h1 = fmix(h1);
    h2 = fmix(h2);

    h1 += h2;
    h2 += h1;

    ((uint64_t *)out)[0] = h1;
    ((uint64_t *)out)[1] = h2;
}


// zend_class_entry * libhash_murmur3_ce;
zend_class_entry * libhash_murmur3_ce_ns;

ZEND_BEGIN_ARG_INFO_EX(libhash_murmur3_to32_arginfo, 0, 0, 0)
  ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(libhash_murmur3_to64_arginfo, 0, 0, 0)
  ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(libhash_murmur3_to128_arginfo, 0, 0, 0)
  ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()


/** {{{ proto public \OpenCV\ImgProc::__desctruct(void)
*/
PHP_METHOD(libhash_murmur3, test) {
  
  zend_string *strg;
  strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s", "opencv");

  RETURN_STR(strg);
}
/* }}} */


/** {{{ proto public \LibHash\Murmur3::to32($val)
*/
PHP_METHOD(libhash_murmur3, to32) {

  char *src = NULL;
  zval *self = NULL;
  size_t src_len;

  if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|s", &src, &src_len) == FAILURE) {
    return;
  }

  int32_t result[1];
  MurmurHash3_x86_32(src, src_len, 0, result);
  RETURN_LONG(result[0]);
}
/* }}} */


/** {{{ proto public \LibHash\Murmur3::to64($val)
*/
PHP_METHOD(libhash_murmur3, to64) {

  char *src = NULL;
  zval *self = NULL;
  size_t src_len;
  zval ret_val;
  array_init(&ret_val);

  if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|s", &src, &src_len) == FAILURE) {
    return;
  }

  uint64_t result[2];
  MurmurHash3_x64_128(src, src_len, 0, result);

  add_next_index_long(&ret_val, result[0]);
  add_next_index_long(&ret_val, result[1]);

  RETURN_ZVAL(&ret_val, 1, 1);
}
/* }}} */

/** {{{ proto public \LibHash\Murmur3::to128($val)
*/
PHP_METHOD(libhash_murmur3, to128) {

  char *src = NULL;
  zval *self = NULL;
  size_t src_len;

  if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|s", &src, &src_len) == FAILURE) {
    return;
  }

  uint64_t result[2];
  MurmurHash3_x64_128(src, src_len, 0, result);

  // php_printf("v:%lld", (unsigned char *)result[0]);
  // php_printf("v:%lld", (unsigned char *)result[1]);

  // RETURN_DOUBLE((unsigned char *)result);
}
/* }}} */

/** {{{ proto \LibHash\Murmur3::__construct()
*/
PHP_METHOD(libhash_murmur3, __construct) {
}
/* }}} */

/** {{{ proto public \LibHash\Murmur3::__desctruct(void)
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
  PHP_ME(libhash_murmur3, to32,                NULL,                      ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  PHP_ME(libhash_murmur3, to64,                NULL,                      ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
//PHP_ME(libhash_murmur3, to128,               NULL,                      ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  PHP_ME(libhash_murmur3, test,                NULL,                      ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
  PHP_ME(libhash_murmur3, __construct,         NULL,                      ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
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
