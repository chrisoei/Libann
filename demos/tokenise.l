%{ 
#include "parse.hh"
#include "syms.h"
%}

%x CMNT
%%
[\t\ ] ; /* whitespace */
\n     			{ return '\n' ; } 
[-\+]?[0-9]+		{ yylval.i_t=atoi(yytext) ; return INTEGER ; }
[-\+]?[0-9]*(\.[0-9]+)?(e[-/+][0-9]+)? 	{
                          yylval.f_t=atof(yytext) ; return FLOAT ; }
#\ /name      	{ return '#' ;}
#\ /type      	{ return '#' ;}
#\ /rows      	{ return '#' ;}
#\ /columns    	{ return '#' ;}
name      	{ return NAME ;}
type      	{ return TYPE ;}
rows      	{ return ROWS ;}
columns    	{ return COLUMNS ;}

^#              { BEGIN CMNT ; }
<CMNT>\n        { BEGIN INITIAL ; }
<CMNT>.          ;

[a-zA-z_]+[0-9a-zA-Z_]*  {
                          put(yytext,yylval.i_t);  
			  return IDENTIFIER ; }
:       		{ return ':' ; }
\.      		{ printf("Dot\n"); return '.' ; }
.      			{ printf("Other\n") ; ECHO ; }
%%

int
yywrap()
{
  return 1;
}
