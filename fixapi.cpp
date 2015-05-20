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

extern "C" {
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_var.h"
}

#include "php_fixapi.h"
#include "fixapi.h"

zend_class_entry *fixapi_FixApi_ce;

BULLSOFT_INIT_CLASS(fixapi, FixApi);

BULLSOFT_INIT_FUNCS(fixapi_methods)
{
	PHP_ME(FixApi, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(FixApi, connect, NULL,  ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
	PHP_FE_END
};

BULLSOFT_INIT_CLASS(fixapi, FixApi)
{
	BULLSOFT_REGISTER_CLASS(fixapi, FixApi, FixApi, fixapi_methods, 0);
}

/* {{{ fixapi_module_entry
 */
zend_module_entry fixapi_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"fixapi",
	NULL,
	PHP_MINIT(fixapi),
	PHP_MSHUTDOWN(fixapi),
	PHP_RINIT(fixapi),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(fixapi),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(fixapi),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_FIXAPI_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_FIXAPI
extern "C" {
ZEND_GET_MODULE(fixapi)
}
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(fixapi)
{
	BULLSOFT_INIT(fixapi, FixApi);
	fixapi_FixApi_ce->create_object = create_fixapi_object;
	Fix_Initialize();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(fixapi)
{
	Fix_Uninitialize();
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(fixapi)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(fixapi)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(fixapi)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "fixapi support", "enabled");
	php_info_print_table_row(2, "Author", PHP_FIXAPI_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_FIXAPI_VERSION);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


zend_object_value create_fixapi_object(zend_class_entry *class_type TSRMLS_DC)
{
	zend_object_value retval;
	fixapi_object *intern;

	intern = (fixapi_object *)emalloc(sizeof(fixapi_object));
	memset(intern, 0, sizeof(fixapi_object));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(
	    intern,
		(zend_objects_store_dtor_t) zend_objects_destroy_object,
		(zend_objects_free_object_storage_t) free_fixapi_object,
		NULL TSRMLS_CC
	);
	retval.handlers = zend_get_std_object_handlers();

	return retval;
}

void free_fixapi_object(fixapi_object *intern TSRMLS_DC)
{
	zend_object_std_dtor(&intern->std TSRMLS_CC);
	printf("free fixapi object\n");
	efree(intern);
}

PHP_METHOD(FixApi, __construct)
{
	// printf("__construct");
}

PHP_METHOD(FixApi, connect)
{
	zval *object;
	fixapi_object *intern;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	MK_STD_INT(mode, 3);
	MK_STD_STRING(msg, "hello, world\n");
	MK_STD_STRING(file, "/tmp/test.log");

	bullsoft_call_func3("error_log", NULL, 3, msg, mode, file);
	bullsoft_call_func1("error_log", NULL, 1, msg);

	MK_STD_ARRAY(arr);
	add_next_index_long(arr, 1);
	add_next_index_long(arr, 2);

	MK_STD(retval);
	bullsoft_call_func1("json_encode", retval, 1, arr);
	php_var_dump(&retval, 0);
	// php_printf("asdfasd, %s\n", Z_STRVAL_P(retval));

	MK_STD(retval1);
	MK_STD_BOOL(assoc, 1);
	bullsoft_call_func2("json_decode", retval1, 2, retval, assoc);
	php_var_dump(&retval1, 0);
	
	object = getThis();
	intern = (fixapi_object *)zend_object_store_get_object(object TSRMLS_CC);
	intern->handle_conn = 1;
    RETURN_LONG(intern->handle_conn);
}



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
