#define _SQL_ // для words.h

#include "sql.h"
#include "../words.h"
#include "../main.h"

using namespace HL;

SQL::SQL( const char c, short sql ) : eos(c), sql(sql)
{
	while(($(i) != eos || ($(i-1) == 0x5c && $(i-2) != 0x5c)) && $(i) != '\0') try {
		i << space(true);
		//if(eos == 0x22) php_var();

		string();
		keyword();

		htmlchars($(i++));
	} catch( SQLCode ) {
		continue;
	}
	newCode << END_TAG << (eos==0x22 ? "&quot;" : "'");
	++i;
	sql = ($(i+space()) == '.');

	throw SQLCode();
}

void SQL::keyword( void )
{
	auto Token = CALL(2);
	if(Token.empty())
		throw SQLCode();
	auto uToken = Token.toUpper();
	if(in_array(uToken, Keywords)) {
		newCode << TAG( SQL_KEYWORD ) << uToken << END_TAG;
		throw SQLCode();
	} else {
		i -= Token.size();
	}
	throw SQLCode();
}
/*
void SQL::php_var( void )
{
	if($(i) == 0x24 && $(i-1) != 0x5c) {
		i += 1;
		char *Token = CALL(1);
		*this << TAG( VARIABLE ) << '$' << Token << END_TAG;
		(*this)[Token];

		throw SQLCode();
	}

	if($(i) == 0x2d && $(i+1) == 0x3e) {
		i += 2;
		*this << TAG( OPERATOR ) << "-&gt;" << END_TAG;
		char *Token = CALL(1);
		*this << TAG( OBJ_VAR ) << Token << END_TAG;
		(*this)[Token];

		throw SQLCode();
	}
}
*/
void SQL::string( void )
{
	if($(i) != 0x22 && $(i) != 0x27) return;
}
