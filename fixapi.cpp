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
#define NCF_FIXAPI_HASH_KEY_PATTERN "ncffixapi:%s:%s:%s"

static int le_fixapi_descriptor;
static int le_fixapi_descriptor_persist;

zend_class_entry *fixapi_FixApi_ce;

BULLSOFT_INIT_CLASS(fixapi, FixApi);

BULLSOFT_INIT_FUNCS(fixapi_methods)
{
	PHP_ME(FixApi, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(FixApi, connect, NULL,  ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(FixApi, isConnected, NULL,  ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
	PHP_ME(FixApi, getConnection, NULL,  ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
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

static void php_fixapi_descriptor_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	if(rsrc->ptr != NULL) {
		php_fixapi_descriptor *fdata = (php_fixapi_descriptor *)rsrc->ptr;
		// Fix_Close(fdata->handle_conn);
		efree(fdata->addr);
		efree(fdata->user);
		efree(fdata->pwd);
		fdata = NULL;
	}
}

static void php_fixapi_descriptor_dtor_persistent(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	if(rsrc->ptr != NULL) {
		php_fixapi_descriptor *fdata = (php_fixapi_descriptor *)rsrc->ptr;
		// Fix_Close(fdata->handle_conn);
		pefree(fdata->addr, 1);
		pefree(fdata->user, 1);
		pefree(fdata->pwd,  1);
	}
}


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(fixapi)
{
	BULLSOFT_INIT(fixapi, FixApi);
	fixapi_FixApi_ce->create_object = create_fixapi_object;

	le_fixapi_descriptor =
		zend_register_list_destructors_ex(php_fixapi_descriptor_dtor, 
										  NULL,
										  PHP_FIXAPI_DESCRIPTOR_RES_NAME,
										  module_number);
	
	le_fixapi_descriptor_persist =
		zend_register_list_destructors_ex(NULL,
										  php_fixapi_descriptor_dtor_persistent,
										  PHP_FIXAPI_DESCRIPTOR_RES_NAME,
										  module_number);
	
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
}
/* }}} */

zend_object_value create_fixapi_object(zend_class_entry *class_type TSRMLS_DC)
{
	zend_object_value retval;
	fixapi_object *intern;

	intern = (fixapi_object *)emalloc(sizeof(fixapi_object));
	memset(intern, 0, sizeof(fixapi_object));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	
	intern->is_connected = 0;
	MAKE_STD_ZVAL(intern->resource_id);
	
	bullsoft_init_properties(intern);
	
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
	FREE_STD(intern->resource_id);
	efree(intern);
}

PHP_METHOD(FixApi, __construct)
{
	// printf("__construct");
}

void log(const char *str)
{
	if (PHP_FIXAPI_DEBUG == 1) {
		MK_STD_INT(mode, 3);
		MK_STD_STRING(msg, str);
		MK_STD_STRING(file, "/tmp/test.log");
		bullsoft_call_func3("error_log", NULL, 3, msg, mode, file);
		FREE_STD(mode);
		FREE_STD(msg);
		FREE_STD(file);
	}
}

PHP_METHOD(FixApi, getConnection)
{
	zval *object;
	fixapi_object *intern;
	object = getThis();
	intern = (fixapi_object *)zend_object_store_get_object(object TSRMLS_CC);
	if (Z_TYPE_P(intern->resource_id) != IS_RESOURCE) {
		RETURN_FALSE;
	}
	
	php_fixapi_descriptor *fdata;
	ZEND_FETCH_RESOURCE2(
        fdata,
		php_fixapi_descriptor*,
		&(intern->resource_id),
		-1,
		PHP_FIXAPI_DESCRIPTOR_RES_NAME,
		le_fixapi_descriptor,
		le_fixapi_descriptor_persist
    );
	
	ZVAL_STRING(return_value, fdata->addr, 1);
}

PHP_METHOD(FixApi, isConnected)
{
	zval *object;
	fixapi_object *intern;
	object = getThis();
	intern = (fixapi_object *)zend_object_store_get_object(object TSRMLS_CC);
	
	RETURN_BOOL(intern->is_connected);
}

PHP_METHOD(FixApi, connect)
{
	zval *object;
	fixapi_object *intern;
	
	php_fixapi_descriptor *fdata;
	zend_bool persist = 0;

	char *addr, *user, *pwd;
	int addr_len, user_len, pwd_len;

	char *hash_key;
	int hash_key_len;
	zend_rsrc_list_entry *existing_rsrc;
	
	long handle_conn;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"sss|b", &addr, &addr_len, &user, &user_len, &pwd, &pwd_len, &persist) == FAILURE) {
		return;
	}

	object = getThis();
	intern = (fixapi_object *)zend_object_store_get_object(object TSRMLS_CC);
	
	hash_key_len = spprintf(&hash_key, 0, NCF_FIXAPI_HASH_KEY_PATTERN, addr, user, pwd);
	printf("hash key: %s\n", hash_key);

	if (zend_hash_find(&EG(persistent_list), hash_key, hash_key_len + 1, (void **)&existing_rsrc) == SUCCESS) {
		log("there is an exists link here\n");
		// 存在一个，直接使用！
		fdata = (php_fixapi_descriptor *) existing_rsrc->ptr;
		// if (Fix_IsConnect(fdata->handle_conn)) {
		// @hardcode
		if (fdata->handle_conn == 1) {
			ZEND_REGISTER_RESOURCE(intern->resource_id, existing_rsrc->ptr, le_fixapi_descriptor_persist);
			efree(hash_key);
			RETURN_TRUE;
		}
		zend_hash_del(&EG(persistent_list), hash_key, hash_key_len + 1);
	}

	log("open a new link here...with...");
	// Fix_ConnectEx(addr, user, pwd);
	// @hardcode
	handle_conn = 1;
	
	if (!persist) {
		log(" non-persist.\n");
		fdata = (php_fixapi_descriptor *) emalloc(sizeof(php_fixapi_descriptor));
		fdata->addr = estrndup(addr, addr_len);
		fdata->user = estrndup(user, user_len);
		fdata->pwd  = estrndup(pwd, pwd_len);
		fdata->handle_conn = handle_conn;
		ZEND_REGISTER_RESOURCE(intern->resource_id, fdata, le_fixapi_descriptor);
	} else {
		log(" persist.\n");
		zend_rsrc_list_entry le;
		fdata = (php_fixapi_descriptor *)pemalloc(sizeof(php_fixapi_descriptor), 1);
		
		fdata->addr = (char *)pemalloc(addr_len + 1, 1);
		memcpy(fdata->addr, addr, addr_len + 1);

		fdata->user = (char *)pemalloc(user_len + 1, 1);
		memcpy(fdata->user, user, user_len + 1);

		fdata->pwd = (char *)pemalloc(pwd_len + 1, 1);
		memcpy(fdata->pwd, pwd, pwd_len + 1);

		fdata->handle_conn = handle_conn;

		// 在EG(regular_list中存一份)
		ZEND_REGISTER_RESOURCE(intern->resource_id, fdata, le_fixapi_descriptor_persist);

		// 在EG(persistent_list)中再存一份
		le.type = le_fixapi_descriptor_persist;
		le.ptr = fdata;
		
		zend_hash_update(&EG(persistent_list), hash_key, hash_key_len + 1, (void*)&le, sizeof(zend_rsrc_list_entry), NULL);
		efree(hash_key);
	}
	
	intern->is_connected = 1;
	RETURN_TRUE;
}



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
