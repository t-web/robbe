/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
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

/* $Id: header 297205 2010-03-30 21:09:07Z johannes $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_robbe.h"

#define __ROBBE_VERSION__ "1.4"

//ZEND_DECLARE_MODULE_GLOBALS(robbe)
zend_robbe_globals robbe_globals;

/* True global resources - no need for thread safety here */
static int le_robbe = 1;

/* {{{ robbe_functions[]
 *
 * Every user visible function must have an entry in robbe_functions[].
 */
zend_function_entry robbe_functions[] = {
	PHP_FE(rb_split,	NULL)
	PHP_FE(rb_dic_exist,	NULL)
	PHP_FE(rb_dic_get,	NULL)
	PHP_FE(rb_utf8_bytes,	NULL)
	PHP_FE(rb_utf8_ucode,	NULL)
	PHP_FE(rb_ucode_utf8,	NULL)
	{NULL, NULL, NULL}	/* Must be the last line in robbe_functions[] */
};
/* }}} */

/* {{{ robbe_module_entry
 */
zend_module_entry robbe_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"robbe",
	robbe_functions,
	PHP_MINIT(robbe),
	PHP_MSHUTDOWN(robbe),
	PHP_RINIT(robbe),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(robbe),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(robbe),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ROBBE
ZEND_GET_MODULE(robbe)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini*/
PHP_INI_BEGIN()
    PHP_INI_ENTRY("robbe.ini_file", ".", PHP_INI_SYSTEM, NULL)
PHP_INI_END()
/* }}} */

/* {{{ php_robbe_globals_construct */
static void php_robbe_globals_construct(zend_robbe_globals *robbe_globals)
{
	robbe_globals->robbe_friso = friso_new_from_ifile( INI_STR("robbe.ini_file") );
	robbe_globals->friso_dic = robbe_globals->robbe_friso->dic;
}
/* }}} */

/* {{{ php_robbe_globals_destruct*/
static void php_robbe_globals_destruct(zend_robbe_globals *robbe_globals) 
{
	/*
	 * cause friso_free will free the dictionary
	 *		so here we don't have to call the friso_dic_free to free the
	 *	the robbe_dic global variable.		
	 */
	//friso_dic_free( robbe_globals->friso_dic );
	//robbe_globals->friso_dic = NULL;
	friso_free( robbe_globals->robbe_friso );
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(robbe)
{
	/*
	 * register some contants that robbe may use
	 *		at its following work.
	 *	the constant is case sensitive and persitent.
	 */
	REGISTER_LONG_CONSTANT("__RB_SIMPLE_MODE__",	1, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("__RB_COMPLEX_MODE__",	2, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("__RB_LEX_CJK_WORDS__",	
			__LEX_CJK_WORDS__, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("__RB_LEX_STOPWORDS__",	
			__LEX_STOPWORDS__, CONST_CS | CONST_PERSISTENT);

	REGISTER_INI_ENTRIES();
	/*initialize the globals variables.*/
	php_robbe_globals_construct( &robbe_globals );

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(robbe)
{
	UNREGISTER_INI_ENTRIES();
	/*destruct the globals variables*/
	php_robbe_globals_destruct(  &robbe_globals );

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(robbe)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(robbe)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(robbe)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Robbe Support", "enabled");
	php_info_print_table_row(2, "Version", __ROBBE_VERSION__);
	php_info_print_table_row(2, "Bug Report", "chenxin619315@gmail.com");
	php_info_print_table_row(2, "Learn More", "http://code.google.com/p/robbe");
	php_info_print_table_row(2, "C Edition", "http://code.google.com/p/friso");
	php_info_print_table_row(2, "Java Edition", "http://code.google.com/p/jcseg");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto array rb_split(string str, long mode)
   Return a array contains all the split result with a specified mode */
PHP_FUNCTION(rb_split)
{
	char *__str__ = NULL;
	int str_len, __idx__;
	unsigned long __mode__;
	zval *__result__;

	friso_task_t task;
	friso_t friso = robbe_globals.robbe_friso;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &__str__, &str_len, &__mode__) == FAILURE) {
		return;
	}

	//initialize the array.
	MAKE_STD_ZVAL( __result__ );
	array_init( __result__ );

	//create a new friso task.
	task = friso_new_task();
	if ( ! ( __mode__ == 1 || __mode__ == 2 ) )
		__mode__ = friso->mode;

	__idx__ = 0;
	friso_set_text(task, __str__);
	while ( friso_next( friso, __mode__, task ) != NULL ) {
		add_index_string( __result__, __idx__++, task->hits->word, 1 );
		//if ( task->hits->type == __FRISO_NEW_WORDS__ ) {
		//	add_index_string( __result__, __idx__++, task->hits->word, 1 );
		//	FRISO_FREE( task->hits->word );
		//} else {
		//	add_index_string( __result__, __idx__++, task->hits->word, 1 );
		//}
	}
	//free the friso task.
	friso_free_task(task);

	//RETURN_ZVAL( __result__, 0, 0);
	*( return_value ) = *( __result__ );
}
/* }}} */

/* {{{ proto bool rb_dic_exist(long type, string str)
   Return a bool to confirm that the given str is a word in a specified dictionary. */
PHP_FUNCTION(rb_dic_exist)
{
	char *__word__ = NULL;
	int wlen;
	long __type__;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &__type__, &__word__, &wlen) == FAILURE) {
		return;
	}

	if ( __type__ < 0 || __type__ >= __FRISO_LEXICON_LENGTH__ ) {
		__type__ = __LEX_CJK_WORDS__;
	}

	wlen = friso_dic_match( robbe_globals.friso_dic, __type__, __word__ );

	RETURN_BOOL(wlen);
}
/* }}} */

/* {{{ proto array rb_dic_get(long type, string str)
   Return a array contains all the information of the given word.*/
PHP_FUNCTION(rb_dic_get)
{
	char *__word__ = NULL;
	int wlen;
	long __type__;
	zval *__entry__;
	lex_entry_t e;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &__type__, &__word__, &wlen) == FAILURE) {
		return;
	}

	MAKE_STD_ZVAL( __entry__ );
	array_init( __entry__ );

	if ( __type__ < 0 || __type__ >= __FRISO_LEXICON_LENGTH__ ) {
		__type__ = __LEX_CJK_WORDS__;
	}

	e = friso_dic_get( robbe_globals.friso_dic, __type__, __word__ );
	if ( e != NULL ) {
		add_assoc_long( __entry__, "length", e->length);
		add_assoc_long( __entry__, "freq", e->fre );
		*( return_value ) = * ( __entry__ );
		return;
	}

	RETURN_BOOL(0);
}
/* }}} */

/* {{{ proto long rb_utf8_bytes(string str)
   Return the bytes that the utf-8 char takes.*/
PHP_FUNCTION(rb_utf8_bytes)
{
	char *__word__ = NULL;
	int wlen, _bytes;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &__word__, &wlen) == FAILURE) {
		return;
	}

	if ( __word__ == NULL ) {
		RETURN_LONG(0);
	} 

	_bytes = get_utf8_bytes( __word__[0] );

	RETURN_LONG(_bytes);
}
/* }}} */

/* {{{ proto long rb_utf8_ucode(string str)
   Return the unicode of the given utf-8 char.*/
PHP_FUNCTION(rb_utf8_ucode)
{
	char *__word__ = NULL;
	int wlen, _ucode;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &__word__, &wlen) == FAILURE) {
		return;
	}

	_ucode = get_utf8_unicode( __word__ );

	RETURN_LONG(_ucode);
}
/* }}} */

/* {{{ proto string rb_ucode_utf8(long ucode)
   Return char that the a unicode pointed to.*/
PHP_FUNCTION(rb_ucode_utf8)
{
	unsigned long *__ucode__ = NULL;
	int _bytes;
	char __word__[7];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &__ucode__ ) == FAILURE) {
		return;
	}

	_bytes = unicode_to_utf8( ( size_t ) __ucode__, __word__ );
	__word__[_bytes] = '\0';

	RETURN_STRINGL( __word__, _bytes, 1 );
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
