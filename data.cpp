#include "data.h"
#include "main.h"

#include "module/php.h"
#include "module/css.h"
#include "module/js.h"
#include "module/html.h"

#include <string.h>

using namespace HL;

Data::string Data::Code, Data::newCode;
unsigned int Data::i, Data::flag, Data::Size;

const Data::string Data::highlight(string code, int f)
{
	Code = code;
	Size = Code.size();
	flag = f;
	i = 0;

	bool _php = false, _js = false, _css = false;

	auto html = HTML();
	auto css  = CSS();
	auto php  = PHP();
	auto js   = JS();

	while(Code.size() > i) try {
		if(_php) php();
		if(_css) css();
		if(_js)  js();

		html();

	} catch( PHPCode ) {
		_php = true;
	} catch( CSSCode ) {
		_css = true;
	} catch( JSCode ) {
		_js = true;
	} catch( HTMLCode ) {
		_php = _js = _css = false;
	};

	return newCode;
}

// find word in array words.h
bool Data::in_array(const string& word, const char **array)
{
	while(*array != '\0') {
		if(word == *array++)
			return true;
	}
	return false;
}

int Data::space(bool write, signed char pos)
{
	signed int p = 0;
	while($(i+pos+p) == ' ' || $(i+pos+p) == '\n' || $(i+pos+p) == '\t' || $(i+pos+p) == '\r') {
		if(write) htmlchars($(i+pos+p));
		pos ? --p : ++p;
	}
	return p;
}

bool Data::compare(const char *const str)
{
	for(int x=0; str[x] != '\0'; ++x) {
		if($(i+x) != str[x]) return false;
	}
	return true;
}

void Data::htmlchars(const char c)
{
	switch (c) {
		case ' ':
			newCode << "&nbsp;";
			break;
		case '\n':
			newCode << BR;
			break;
		case '<':
			newCode << "&lt;";
			break;
		case '>':
			newCode << "&gt;";
			break;
		case '&':
			newCode << "&amp;";
			break;
		case '"':
			newCode << "&quot;";
			break;
		case '\t':
			newCode << "&nbsp;&nbsp;&nbsp;";
			break;
		case '\r':
			return;
		default:
			newCode << c;
	}
}

void Data::free( void )
{
	Code.clear();
	newCode.clear();
}

Data::string Data::getToken(bool (Data::*callback)(char, short))
{
	unsigned long x = i;
	string Token;
	auto s = Code.size();
	for(char c = $(i); s > i; ++i, c = $(i)) {
		if((this->*callback)(c, i-x)) break;
		Token << c;
	}
	return Token;
}

bool Data::callback1(register char c, register short i)
{
	if(i == 0 && c != '_' && (c < 'A' || (c > 'Z' && c < 'a') || (c > 'z' && c < 0x7f)))
		return true;
	if(i > 0 && c != '_' && (c < '0' || (c > '9' && c < 'A') || (c > 'Z' && c < 'a') || (c > 'z' && c < 0x7f)))
		return true;

	return false;
}

bool Data::callback2(register char c, short)
{
	return (c < 'A' || (c > 'Z' && c < 'a') || c > 'z') && c != '_';
}

bool Data::callback3(register char c, short)
{
	return c < '0' || (c > '9' && c < 'A') || (c > 'Z' && c < 'a') || c > 'z';
}

bool Data::callback4(register char c, short)
{
	return (c < 'A' || (c > 'Z' && c < 'a') || c > 'z') && c != '_' && c != '-' && c != ':';
}

bool Data::callback5(register char c, short)
{
	return (c < 'A' || (c > 'Z' && c < 'a') || c > 'z') && c != '-';
}

bool Data::callback6(register char c, short)
{
	return c < 'A' || (c > 'Z' && c < 'a') || c > 'z';
}

// Data::string class
Data::string Data::string::toLower( void ) const
{
	string s;

	for(auto c : *this) {
		s << (c >= 'A' && c <= 'Z' ? c + 0x20 : c);
	}
	return s;
}

Data::string Data::string::toUpper( void ) const
{
	string s;

	for(auto c : *this) {
		s << (c >= 'a' && c <= 'z' ? c - 0x20 : c);
	}
	return s;
}

Data::string Data::string::replace( const char from, const char to ) const
{
	string s;

	for(auto c : *this) {
		s << (c == from ? to : c);
	}

	return s;
}

Data::string Data::string::replace( const char *from, const string to ) const
{
	if(find(from, 0) == -1) return *this;
	string s;

	size_t last = 0, found, len = 0;
	while(from[len] != '\0') ++len;

	while( (found = find(from, last)) != -1 ) {
		s += substr(last, found-last);
		s += to;
		last = found + len;
	}
	s += substr(last, size());
	return s;
}

std::vector< Data::string > Data::string::split( char c )
{
	std::vector< Data::string > v;
	size_t found, last = 0;
	while( (found = find(c, last)) != -1 ) {
		v.push_back(substr(last, found-last));
		last = found+1;
	}

	if(last < size()) {
		v.push_back(substr(last, size()));
	}

	return v;
}

Data::string Data::string::urldecode()
{
	char a, b;
	string s;
	for(size_t i=0; i<size(); ++i) {
		if(at(i) == '+') {
			s += ' ';
		} else if(at(i) == '%' && (a = at(i+1)) && (b = at(i+2)) && isxdigit(a) && isxdigit(b)) {

			if (a >= 'a') a -= 32;
			if (a >= 'A') a -= 55;
			else a -= 48;

			if (b >= 'a') b -= 32;
			if (b >= 'A') b -= 55;
			else b -= 48;

			s += 16*a+b;
			i += 2;
		} else {
			s += at(i);
		}
	}
	return s;
}

Data::string &Data::string::operator<<( const char c )
{
	*this += c;
	return *this;
}

Data::string &Data::string::operator<<( const char *c )
{
	append( c );
	return *this;
}

Data::string &Data::string::operator<<( const string &c )
{
	append( c );
	return *this;
}

