#ifndef _HTML_H
#define _HTML_H

#include "../data.h"

#define HTML_TAG		"htmlTag"
#define HTML_NAME		"htmlKey"
#define HTML_VALUE		"htmlValue"
#define HTML_LINK		"htmlLink"
#define HTML_COMMENT	"htmlComment"

namespace HL {

class HTML : public Data
{
public:
	void operator()( void );

private:
	void parse_comment( void ) throw(HTMLCode);
	void parse_tag( void ) throw(HTMLCode, CSSCode, JSCode);
	void parse_attr( void );
};

}

#endif // _HTML_H
