/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_FIXAPI_H
#define PHP_FIXAPI_H

extern zend_module_entry fixapi_module_entry;
#define phpext_fixapi_ptr &fixapi_module_entry

#define PHP_FIXAPI_EXTNAME "fixapi"
#define PHP_FIXAPI_VERSION "0.1.0"
#define PHP_FIXAPI_AUTHOR "Gu Weigang<guweigang@ucfgroup.com>"
#define PHP_FIXAPI_DESCRIPTOR_RES_NAME "wxlc_fixapi_resource_name"
#define PHP_FIXAPI_DEBUG 1

#include "bullsoft_php.h"
#include "fixapi.h"

#ifdef PHP_WIN32
#	define PHP_FIXAPI_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_FIXAPI_API __attribute__ ((visibility("default")))
#else
#	define PHP_FIXAPI_API
#endif

extern "C" {
#ifdef ZTS
#include "TSRM.h"
#endif
}

PHP_MINIT_FUNCTION(fixapi);
PHP_MSHUTDOWN_FUNCTION(fixapi);
PHP_RINIT_FUNCTION(fixapi);
PHP_RSHUTDOWN_FUNCTION(fixapi);
PHP_MINFO_FUNCTION(fixapi);

typedef struct _php_fixapi_descriptor
{
	HANDLE_CONN handle_conn;
	char *addr;
	char *user;
	char *pwd;
} php_fixapi_descriptor;

typedef struct _fixapi_object {
	zend_object std;
	zval *resource_id;
	zend_bool is_connected;
} fixapi_object;

zend_object_value create_fixapi_object(zend_class_entry *class_type TSRMLS_DC);
void free_fixapi_object(fixapi_object* intern TSRMLS_DC);

PHP_METHOD(FixApi, __construct);
PHP_METHOD(FixApi, connect);
PHP_METHOD(FixApi, isConnected);
PHP_METHOD(FixApi, getConnection);

PHP_METHOD(FixApi, setConnEvent);
PHP_METHOD(FixApi, close);
PHP_METHOD(FixApi, allocateSession);
PHP_METHOD(FixApi, releaseSession);

PHP_METHOD(FixApi, setAppInfo);
PHP_METHOD(FixApi, setDefaultInfo);
PHP_METHOD(FixApi, setTimeOut);
PHP_METHOD(FixApi, setWTFS);
PHP_METHOD(FixApi, setFBDM);
PHP_METHOD(FixApi, setDestFBDM);
PHP_METHOD(FixApi, setNode);
PHP_METHOD(FixApi, setGYDM);

PHP_METHOD(FixApi, createHead);
PHP_METHOD(FixApi, setString);
PHP_METHOD(FixApi, setLong);
PHP_METHOD(FixApi, setDouble);

PHP_METHOD(FixApi, run);
PHP_METHOD(FixApi, asyncRun);

PHP_METHOD(FixApi, isReplyed);
PHP_METHOD(FixApi, cancel);
PHP_METHOD(FixApi, getCode);
PHP_METHOD(FixApi, getErrMsg);
PHP_METHOD(FixApi, getCount);

PHP_METHOD(FixApi, haveItem);
PHP_METHOD(FixApi, getItem);
PHP_METHOD(FixApi, getLong);
PHP_METHOD(FixApi, getDouble);
PHP_METHOD(FixApi, setToken);
PHP_METHOD(FixApi, getToken);

PHP_METHOD(FixApi, encode);
PHP_METHOD(FixApi, addBackupSvrAddr);

PHP_METHOD(FixApi, subscibeByCustomer);
PHP_METHOD(FixApi, unSubscibeByHandle);

PHP_METHOD(FixApi, getColumnCount);


/* In every utility function you add that needs to use variables 
   in php_fixapi_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as FIXAPI_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define FIXAPI_G(v) TSRMG(fixapi_globals_id, zend_fixapi_globals *, v)
#else
#define FIXAPI_G(v) (fixapi_globals.v)
#endif

#endif	/* PHP_FIXAPI_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
