#pragma once
#include <stdbool.h>

void phase_program();		// <����> ::= main() <����>
void phase_state_block();	// <����> ::= '{' <��䴮> '}'
void phase_state_string();	// <��䴮> ::= <���>[;] {<���>[;]} 
							// ��ֵ�����Ҫ';'��������ѭ������Ҫ';'
							// NOTE: Ŀǰ����ͷ������";;;;"(id miss) ��βҲ������} miss��
							// ����ʶ�� int i = 20; ֻ�� i = 20;

void phase_statement();		// <���> ::= <��ֵ���> | <�������> | <ѭ�����>


void phase_assign();	// <��ֵ���> ::= ID = <���ʽ> 
void phase_express();	// <���ʽ> ::= <��> { +<��> | -<��> }
void phase_term();		// <��> ::= <����> { *<����> | /<����> }
void phase_factor();	// <����> ::= IDENT | INTE | (<���ʽ>)


void phase_if();		// <�������> ::= if(<����>) <����> [else <����>] 
						// �ݲ�����ʡ�� '{' '}'�����
void phase_while();		// <ѭ�����> ::= while(<����>) <����>
void phase_condition();	// <����> ::= <���ʽ> <��ϵ�����> <���ʽ>
void phase_rela_op();	// <��ϵ�����> ::= < | <= | > | >= | == | !=