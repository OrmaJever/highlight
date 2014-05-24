#ifdef WIN32
#  ifdef PHP_55
#     define PHP_COMPILER_ID "VC11"
#  else
#     define PHP_COMPILER_ID "VC9"
#  endif
#endif

#include "main.h"
#include "data.h"
#include "php.h"
#include "ext/standard/info.h"

PHP_FUNCTION(highlight)
{
	char *code, *sep = "\n";
	long len, sep_len;
	int flag = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s|l", &code, &len, &sep, &sep_len, &flag) == FAILURE) return;

	auto newCode = HL::Data::highlight(HL::Data::string(code), sep, flag);
	RETURN_STRING(newCode.c_str(), 1);
}

PHP_MINFO_FUNCTION(highlight)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Function name", "php_highlight()");
	php_info_print_table_row(2, "Version", VERSION);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

PHP_MINIT_FUNCTION(highlight)
{
	REGISTER_LONG_CONSTANT("HIGHLIGHT_ALL", 0,    CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIKE_PHP",		1<<0, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIKE_JS",		1<<1, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIKE_CSS",		1<<2, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIKE_HTML",		1<<3, CONST_CS | CONST_PERSISTENT);
}

PHP_RSHUTDOWN_FUNCTION(highlight)
{
	HL::Data::free();
}

const zend_function_entry highlight_functions[] = {
	ZEND_FE(highlight, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry highlight_module_entry = {
	STANDARD_MODULE_HEADER,
	"highlight",
	highlight_functions,
	PHP_MINIT(highlight),
	NULL, NULL,
	PHP_RSHUTDOWN(highlight),
	PHP_MINFO(highlight),
	VERSION,
	STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(highlight)

