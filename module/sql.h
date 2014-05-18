#ifndef _SQL_H
#define _SQL_H

#include "../data.h"

#define SQL_KEYWORD		"sqlKeyword"

namespace HL {

class SQL : public Data
{
public:
	SQL( const char, short );
	~SQL( void ) {};

private:
	const char eos;
	unsigned short sql;

	void keyword( void );
	void string ( void );
	//void php_var( void );
};
}
#endif // _SQL_H
