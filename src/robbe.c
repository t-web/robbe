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

#define __ROBBE_VERSION__ "1.6.0"
#ifdef _WIN32
#	define robbe_default_ifile "c:/windows/friso.ini"
#else
#	define robbe_default_ifile "/etc/friso/friso.ini"
#endif

//ZEND_DECLARE_MODULE_GLOBALS(robbe)
zend_robbe_globals robbe_globals;

/* True global resources - no need for thread safety here */
static int le_robbe = 1;

/* {{{ robbe_functions[]
 *
 * Every user visible function must have an entry in robbe_functions[].
 */
zend_function_entry robbe_functions[] = {
	PHP_FE(rb_split,		NULL)
	PHP_FE(rb_version,		NULL)
	PHP_FE(friso_version,	NULL)
	PHP_FE(rb_charset,		NULL)
	PHP_FE(rb_dic_exist,	NULL)
	PHP_FE(rb_dic_get,		NULL)
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
    PHP_INI_ENTRY("robbe.ini_file", robbe_default_ifile, PHP_INI_SYSTEM, NULL)
PHP_INI_END()
/* }}} */

/* {{{ php_robbe_globals_construct */
static void php_robbe_globals_construct(zend_robbe_globals *robbe_globals)
{
	robbe_globals->friso = friso_new();
	robbe_globals->config = friso_new_config();
	friso_init_from_ifile(robbe_globals->friso,
			robbe_globals->config, INI_STR("robbe.ini_file"));
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
	friso_free_config( robbe_globals->config );
	friso_free( robbe_globals->friso );
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
	char *_str = NULL, *_key;
	int slen, idx, klen;

	zval *ret, *cfg, **data;
	HashTable *cfgArr;
	HashPosition pointer;

	friso_task_t task;
	friso_config_t nconfig = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &_str, &slen, &cfg) == FAILURE) {
		return;
	}

	//check and initialize the friso.
	if ( Z_TYPE_P(cfg) != IS_NULL ) {
		nconfig = friso_new_config();
		memcpy(nconfig, robbe_globals.config, sizeof(friso_config_entry));

		//check the new setting.
		cfgArr = Z_ARRVAL_P(cfg);
		//zend_printf("array length: %d", zend_hash_num_elements(cfgArr));
		for ( zend_hash_internal_pointer_reset_ex(cfgArr, &pointer); 
			zend_hash_get_current_data_ex(cfgArr, (void **)&data, &pointer) == SUCCESS;
			zend_hash_move_forward_ex(cfgArr, &pointer) ) 
		{
			zend_hash_get_current_key_ex(cfgArr, &_key, &klen, NULL, 0, &pointer);
			//zend_printf("key: %s, value: %d<br />", _key, (*data)->value.lval);
			//convert the data to long.
			convert_to_long_ex(data);
			if ( strcmp(_key, "max_len") == 0 )
				nconfig->max_len = (ushort_t)(*data)->value.lval;
			else if ( strcmp(_key, "r_name") == 0 )
				nconfig->r_name = (ushort_t)(*data)->value.lval;
			else if ( strcmp(_key, "mix_len") == 0 )
				nconfig->mix_len = (ushort_t)(*data)->value.lval;
			else if ( strcmp(_key, "lna_len") == 0 )
				nconfig->lna_len = (ushort_t)(*data)->value.lval;
			else if ( strcmp(_key, "add_syn") == 0 )
				nconfig->add_syn = (ushort_t)(*data)->value.lval;
			else if ( strcmp(_key, "clr_stw") == 0 )
				nconfig->clr_stw = (ushort_t)(*data)->value.lval;
			else if ( strcmp(_key, "add_syn") == 0 )
				nconfig->add_syn = (ushort_t)(*data)->value.lval;
			else if ( strcmp(_key, "keep_urec") == 0 )
				nconfig->keep_urec = (ushort_t)(*data)->value.lval;
			else if ( strcmp(_key, "spx_out") == 0 )
				nconfig->spx_out = (ushort_t)(*data)->value.lval;
			else if ( strcmp(_key, "nthreshold") == 0 )
				nconfig->nthreshold = (uint_t) (*data)->value.lval;
			else if ( strcmp(_key, "mode") == 0 )
				nconfig->mode = (friso_mode_t) (*data)->value.lval;
		}
	}
 
	//initialize the array.
	MAKE_STD_ZVAL( ret );
	array_init( ret );

	//create a new friso task.
	task = friso_new_task();
	idx = 0;
	friso_set_text(task, _str);
	while ( friso_next( robbe_globals.friso,
			nconfig == NULL ? robbe_globals.config : nconfig, task ) != NULL ) {
		add_index_string( ret, idx++, task->hits->word, 1 );
	}

	//free the friso task.
	friso_free_task(task);
	if ( nconfig != NULL ) friso_free_config(nconfig);

	//RETURN_ZVAL( ret, 0, 0);
	*( return_value ) = *( ret );
}
/* }}} */

/* {{{ proto string rb_version()
   Return the current version of robbe. */
PHP_FUNCTION(rb_version)
{
	RETURN_STRINGL(__ROBBE_VERSION__, strlen(__ROBBE_VERSION__), 1);
}
/* }}} */

/* {{{ proto string friso_version()
   Return the current version of Friso. */
PHP_FUNCTION(friso_version)
{
	RETURN_STRINGL(FRISO_VERSION, strlen(FRISO_VERSION), 1);
}
/* }}} */

/* {{{ proto string rb_charset()
   Return the current charset of friso. */
PHP_FUNCTION(rb_charset)
{
	char *charset = robbe_globals.friso->charset == FRISO_UTF8 ? "UTF-8" : "GBK";
	RETURN_STRINGL(charset, strlen(charset), 1);
}
/* }}} */

/* {{{ proto bool rb_dic_exist(long type, string str)
   Return a bool to confirm that the given str is a word in a specified dictionary. */
PHP_FUNCTION(rb_dic_exist)
{
	char *word = NULL;
	int wlen;
	long type;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &type, &word, &wlen) == FAILURE) {
		return;
	}

	if ( robbe_globals.friso->dic == NULL )
		RETURN_BOOL(0);

	if ( type < 0 || type >= __FRISO_LEXICON_LENGTH__ )
		type = __LEX_CJK_WORDS__;

	wlen = friso_dic_match( robbe_globals.friso->dic, type, word );

	RETURN_BOOL(wlen);
}
/* }}} */

/* {{{ proto array rb_dic_get(long type, string str)
   Return a array contains all the information of the given word.*/
PHP_FUNCTION(rb_dic_get)
{
	char *word = NULL;
	int wlen;
	long type;
	zval *entry;
	lex_entry_t e;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &type, &word, &wlen) == FAILURE) {
		return;
	}

	//check the dictionary
	if ( robbe_globals.friso->dic == NULL )
		RETURN_BOOL(0);

	MAKE_STD_ZVAL( entry );
	array_init( entry );

	if ( type < 0 || type >= __FRISO_LEXICON_LENGTH__ ) {
		type = __LEX_CJK_WORDS__;
	}

	e = friso_dic_get( robbe_globals.friso->dic, type, word );
	if ( e != NULL ) {
		add_assoc_long( entry, "length", e->length);
		add_assoc_long( entry, "freq", e->fre );
		*( return_value ) = * ( entry );
		return;
	}

	RETURN_BOOL(0);
}
/* }}} */

/* {{{ proto long rb_utf8_bytes(string str)
   Return the bytes that the utf-8 char takes.*/
PHP_FUNCTION(rb_utf8_bytes)
{
	char *word = NULL;
	int wlen, _bytes;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &wlen) == FAILURE) {
		return;
	}

	if ( word == NULL ) RETURN_LONG(0);
	_bytes = get_utf8_bytes( word[0] );

	RETURN_LONG(_bytes);
}
/* }}} */

/* {{{ proto long rb_utf8_ucode(string str)
   Return the unicode of the given utf-8 char.*/
PHP_FUNCTION(rb_utf8_ucode)
{
	char *word = NULL;
	int wlen, _ucode;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &wlen) == FAILURE) {
		return;
	}

	_ucode = get_utf8_unicode( word );

	RETURN_LONG(_ucode);
}
/* }}} */

/* {{{ proto string rb_ucode_utf8(long ucode)
   Return char that the a unicode pointed to.*/
PHP_FUNCTION(rb_ucode_utf8)
{
	unsigned long *ucode = NULL;
	int _bytes;
	char word[7];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &ucode ) == FAILURE) {
		return;
	}

	_bytes = unicode_to_utf8( ( size_t ) ucode, word );
	word[_bytes] = '\0';

	RETURN_STRINGL( word, _bytes, 1 );
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
