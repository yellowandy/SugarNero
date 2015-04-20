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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_SugarNero.h"



/* pointer to the original Zend engine compile_file function */
typedef zend_op_array* (zend_compile_t)(zend_file_handle*, int TSRMLS_DC);
static zend_compile_t *old_compile_file;
zend_op_array* ext_zend_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC);
//static apc_serializer_t apc_serializers[APC_MAX_SERIALIZERS] = {{0,}};




ZEND_DECLARE_MODULE_GLOBALS(SugarNero)


/* True global resources - no need for thread safety here */
static int le_SugarNero;

/* {{{ SugarNero_functions[]
 *
 * Every user visible function must have an entry in SugarNero_functions[].
 */
const zend_function_entry SugarNero_functions[] = {
	PHP_FE(confirm_SugarNero_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in SugarNero_functions[] */
};
/* }}} */

/* {{{ SugarNero_module_entry
 */
zend_module_entry SugarNero_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"SugarNero",
	SugarNero_functions,
	PHP_MINIT(SugarNero),
	PHP_MSHUTDOWN(SugarNero),
	PHP_RINIT(SugarNero),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(SugarNero),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(SugarNero),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_SUGARNERO_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SUGARNERO
ZEND_GET_MODULE(SugarNero)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_BOOLEAN("SugarNero.enabled",      "0",    PHP_INI_SYSTEM, OnUpdateBool,              nero_enabled,         zend_SugarNero_globals, SugarNero_globals)
    STD_PHP_INI_ENTRY("SugarNero.flav", "ent", PHP_INI_ALL, OnUpdateString, nero_flav, zend_SugarNero_globals, SugarNero_globals)
PHP_INI_END()
/* }}} */

/* {{{ php_SugarNero_init_globals
 */
static void php_SugarNero_init_globals(zend_SugarNero_globals *SugarNero_globals)
{
	SugarNero_globals->nero_enabled = 0;
	SugarNero_globals->nero_flav = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(SugarNero)
{
	REGISTER_INI_ENTRIES();

    //If we are enabled we'll re-write all source code that is sent to us.
	if(SUGARNERO_G(nero_enabled)) {

	    old_compile_file = zend_compile_file;
        zend_compile_file = ext_zend_compile_file;
	}

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(SugarNero)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(SugarNero)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(SugarNero)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(SugarNero)
{
    printf("PRINTING TABLE");
    fputs("Hello World full of errors!", stderr);
	php_info_print_table_start();
	php_info_print_table_header(2, "SugarNero support", SUGARNERO_G(nero_enabled) ? "enabled" : "disabled");
	php_info_print_table_row(2, "Sugar Flavor", SUGARNERO_G(nero_flav));
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();

}
/* }}} */


zend_op_array* ext_zend_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
{

    char *buf;
    size_t size;
    if (zend_stream_fixup(file_handle, &buf, &size TSRMLS_CC) == FAILURE) {
        return NULL;
    }


    char *res = "<?php echo 'hello';";
    size_t res_size = 19;

    file_handle->handle.stream.mmap.buf = res;
    file_handle->handle.stream.mmap.len = res_size;


    return old_compile_file(file_handle, type TSRMLS_CC);
}


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_SugarNero_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_SugarNero_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "SugarNero", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
