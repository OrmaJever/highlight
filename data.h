#ifndef _DATA_H
#define _DATA_H

#include <string>
#include <vector>

#ifdef WIN32
#	define __STDIO__
#endif

#define $(x) Code[x]
#define $_(str) compare(str)
#define CALL(n) getToken(&Data::callback##n)
#define LINK(type, href, s) "<a href=\"http://www.php.net/manual/en/" type "." << href << ".php\" class=\"" s "\" target=_blank>"
#define END_LINK "</a>"
#define TAG(s) "<span class=\"" s "\">"
#define END_TAG "</span>"

namespace HL {

class Data
{

public:
	class string;

	static void init( void );
	static void free( void );

	virtual bool callback1( char, short ); // [a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*
	virtual bool callback2( char, short ); // [a-zA-Z_]*
	virtual bool callback3( char, short ); // [a-zA-Z0-9]*
	virtual bool callback4( char, short ); // [a-zA-Z-:_]*
	virtual bool callback5( char, short ); // [a-zA-Z-]*
	virtual bool callback6( char, short ); // [a-zA-Z]*
	virtual bool callback7( char, short ); // [a-zA-Z0-9_-]*

	static const string highlight( string, char *, int );
	string getToken( bool (Data::*)(char, short) );

protected:

	class PHPCode {};
	class CSSCode {};
	class JSCode {};
	class HTMLCode {};
	class SQLCode {};

	static string Code, newCode;
	static char *sep;
	static unsigned int i, Size, flag;

	// read spaces and write if need
	int space( bool write = false, signed char pos = 0 );
	static bool compare( const char *const );

	// php function
	bool in_array	( const string &, const char ** );
	void htmlchars	( char const );
};

class Data::string : public std::string
{
public:
	string() {};
	string (const std::string &v) : std::string (v) {}
	string &operator<<( const char );
	string &operator<<( const char * );
	string &operator<<( const string & );

	using std::string::operator=;

	string toLower() const;
	string toUpper() const;
	string replace( const char, const char ) const;
	string replace( const char *, const string ) const;
	std::vector< string > split( char );
	string urldecode();
};

}
#endif // _DATA_H
