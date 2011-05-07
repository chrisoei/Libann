 /* 
    Copyright (C) 2003  John Darrington 

    $Id: parse.y,v 1.6 2003/05/14 05:50:16 john Exp $

    This program is free software; you can redistibute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
%{ 
  extern "C" {
    int yyerror(char *s);
   int yylex(...);
  }
  #include <stdio.h>
  #include <assert.h>
  #include <ann/ann.h>
  #include <ann/fm.h>
  #include <ann/mlp.h>
  extern "C" { 
  #include "syms.h" 
  }
  extern char *yytext ;
  extern ann::FeatureMap featureMap;
  std::string matrixName;
  std::set<ann::ExtInput> featureSet;
  ann::ExtInput feature(0);
  int rows=-1;
  int cols=-1;
  int eRow, eCol;
%}

  
%union { 
    int i_t ;
    float f_t ;
} ;

%token <i_t> INTEGER
%token <f_t> FLOAT
%token <i_t> IDENTIFIER

%type <f_t> Number

%token TYPE ROWS COLUMNS NAME
%%


OctaveFile:  Space MatrixList
 ;

MatrixList: Matrix Space | Matrix Space MatrixList
 ;

Space : | '\n' | Space '\n'
 ;

Matrix : MatrixHeader MatrixBody 
 { 
 featureMap[matrixName]=featureSet; 
 rows=cols=-1;
 }
 ;

MatrixBody:  RowList 
 ;

RowList : | Row | Row RowList 
 ;

Row : ElementList '\n'
 {
   // copy feature into feature set
   featureSet.insert(feature);

   // clear the set ready for the next row
   feature.clear();
   feature.resize(cols);
   eRow++ ; 
 } ;

ElementList:  Element | Element ElementList
 ;

Number: FLOAT | INTEGER
 { $$ = $1 ; } ;

Element:  Number
 { 

   // put this number into the feature
   feature[eCol]=$1;

   ++eCol ;
   if (eCol >= cols )  eCol = 0;
 };


MatrixHeader: NameSpec TypeSpec RowSpec ColSpec
 { 
    assert ( rows >= 0 ); 
    assert ( cols >= 0 ); 

    // Clear the feature set, ready for this new class of features
    featureSet.clear();
    
    feature.resize(cols);

    eRow=eCol=0;
 };

NameSpec: '#' NAME ':' IDENTIFIER '\n'
  { 
    matrixName=get($4);
  };

TypeSpec: '#' TYPE ':' IDENTIFIER '\n'
  ;

RowSpec: '#' ROWS ':' INTEGER '\n'
  { rows=$4 ; }
  ;

ColSpec: '#' COLUMNS ':' INTEGER '\n'
  { cols=$4 ;} 
  ;

%%

int
yyerror(char *s)
{
  fprintf(stderr,"%s\n",s);
  fprintf(stderr,"Last token was \"%s\"\n",yytext);
}


