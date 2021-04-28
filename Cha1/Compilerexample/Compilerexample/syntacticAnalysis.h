#pragma once
#include <stdbool.h>

void phase_program();		// <程序> ::= main() <语句块>
void phase_state_block();	// <语句块> ::= '{' <语句串> '}'
void phase_state_string();	// <语句串> ::= <语句>[;] {<语句>[;]} 
							// 赋值语句需要';'，条件和循环不需要';'
							// NOTE: 目前，开头不允许";;;;"(id miss) 结尾也不允许（} miss）
							// 不能识别 int i = 20; 只能 i = 20;

void phase_statement();		// <语句> ::= <赋值语句> | <条件语句> | <循环语句>


void phase_assign();	// <赋值语句> ::= ID = <表达式> 
void phase_express();	// <表达式> ::= <项> { +<项> | -<项> }
void phase_term();		// <项> ::= <因子> { *<因子> | /<因子> }
void phase_factor();	// <因子> ::= IDENT | INTE | (<表达式>)


void phase_if();		// <条件语句> ::= if(<条件>) <语句块> [else <语句块>] 
						// 暂不考虑省略 '{' '}'的情况
void phase_while();		// <循环语句> ::= while(<条件>) <语句块>
void phase_condition();	// <条件> ::= <表达式> <关系运算符> <表达式>
void phase_rela_op();	// <关系运算符> ::= < | <= | > | >= | == | !=