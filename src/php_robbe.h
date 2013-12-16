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
  | Author: chenxin - chenxin619315@gmail.com                            |
  | Link:   http://code.google.com/p/robbe                               |
  +----------------------------------------------------------------------+
*/

/* $Id: header 297205 2010-03-30 21:09:07Z johannes $ */

#ifndef PHP_ROBBE_H
#define PHP_ROBBE_H

extern zend_module_entry robbe_module_entry;
#define phpext_robbe_ptr &robbe_module_entry

#ifdef PHP_WIN32
#define PHP_ROBBE_API __declspec(dllexport)
#else
#define PHP_ROBBE_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include "friso_API.h"
#include "friso.h"

PHP_MINIT_FUNCTION(robbe);
PHP_MSHUTDOWN_FUNCTION(robbe);
PHP_RINIT_FUNCTION(robbe);
PHP_RSHUTDOWN_FUNCTION(robbe);
PHP_MINFO_FUNCTION(robbe);

PHP_FUNCTION(rb_split);
PHP_FUNCTION(rb_version);
PHP_FUNCTION(friso_version);
PHP_FUNCTION(rb_charset);
PHP_FUNCTION(rb_dic_exist);
PHP_FUNCTION(rb_dic_get);
PHP_FUNCTION(rb_utf8_bytes);
PHP_FUNCTION(rb_utf8_ucode);
PHP_FUNCTION(rb_ucode_utf8);

/*  */
/*ZEND_BEGIN_MODULE_GLOBALS(robbe)
  friso_t robbe_friso;
  friso_dic_t robbe_dic;
ZEND_END_MODULE_GLOBALS(robbe)*/
typedef struct {
  friso_t friso;
  friso_config_t config;
} zend_robbe_globals;


/* In every utility function you add that needs to use variables 
   in php_robbe_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as ROBBE_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define ROBBE_G(v) TSRMG(robbe_globals_id, zend_robbe_globals *, v)
#else
#define ROBBE_G(v) (robbe_globals.v)
#endif

#endif	/* PHP_ROBBE_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
