#define _PHP_ // for words.h

#include "php.h"
#include "../words.h"

using namespace HL;

void PHP::operator()( void )
{
	parse_OpenTag();

	while(Size > i) try {
		i += space(true);
		if($(i) == '(') parse_Types();
		parse_Quote();
		if($(i) == '$') parse_Variable();
		if($(i) >= '0' && $(i) <= '9') parse_Numbers();
		if($(i) == '_' || ($(i) >= 'A' && $(i) <= 'Z') || ($(i) >= 'a' && $(i) <= 'z')) parse_Word();
		if($(i) == '"' || $(i) == '\'') parse_String();
		if($_("//") || $_("/*") || $(i) == '#') parse_Comment();
		if($_("->")) parse_Object();
		if($_("?>")) parse_CloseTag();
		if($(i) == '\\') parse_Namespace();
		if($_("<<<")) parse_HereDoc();
		parse_Operator();

		if($(i) == ';') sql = -1;
		htmlchars($(i++));

	} catch(PHPCode) {
		continue;
	}
}

void PHP::parse_OpenTag( void )
{
	newCode << TAG( PHP_TAG ) "&lt;?";
	if(($(i+2) == 'p' || $(i+2) == 'P') && ($(i+3) == 'h' || $(i+3) == 'H') && ($(i+4) == 'p' || $(i+4) == 'P')) {
		newCode << $(i+2) << $(i+3) << $(i+4);
		i += 5;
	} else if($(i+2) == '=') {
		newCode << "=";
		i += 3;
	} else {
		i += 2;
	}

	newCode << END_TAG;
}

void PHP::parse_CloseTag( void ) throw(HTMLCode)
{
	newCode << TAG( PHP_TAG ) "?&gt;" END_TAG;
	i += 2;
	throw HTMLCode();
}

void PHP::parse_Quote( void ) throw(PHPCode)
{
	if($(i) == '{') {
		newCode << (++bFigure%2 ? TAG( PHP_ODD_FIGURE_QUOTE ) : TAG( PHP_EVEN_FIGURE_QUOTE ));
		if(CLASS != -1) ++CLASS;
	} else if($(i) == '}') {
		newCode << (bFigure--%2 ? TAG( PHP_ODD_FIGURE_QUOTE ) : TAG( PHP_EVEN_FIGURE_QUOTE ));
		if(CLASS != -1) {
			if(CLASS != 1) --CLASS;
			else CLASS = -1;
		}
	} else if($(i) == '(') {
		++bRound;
		newCode << TAG( PHP_LUNE_QUOTE );
	} else if($(i) == ')') {
		--bRound;
		newCode << TAG( PHP_LUNE_QUOTE );
	} else if($(i) == '[') {
		++bSqr;
		newCode << TAG( PHP_SQR_QUOTE );
	} else if($(i) == ']') {
		--bSqr;
		newCode << TAG( PHP_SQR_QUOTE );
	} else return;

	newCode << $(i++) << END_TAG;
	throw PHPCode();
}

void PHP::parse_Variable( void ) throw(PHPCode)
{
	++i;
	auto Token = CALL(1);

	newCode << TAG( PHP_VARIABLE ) "$" << Token << END_TAG;
	auto sp = space();
	if($(i+sp) == ':' && $(i+1+sp) == ':') { // $varName::MethodName()
		i += space(true) + 2;
		newCode << TAG( PHP_OPERATOR ) "::" END_TAG;
		i += space(true);
		auto Token = CALL(1);

		newCode << ($(space()+i) == '(' ? TAG( PHP_METHOD ) : TAG( PHP_USER_CONST ));
		newCode << Token << END_TAG;
	}
	throw PHPCode();
}


void PHP::parse_Numbers( void ) throw(PHPCode)
{
	if($(i) >= '0' && $(i) <= '9') {
		newCode << TAG( PHP_NUMBER );
		while($(i) >= '0' && $(i) <= '9') {
			newCode << $(i++);
		}
	} else if($_("0x")) {
		i += 2;
		newCode << TAG( PHP_HEX_NUMBER ) "0x";
		while(($(i) >= '0' && $(i) <= '9') || ($(i) >= 'A' && $(i) <= 'F') || ($(i) >= 'a' && $(i) <= 'f')) {
			newCode << $(i++);
		}
	}
	newCode << END_TAG;
	throw PHPCode();
}

void PHP::parse_String( void ) throw(PHPCode)
{
	char prev = $(i+space(false, -1)-1);
	char quote = $(i++);

	// sql query?
	if(sql == -1 && prev != '[') {
		auto sp = space();
		i += sp;
		auto Token = CALL(2);
		if(!Token.empty()) {
			if($(i) == ' ' && in_array(Token.toUpper(), sqlKeyword)) sql = bRound;
			i -= Token.size() + sp;
		}
	}

	if(sql != -1 && sql <= bRound) {
		newCode << TAG( SQL );
	} else if(quote == '"') {
		newCode << TAG( PHP_D_STRING );
	} else {
		newCode << TAG( PHP_S_STRING );
	}

	htmlchars(quote);

	//Sql( quote, sql );

	while((quote != $(i) || ($(i-1) == '\\' && $(i-2) != '\\')) && Size > i ) {

		if(quote == '"' && $(i) == '$' && $(i-1) != '\\') {
			try { parse_Variable(); } catch(PHPCode) { continue; }
		}
		if(quote == '"' && $_("->")) {
			try { parse_Object(true); } catch(PHPCode) { continue; }
		}

		if($(i) == '\n') {
			if(sql == -1) {
				newCode << END_TAG;
				i += space(true);
				newCode << (quote == '"' ? TAG( PHP_D_STRING ) : TAG( PHP_S_STRING ));
			} else {
				newCode << END_TAG << sep << TAG( SQL );
				++i;
			}
			continue;
		}
		htmlchars($(i++));
	}

	htmlchars($(i++));
	newCode << END_TAG;
	throw PHPCode();
}

void PHP::parse_Comment( void ) throw(PHPCode)
{
	if(($_("//")) || $(i) == '#') {
		if($(i) == '#') {
			newCode << TAG( PHP_COMMENT ) "#";
			i += 1;
		} else {
			newCode << TAG( PHP_COMMENT ) "//";
			i += 2;
		}

		while($(i) != '\n' && Size > i) {
			htmlchars($(i++));
		}

		newCode << END_TAG;
		throw PHPCode();

	} else if($_("/*")) {
		newCode << TAG( PHP_COMMENT ) "/*";
		i += 2;

		while(!$_("*/") && Size > i) {
			if($(i) == '\n') {
				newCode << END_TAG;
				i += space(true);
				newCode << TAG( PHP_COMMENT );
			} else {
				htmlchars($(i++));
			}
		}
		if( Size > i ) {
			newCode << "*/" END_TAG;
			i += 2;
		}
		throw PHPCode();
	}

}

void PHP::parse_Operator( void ) throw(PHPCode)
{
#	define _(c) (cc!=(c))
#	define __(c) ($(i)==(c))
	char cc = $(i);
	if(_(':') && _(',') && _('!') && _('%') && _('&') && _('*') && _('+') && _('-') && _('.') && _('/') && _('<') && _('=') && _('>') && _('?') && _('@') && _('^') && _('|') && _('~')) {
		return;
	}
	newCode << TAG( PHP_OPERATOR );
	while(__(':') || __(',') || __('!') || __('%') || __('&') || __('*') || __('+') || __('-') || __('.') || __('/') || __('<') || __('=') || __('>') || __('?') || __('@') || __('^') || __('|') || __('~')) {
		htmlchars($(i++));
	}
	newCode << END_TAG;
	throw PHPCode();
#	undef _
#	undef __
}

void PHP::parse_HereDoc( void ) throw(PHPCode)
{
	newCode << TAG( PHP_HEREDOC ) "&lt;&lt;&lt;";
	bool nowdoc = false;
	i += 3;
	i += space(true);
	if($(i) == '\'') {
		newCode << '\'';
		++i;
		nowdoc = true;
	}
	auto bWord = CALL(1);
	newCode << bWord;
	if(nowdoc) {
		newCode << $(i++);
	}

	while( Size > i ) {

		if(!nowdoc && $(i) == '$' && $(i-1) != '\\') {
			try { parse_Variable(); } catch(PHPCode) { continue; }
		}
		if(!nowdoc && $_("->")) {
			try { parse_Object(true); } catch(PHPCode) { continue; }
		}

		if($(i) == '\n') {
			newCode << END_TAG;
			i += space(true);
			newCode << TAG( PHP_HEREDOC );

			auto Token = CALL(1);
			newCode << Token;

			if(bWord == Token) {
				newCode << END_TAG;
				throw PHPCode();
			}
			continue;
		}

		htmlchars($(i++));
	}
	throw PHPCode();
}

void PHP::parse_Types( void ) throw(PHPCode) // (int)$i
{
	newCode << TAG( PHP_LUNE_QUOTE ) "(" END_TAG;
	++i;
	i += space(true);

	auto Token = CALL(2);

	if($(i+space()) != ')' || !in_array(Token, types)) {
		i -= Token.size();
	} else {
		newCode << TAG( PHP_TYPE ) << Token << END_TAG;
	}

	throw PHPCode();
}

void PHP::parse_Object(bool str) throw(PHPCode)
{
	i += 2;
	newCode << TAG( PHP_OPERATOR ) "-&gt;" END_TAG;
	auto Token = CALL(1);

	newCode << ($(space()+i) == '(' && !str ? TAG( PHP_METHOD ) : TAG( PHP_OBJ_VAR ));
	newCode << Token << END_TAG;

	throw PHPCode();
}

void PHP::parse_Namespace( bool t ) throw(PHPCode)
{
	newCode << TAG( PHP_NAMESPACE );

	do {
		i += space(true);
		if($(i) == '\\') {
			newCode << '\\';
			++i;
			i += space(true);
		}
		auto Token = CALL(1);

		if(!t && $(i+space()) !=  '\\') {
			i -= Token.size();
			break;
		}
		newCode << Token;
	} while($(i+space()) ==  '\\');

	newCode << END_TAG;
	throw PHPCode();
}

void PHP::parse_Word( void ) throw(PHPCode)
{
	auto Token = CALL(1);
	auto t = Token.toLower();

	if(t == "array") { // array can be type - fn(array $arr) and funct name - array(1,2,3)
		if($(i+space()) != '(') {
			newCode << TAG( PHP_TYPE ) << Token << END_TAG;
		} else {
			newCode << LINK( "function", t.replace('_', '-'), PHP_STD_FUNCT ) << Token << END_LINK;
		}
		throw PHPCode();
	}

	parse_Keyword(Token);
	parse_stdFunct(Token);
	parse_stdConst(Token);
	parse_Classes(Token);
	parse_userFunct(Token);
	parse_userConst(Token);
}

void PHP::parse_Keyword(string &word) throw(PHPCode)
{
	auto w = word.toLower();
	if(!in_array(w, PHPKeyword))
		return;

	newCode << TAG( PHP_KEYWORD ) << word << END_TAG;

	if(w == "new" || w == "class" || w == "extends" || w == "interface" || w == "implements") {
		parse_Classes();
		if(w == "class") CLASS = 0;
	}

	if(w == "function")
		parse_userFunct();

	// namespace Foo, namespace\Foo\funct(), use \std\Foo as F
	if((w == "namespace" && $(i+space()) != '\\') || (w == "use" && $(i+space()) != '(') || (w == "as" && $(i+space()) != '$'))
		parse_Namespace(1);

	throw PHPCode();
}

void PHP::parse_stdFunct(string &word) throw(PHPCode)
{
	auto w = word.toLower();
	if($(i+space()) != '(' || w[0] == '_' || !in_array(w, stdFunct[w[0]-0x61]))
		return;

	newCode << LINK( "function", w.replace('_', '-'), PHP_STD_FUNCT ) << word << END_LINK;
	throw PHPCode();
}

void PHP::parse_stdConst(string &word) throw(PHPCode)
{
	unsigned char arrPos = 0;
	if(word[0] == '_') {
		arrPos = word[0]-0x45;
	} else if(word[0] >= 'A' && word[0] <= 'Z') {
		arrPos = word[0]-0x41;
	} else if(word[0] >= 'a' && word[0] <= 'z') {
		arrPos = word[0]-0x61;
	}

	if($(i+space()) == '(' || !in_array(word, stdConst[arrPos]))
		return;

	newCode << TAG( PHP_STD_CONST ) << word << END_TAG;
	throw PHPCode();
}

void PHP::parse_Classes( void ) throw(PHPCode)
{
	i += space(true);

	if($(i) == '\\') // new \className()
		try { parse_Namespace(); } catch(PHPCode) {}

	auto Token = CALL(1);

	if($(i+space()) == '\\') { // new namespace\className()
		if(Token == "namespace")
			newCode << TAG( PHP_KEYWORD ) << Token << END_TAG;
		else
			i -= Token.size();

		try { parse_Namespace(); } catch(PHPCode) {}
		i += space(true);
		Token = CALL(1);
	}

	auto t = Token.toLower();

	if(in_array(t, stdClass)) {
		newCode << LINK( "class", t, PHP_STD_CLASS ) << Token << END_LINK;
	} else {
		newCode << TAG( PHP_USER_CLASS ) << Token << END_TAG;
	}
}

void PHP::parse_Classes(string &word) throw(PHPCode)
{
	int sp = space();
	if($(i+sp) != '$' && ($(i+sp) != ':' || $(i+sp+1) != ':')) // fn(className $obj), className::method()
		return;

	auto w = word.toLower();
	if(in_array(w, stdClass)) {
		newCode << LINK( "class", w, PHP_STD_CLASS ) << word << END_LINK;
	} else {
		newCode << TAG( PHP_USER_CLASS ) << word << END_TAG;
	}

	if($(i+sp) != '$') { // ClassName::method()
		i += space(true) + 2;
		newCode << TAG( PHP_OPERATOR ) "::" END_TAG;
		i += space(true);
		auto Token = CALL(1);

		newCode << ($(space()+i) == '(' ? TAG( PHP_METHOD ) : TAG( PHP_USER_CONST ));
		newCode << Token << END_TAG;
	}
	throw PHPCode();
}

void PHP::parse_userFunct( void ) throw(PHPCode)
{
	i += space(true);
	if($(i) == '(') return; // lambda function

	auto Token = CALL(1);

	newCode << (CLASS != -1 ? TAG( PHP_METHOD ) : TAG( PHP_USER_FUNCT ));
	newCode << Token << END_TAG;
}

void PHP::parse_userFunct(string &word) throw(PHPCode)
{
	if($(space()+i) != '(')
		return;

	newCode << TAG( PHP_USER_FUNCT ) << word << END_TAG;
	throw PHPCode();
}

void PHP::parse_userConst(string &word) throw(PHPCode)
{
	if($(i+space()) == '\\') { // use A\B  or A\B\funct()
		i -= word.size();
		parse_Namespace();
	} else {
		newCode << TAG( PHP_USER_CONST ) << word << END_TAG;
	}
	throw PHPCode();
}
