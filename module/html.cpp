#define _HTML_

#include "../words.h"
#include "html.h"
#include "js.h"
#include "css.h"

using namespace HL;

void HTML::operator()( void )
{
	while( (!flag || flag == 8) && Size > i ) try {
		if($_("<?")) throw PHPCode();

		if($_("<!--")) parse_comment();
		if($(i) == '<') parse_tag();

		htmlchars($(i++));
	} catch( HTMLCode ) {
		continue;
	};
}

void HTML::parse_comment( void ) throw(HTMLCode)
{
	newCode << TAG( HTML_COMMENT );

	while(!$_("-->") && Size > i) {
		if($(i) == '\n') {
			newCode << END_TAG;
			i += space(true);
			newCode << TAG( HTML_COMMENT );
		} else {
			htmlchars($(i++));
		}
	}
	newCode << END_TAG;
	throw HTMLCode();
}

void HTML::parse_tag( void ) throw(HTMLCode, CSSCode, JSCode)
{
	newCode << "&lt;";

	bool endTag = false;

	if($(++i) == '/') {
		newCode << '/';
		endTag = true;
		++i;
	}

	if($_("!DOCTYPE")) {
		while($(i) != '>' && Size > i) {
			htmlchars($(i++));
		}
		throw HTMLCode();
	}

	auto Token = CALL(3);

	newCode << TAG( HTML_TAG ) << Token << END_TAG;
	i += space(true);

	while($(i) != '>' && $(i) != '/' && Code.size() > i) {
		parse_attr();
		i += space(true);
	}

	if($(i) == '/') {
		newCode << '/';
		++i;
	}
	if($(i) == '>') {
		newCode << "&gt;";
		++i;
	}

	if(!endTag && !Token.empty()) {
		auto lt = Token.toLower();
		if(lt == "style") {
			throw CSSCode();
		} else if(lt == "script") {
			throw JSCode();
		}
	}

	throw HTMLCode();
}

void HTML::parse_attr( void )
{
	newCode << TAG( HTML_NAME );
	auto Token = CALL(4);

	if(Token.empty()) {
		htmlchars($(i++));
		throw HTMLCode();
	}

	newCode << Token << END_TAG;
	i += space(true);

	if($(i) != '=')
		throw HTMLCode();

	newCode << $(i++);
	auto TLow = Token.toLower();
	if(in_array(TLow, events) && ($(i+space()) == '"' || $(i+space()) == '\'')) {
		i += space(true);
		char end = $(i++);
		newCode << end;
		auto js = JS();
		try {
			js(end);
		} catch(HTMLCode) {
			htmlchars($(i++));
			return;
		}
	} else if(TLow == "style" && ($(i+space()) == '"' || $(i+space()) == '\'')) {
		i += space(true);
		char end = $(i++);
		newCode << end;
		auto css = CSS();
		try {
			css(end);
		} catch(HTMLCode) {
			htmlchars($(i++));
			return;
		};
	} else if(TLow == "src" || TLow == "href") {
		newCode << TAG( HTML_LINK );
	} else {
		newCode << TAG( HTML_VALUE );
	}

	i += space(true);

	if($(i) == '"' || $(i) == '\'') {
		char end = $(i++);
		htmlchars(end);
		while(($(i) != end || $(i-1) == '\\') && Size > i) {
			htmlchars($(i++));
		}
		newCode << $(i++);
	} else {
		while($(i) != ' ' && $(i) != '>' && $(i) != '/' && Size > i) {
			htmlchars($(i++));
		}
	}

	newCode << END_TAG;
}
