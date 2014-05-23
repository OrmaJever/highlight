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

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &code, &len, &sep, &sep_len) == FAILURE) return;

	auto newCode = HL::Data::highlight(HL::Data::string(code), sep);
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
	NULL, NULL, NULL,
	PHP_RSHUTDOWN(highlight),
	PHP_MINFO(highlight),
	VERSION,
	STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(highlight)

