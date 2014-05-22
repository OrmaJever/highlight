#ifndef _JS_H
#define _JS_H

#include "../data.h"

#define JS_STRING		"jsString"
#define JS_KEYWORD		"jsKeyword"
#define JS_NUMBER		"jsNumber"
#define JS_HEX_NUMBER	"jsHexNumber"
#define JS_COMMENT	"jsComment"
#define JS_REGEXP		"jsRegExp"
#define JS_VARIABLE		"jsVariable"
#define JS_FUNCT		"jsFunct"
#define JS_OBJECT		"jsObj"

namespace HL {

class JS : public Data
{
public:
	void operator()( const char end = 0 ) throw(HTMLCode);

	void parse_String()		throw(JSCode);
	void parse_Numbers() 	throw(JSCode);
	void parse_Word()		throw(JSCode);
	void parse_Comment()	throw(JSCode);
	void parse_RegExp()		throw(JSCode);

	void parse_Keyword	( const string & )	throw(JSCode);
	void parse_Variable	( const string & )	throw(JSCode);
};

}

#endif // _JS_H
