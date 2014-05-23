#define _CSS_ // for words.h

#include "../words.h"
#include "css.h"

using namespace HL;

void CSS::operator()( const char end )
{
	i += space(true);

	while((end && $(i) != end) || (!end && $(i) != '<' && !$_("</style>"))) try {
		if(Size < i) break;

		parse_key();
		if($(i) == ':') parse_value();
		if($_("/*")) parse_comment();
		if($(i) == '[') parse_selector();
		parse_tag();

		htmlchars($(i++));
	} catch( CSSCode ) {
		continue;
	}
	throw HTMLCode();
}

void CSS::parse_key()
{
	auto Token = CALL(5);

	if($(i+space()) != ':' || Token.empty()) {
		i -= Token.size();
		return;
	}
	++i;
	auto Tok = CALL(4);
	if(in_array(Tok, pseClass)) {
		i -= Token.size()+Tok.size()+1;
		return;
	}

	i -= Tok.size()+1;
	newCode << TAG( CSS_KEY ) << Token << END_TAG;
	throw CSSCode();
}

void CSS::parse_value()
{
	newCode << ':';
	++i;
	i += space(true);

	auto Token = CALL(4);

	if(in_array(Token, pseClass)) {
		newCode << TAG( CSS_PSE ) << Token;
	} else {
		i -= Token.size();
		newCode << TAG( CSS_VALUE );
		while($(i) != ';' && Size > i) {
			htmlchars($(i++));
		}
	}

	newCode << END_TAG;
	throw CSSCode();
}

void CSS::parse_tag()
{
	if($(i) == '.') {
		++i;
		newCode << TAG( CSS_CLASS ) << '.' << CALL(7) << END_TAG;
	} else if($(i) == '#') {
		++i;
		newCode << TAG( CSS_ID ) << '#' << CALL(7) << END_TAG;
	} else if(($(i) >= 'A' && $(i) <= 'Z') || ($(i) >= 'a' && $(i) <= 'z')) {
		newCode << TAG( CSS_TAG ) << CALL(2) << END_TAG;
	} else {
		return;
	}
	throw CSSCode();
}

void CSS::parse_comment()
{
	newCode << TAG( CSS_COMMENT );
	while(!$_("*/") && Size > i) {
		if($(i) == '\n') {
			newCode << END_TAG;
			i += space(true);
			newCode << TAG( CSS_COMMENT );
		} else {
			htmlchars($(i++));
		}
	}
	newCode << END_TAG;
	throw CSSCode();
}

void CSS::parse_selector()
{
	newCode << '[';
	++i;
	i += space(true);
	newCode << TAG( CSS_SELNAME ) << CALL(2) << END_TAG;

	while($(i) != '"' && Size > i)
		htmlchars($(i++));
	++i;
	newCode << TAG( CSS_SELVALUE ) << '"';

	while($(i) != '"' && Size > i)
		htmlchars($(i++));

	newCode << '"' << END_TAG;
	++i;
	i += space(true);
	throw CSSCode();
}

