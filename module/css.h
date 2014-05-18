#ifndef _CSS_H
#define _CSS_H

#include "../data.h"

#define CSS_COMMENT		"cssComment"
#define CSS_TAG			"cssTag"
#define CSS_CLASS		"cssClass"
#define CSS_ID			"cssId"
#define CSS_KEY			"cssKey"
#define CSS_VALUE		"cssValue"
#define CSS_PSE			"cssPseudo"
#define CSS_SELNAME		"cssSelName"
#define CSS_SELVALUE	"cssSelValue"

namespace HL {

class CSS : public Data
{
public:
	CSS( void ) : quote(-1) {};
	void operator()( const char end = 0 );

private:
	void parse_tag();
	void parse_key();
	void parse_value();
	void parse_comment();
	void parse_selector();

	char quote;
};

}

#endif // _CSS_H
