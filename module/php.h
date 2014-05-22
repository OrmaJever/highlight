#ifndef _HIGHLIGHT_H
#define _HIGHLIGHT_H

#include "../data.h"

#define PHP_TAG          	   "phpTag"
#define PHP_ODD_FIGURE_QUOTE   "phpOddFigureQuote"
#define PHP_EVEN_FIGURE_QUOTE  "phpEvenFigureQuote"
#define PHP_LUNE_QUOTE         "phpRoundQuote"
#define PHP_SQR_QUOTE          "phpSqrQuote"
#define PHP_VARIABLE           "phpVariable"
#define PHP_HEX_NUMBER         "phpHexNumber"
#define PHP_NUMBER             "phpNumber"
#define PHP_D_STRING           "phpDString"
#define PHP_S_STRING           "phpSString"
#define PHP_COMMENT			   "phpComment"
#define PHP_OPERATOR           "phpOperator"
#define PHP_HEREDOC            "phpHeredoc"
#define PHP_TYPE               "phpType"
#define PHP_NAMESPACE          "phpNamespace"
#define PHP_METHOD             "phpMethod"
#define PHP_OBJ_VAR            "phpObjVar"
#define PHP_KEYWORD            "phpKeyword"
#define PHP_STD_FUNCT          "phpStdFunct"
#define PHP_STD_CONST          "phpStdConst"
#define PHP_STD_CLASS          "phpStdClass"
#define PHP_USER_CLASS         "phpUserClass"
#define PHP_USER_FUNCT         "phpUserFunct"
#define PHP_USER_CONST         "phpUserConst"
#define SQL                	 "sql"

namespace HL {

class PHP : public Data
{
public:
	PHP( void ) : sql(-1), CLASS(-1), bFigure(0), bRound(0), bSqr(0) {};
	virtual void operator()( void );

private:

	signed short CLASS, bFigure, bRound, bSqr, sql;

	void parse_OpenTag  ( void );
	void parse_CloseTag ( void ) throw(HTMLCode);
	void parse_Quote    ( void ) throw(PHPCode);
	void parse_Variable ( void ) throw(PHPCode);
	void parse_Numbers  ( void ) throw(PHPCode);
	void parse_String   ( void ) throw(PHPCode);
	void parse_Comment  ( void ) throw(PHPCode);
	void parse_Operator ( void ) throw(PHPCode);
	void parse_HereDoc  ( void ) throw(PHPCode);
	void parse_Types    ( void ) throw(PHPCode);
	void parse_Word     ( void ) throw(PHPCode);
	void parse_Namespace( bool t = false )   throw(PHPCode);
	void parse_Object   ( bool str = false ) throw(PHPCode);

	void parse_Keyword  ( string & ) throw(PHPCode);
	void parse_stdFunct ( string & ) throw(PHPCode);
	void parse_stdConst ( string & ) throw(PHPCode);
	void parse_Classes  ( string & ) throw(PHPCode);
	void parse_Classes  ( void )	 throw(PHPCode);
	void parse_userFunct( void  )	 throw(PHPCode);
	void parse_userFunct( string & ) throw(PHPCode);
	void parse_userConst( string & ) throw(PHPCode);
};

}
#endif // _HIGHLIGHT_H
