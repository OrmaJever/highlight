#define _JS_ // for words.h

#include "../words.h"
#include "js.h"

using namespace HL;

void JS::operator()( const char end ) throw(HTMLCode)
{
	i += space(true);

	while((end && ($(i) != end || $(i-1) == '\\')) || (!end && !$_("</script>"))) try {
		if(Size < i) break;

		if($(i) == '\'' || $(i) == '"') parse_String();
		if($(i) >= '0' && $(i) <= '9') parse_Numbers();
		if(($(i) >= 'A' && $(i) <= 'Z') || ($(i) >= 'a' && $(i) <= 'z')) parse_Word();
		if($_("//") || $_("/*")) parse_Comment();
		if($(i) == '/') parse_RegExp();

		htmlchars($(i++));
	} catch( JSCode ) {
		continue;
	}

	throw HTMLCode();
}

void JS::parse_String() throw(JSCode)
{
	char quote = $(i++);
	newCode << TAG( JS_STRING );
	htmlchars(quote);

	while((quote != $(i) || ($(i-1) == '\\' && $(i-2) != '\\')) && Size > i) {
		if($(i) == '\n') {
			newCode << END_TAG;
			i += space(true);
			newCode << TAG( JS_STRING );
		} else {
			htmlchars($(i++));
		}
	}
	htmlchars($(i++));
	newCode << END_TAG;
	throw JSCode();
}

void JS::parse_Numbers() throw(JSCode)
{
	if($(i) >= '0' && $(i) <= '9') {
		newCode << TAG( JS_NUMBER );
		while($(i) >= '0' && $(i) <= '9') {
			newCode << $(i++);
		}
	} else if($_("0x")) {
		i += 2;
		newCode << TAG( JS_HEX_NUMBER ) "0x";
		while(($(i) >= '0' && $(i) <= '9') || ($(i) >= 'A' && $(i) <= 'F') || ($(i) >= 'a' && $(i) <= 'f')) {
			newCode << $(i++);
		}
	}
	newCode << END_TAG;
	throw JSCode();
}

void JS::parse_Comment() throw(JSCode)
{
	if(($_("//"))) {
		newCode << TAG( JS_L_COMMENT ) "//";
		i += 2;

		while($(i) != '\n' && Size > i) {
			htmlchars($(i++));
		}

		newCode << END_TAG;
		throw JSCode();
	} else if($_("/*")) {
		newCode << TAG( JS_D_COMMENT ) "/*";
		i += 2;

		while(!$_("*/") && Size > i) {
			if($(i) == '\n') {
				newCode << END_TAG;
				i += space(true);
				newCode << TAG( JS_D_COMMENT );
			} else {
				htmlchars($(i++));
			}
		}
		if( Size > i ) {
			newCode << "*/" END_TAG;
			i += 2;
		}
		throw JSCode();
	}
}

void JS::parse_RegExp() throw(JSCode)
{
	char prev = $(i+space(false, -1)-1);
	if(prev != '(' && prev != '(' && prev != ':')
		return;

	newCode << TAG( JS_REGEXP ) << $(i++);

	while(($(i) != '/' || $(i-1) == '\\') && Size > i) {
		htmlchars($(i++));
	}

	newCode << $(i++);
	while($(i) == 'i' || $(i) == 'g' || $(i) == 'm') {
		newCode << $(i++);
	}

	newCode << END_TAG;
	throw JSCode();
}

void JS::parse_Word() throw(JSCode)
{
	auto Token = CALL(1);

	parse_Keyword(Token);
	parse_Variable(Token);

	newCode << Token;
	throw JSCode();
}

void JS::parse_Keyword( const string &word ) throw(JSCode)
{
	if(!in_array(word, keyword))
		return;

	newCode << TAG( JS_KEYWORD ) << word << END_TAG;

	throw JSCode();
}

void JS::parse_Variable( const string &word ) throw(JSCode)
{
	char next = $(i+space());
	if(next == '(') {
		newCode << TAG( JS_FUNCT ) << word << END_TAG;
		throw JSCode();
	}
	if(next == '.' || next == ':' || $(i-word.size()-1) == '.') {
		newCode << TAG( JS_OBJECT ) << word << END_TAG;
		throw JSCode();
	}

	newCode << TAG( JS_VARIABLE ) << word << END_TAG;
	throw JSCode();
}
