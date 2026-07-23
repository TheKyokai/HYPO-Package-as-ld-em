%option noyywrap noinput nounput

%{

#include <iostream>
using namespace std;

#include "assembler/assembler.hpp"
#include "assembler/parser.hpp"

yy::location loc; // Must be initialized manually before parsing (loc.initialize();)

#define YY_USER_INIT loc.initialize();


#define YY_DECL yy::parser::symbol_type yylex()
#define YY_USER_ACTION loc.columns(yyleng);

#define PRINT_TOKEN(token) \
    cout << "[TOKEN] " << token << " (" << loc.begin.line << ":" << loc.begin.column << ")\n"

%}

%%

    /* Asm directives */

\.global		{ return yy::parser::make_ASMD_GLOBAL(loc); }
\.extern		{ return yy::parser::make_ASMD_EXTERN(loc); }
\.section		{ return yy::parser::make_ASMD_SECTION(loc); }
\.word			{ return yy::parser::make_ASMD_WORD(loc); }
\.skip			{ return yy::parser::make_ASMD_SKIP(loc); }
\.end			{ return yy::parser::make_ASMD_END(loc); }
\.ascii			{ return yy::parser::make_ASMD_ASCII(loc); }
\.equ			{ return yy::parser::make_ASMD_EQU(loc); }


    /* Instructions */

halt			{ return yy::parser::make_INST_HALT(loc); }
int			    { return yy::parser::make_INST_INT(loc); }
iret			{ return yy::parser::make_INST_IRET(loc); }
call			{ return yy::parser::make_INST_CALL(loc); }
ret			    { return yy::parser::make_INST_RET(loc); }
jmp		    	{ return yy::parser::make_INST_JMP(loc); }
beq		    	{ return yy::parser::make_INST_BEQ(loc); }
bne		    	{ return yy::parser::make_INST_BNE(loc); }
bgt		    	{ return yy::parser::make_INST_BGT(loc); }
push			{ return yy::parser::make_INST_PUSH(loc); }
pop		    	{ return yy::parser::make_INST_POP(loc); }
xchg			{ return yy::parser::make_INST_XCHG(loc); }
add		    	{ return yy::parser::make_INST_ADD(loc); }
sub		    	{ return yy::parser::make_INST_SUB(loc); }
mul		    	{ return yy::parser::make_INST_MUL(loc); }
div		    	{ return yy::parser::make_INST_DIV(loc); }
not		    	{ return yy::parser::make_INST_NOT(loc); }
and		    	{ return yy::parser::make_INST_AND(loc); }
or		    	{ return yy::parser::make_INST_OR(loc); }
xor		    	{ return yy::parser::make_INST_XOR(loc); }
shl			    { return yy::parser::make_INST_SHL(loc); }
shr			    { return yy::parser::make_INST_SHR(loc); }
ld		    	{ return yy::parser::make_INST_LD(loc); }
st		    	{ return yy::parser::make_INST_ST(loc); }
csrrd			{ return yy::parser::make_INST_CSRRD(loc); }
csrwr			{ return yy::parser::make_INST_CSRWR(loc); }


    /* Registers */

\%r[0-9]+		{ 
                    int reg = atoi(yytext + 2);
                    if (reg < 0 || reg > 15)
                        throw runtime_error(string("Invalid register (out of range): ") + yytext);

                    return yy::parser::make_GPR(reg, loc); 
                }
\%sp		    {
                    return yy::parser::make_GPR(14, loc); 
                }
\%pc		    {
                    return yy::parser::make_GPR(15, loc); 
                }
\%status		{ return yy::parser::make_CSR(0, loc); }
\%handler		{ return yy::parser::make_CSR(1, loc); }
\%cause			{ return yy::parser::make_CSR(2, loc); }

    /* Misc */

:		    	{ return yy::parser::make_COLON(loc); }
,		    	{ return yy::parser::make_COMMA(loc); }
\$			    { return yy::parser::make_IMM_PREFIX(loc); }
\(		    	{ return yy::parser::make_L_PAREN(loc); }
\)		    	{ return yy::parser::make_R_PAREN(loc); }
\[		    	{ return yy::parser::make_L_BRACKET(loc); }
\]		    	{ return yy::parser::make_R_BRACKET(loc); }
\n		    	{ loc.lines(yyleng); loc.step(); return yy::parser::make_YYEOL(loc); }

\+		    	{ return yy::parser::make_PLUS(loc); }
\-			    { return yy::parser::make_MINUS(loc); }
\*		    	{ return yy::parser::make_MUL(loc); }
\/		    	{ return yy::parser::make_DIV(loc); }
	

[\.A-Za-z_][\.A-Za-z0-9_]*	{
                                return yy::parser::make_SYMBOL(string(yytext), loc);
                            }

(0[xX][0-9A-Fa-f]+|[0-9]+) 	{
                                return yy::parser::make_NUM_LITERAL(strtol(yytext, nullptr, 0), loc);
                            }

\"([^\\\"]|\\.)*\"	        {
                                string s(yytext);
                                return yy::parser::make_STR_LITERAL(s.substr(1, s.size() - 2), loc);
                            }

    /* Whitespaces */

[ \t\r]+		;

    /* Comment */

#[^\n]*			; 

<<EOF>>                     { return yy::parser::symbol_type(0, loc); } 

    /* Error handling */

. 			{
				throw runtime_error(string("Unexpected character: '") + yytext + "'");
			}

%%
