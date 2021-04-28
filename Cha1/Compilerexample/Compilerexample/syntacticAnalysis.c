// �﷨��������token��Դ��ʷ�������
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexicalAnalysis.h"
#include "syntacticAnalysis.h"

// �﷨�������б�ʽ�������� --> [������] --> �����
// 1. дһ��֧�ֵ��﷨���᷵��success��
// 2. дһ����֧�ֵ��﷨���᷵��error

// �ؼ��㣺ʲôʱ���ȡ��һ��token��ʲôʱ�򲻶�ȡ��
// ��ǰ�׶ε������־������Ҫ��ǰ�׶ζ�ȡtoken��
// �����׶εı�־����ֱ�ӵ��������׶Σ���Ҫ��ʱ���ٶ�ȡtoken
// ������ˡ�������������ã������ϵ��õ�ʱ�򣬻ᵼ��token��ͻ��
// �е���䷵�ص���һ��token���е��ǵ�ǰtoken�����Ǻ�����ж���һ������

extern int cursor;
token myToken = { NULLP,0 };

// <����> ::= main() <����>
void phase_program() {
	myToken = getToken();
	if (myToken.k == MAIN) {
		myToken = getToken();
		if (myToken.k == LPAREN) {
			myToken = getToken();
			if (myToken.k == RPAREN) {
				myToken = getToken();
				phase_state_block();
				return;
			}
			else {
				printf("main function ')' is missing.\n");
				exit(2);
			}
		}
		else {
			printf("main function '(' is missing.\n");
			exit(2);
		}
	}
	else {
		printf("main function is missing.\n");
		exit(2);
	}
}

// <����> ::= '{' <��䴮> '}' ����ʱ��token��'}'
// �룺{
// ����}
void phase_state_block() {
	// myToken = getToken();
	if (myToken.k == LBRANCE) {
		myToken = getToken();
		phase_state_string(); // ��������䴮��ȷ����ǰ��';'����һ��token
		if (myToken.k == RBRANCE) {
			return;
		}
		else {
			printf("'}' is missing.\n");
			exit(2);
		}
	}
	else {
		printf("'{' is missing.\n");
		exit(2);
	}
}

// <��䴮> ::= <���>[;] {<���>[;]} ����֮��ָ����һ��token
// �룺��ǰtoken
// ����
void phase_state_string() {
	// myToken = getToken();

	// ʶ�� <���>;
	if (myToken.k == IDENT) {
		phase_statement(); // �������أ�˵��<���>��ȷ���һ�ȡ���������token
		if (myToken.k != SEMICOLON) {
			printf("';' is missing.\n");
			exit(2);
		}
	}
	else if (myToken.k == IF || myToken.k == WHILE) {
		phase_statement();
	}
	else {
		printf("statement is missing.\n");
		exit(2);
	}

	

	// ʶ�� {<���>;}
	myToken = getToken();
	// ������ʱ���ܽ�� int = 20; ������
	while (myToken.k == IDENT || myToken.k == IF || myToken.k == WHILE)
	{
		if (myToken.k == IDENT) {
			phase_statement(); // next token ';'
			if (myToken.k != SEMICOLON) {
				printf("';' is missing.\n");
				exit(2);
			}
		}
		else if (myToken.k == IF || myToken.k == WHILE) {
			phase_statement(); // current token '}'
		}
		
		myToken = getToken(); // get next statement string
	}

	return;
	/// ��������ִ�н�����˵��״̬�ǣ�<���>{;<���>}
	/// ���ҵ�ǰָ����ַ��� ����״̬����һ���ַ�
	/// �����ʶ�� { } Ϊ0 �������/ ���ʶ�����һ���ֺš�
	/// ��ǰ����£�������û��<���>�������ȡ��';'������Ǹ��ַ�
	/// roolback(); �ع����У���Ϊ���пհ׷������ǻع�����һ��token
	
}

// <���> ::= <��ֵ���> | <�������> | <ѭ�����>
// <��ֵ���>  �룺��ǰtoken��������һ��token
// <������ѭ��> �룺��ǰtoken������current token '}'
void phase_statement() {	
	// myToken = getToken();
	if (myToken.k == IDENT) {
		phase_assign();
	}
	else if (myToken.k == IF) {
		phase_if(); // current token '}'
	}
	else if (myToken.k == WHILE) {
		phase_while();
	}
	//else {
	//	printf("it's not a statement, its format is wrong.\n");
	//	exit(2);
	//}

	return;
}


// <��ֵ���> ::= ID = <���ʽ> 
// int i = 10; ��ʱ��֧�֣��������δ����ı�ʶ���ĸ�ֵ�����﷨������������
// �룺��һ��token
// ������һ��token
void phase_assign() {

	myToken = getToken();
	if (myToken.k == ASSIGN) {
		phase_express();
	} else {
		printf("assignment statement error,'=' is missing.\n");
		exit(2);
	}
}


// ִ��֮ǰ��ָ��<���ʽ>����һ��token
// ����֮��ָ��<���ʽ>����һ��token
void phase_express() {
	// �ڸø��ڵ��´���һ����֧
	phase_term();	// �����أ�˵����ǰtoken�ǡ�+����-��
					// ���ܷ��ص���ʲô����Ϊ��ʱ�Ƿ��ս���������ж϶Դ�
					// ���ս����Ҫ�¸ҵ�����ȥ����·�ϲ�֪���Դ��
					// ֻ�е���ĳ���ս�������ܸ����߼��ж϶Դ�
	
	// token myToken = getToken();
	
	// { +<��> | -<��> } 
	while (myToken.k == ADD || myToken.k == SUB) // �൱�ڸ��ڵ�
	{
		phase_term(); // �ڸø��ڵ��´���һ����֧
		// myToken = getToken();
	}

	return;
}

void phase_term() {
	// �ڸø��ڵ��´���һ����֧
	phase_factor(); // �����ﷵ������˵���Ǳ�ʶ��������

	myToken = getToken(); // ��ȡ��һ���ַ���+��-��*��/��

	//  { *<����> | /<����> } 
	while (myToken.k == MUL || myToken.k == DIV) // �൱�ڸ��ڵ�
	{
		phase_factor();// �ڸø��ڵ��´���һ����֧
		myToken = getToken();
	}

	return; // �����ﷵ�أ�˵����ȡ���ַ��ǡ�+����-��
}

// Ҷ�ӽ��
void phase_factor() {
	myToken = getToken();

	// IDENT | INTE
	if (myToken.k == IDENT || myToken.k == INTE) 
		return;
	else if(myToken.k == LPAREN) // (
	{
		// myToken = getToken();
		phase_express(); // ���ǣ�'( ��ȷ���ʽ'
		if (myToken.k == RPAREN) // )
			return;
		else{
			printf("')' is missing.\n");
			exit(2);
		}
	}
	else {
		printf("expression error.\n");
		exit(2); // �˳���2�������﷨�����׶εĴ���
	}
}

// <�������> ::= if(<����>) <����> [else <����>]
// �룺previouse token 'if'
// ����current token '}'
void phase_if() {
	myToken = getToken();
	if (myToken.k == LPAREN) { // if (
		phase_condition(); // next token,expect:')'

		if (myToken.k == RPAREN) { // if ( <����> )
			myToken = getToken();
			phase_state_block(); // expect '}'
		}
		else {
			printf("if condition ')' is missing.\n");
			exit(2);
		}
	}
	else {
		printf("if condition '(' is missing.\n");
		exit(2);
	}

	// ʵ��[else <����>]����������else��token���䣺'}'
	// ͵͵������һ��token�ǲ���else��������ǣ��ͻ�ԭ�������� else <����>
	// ע���ַ���ָ��Ŀ�������ָֹ������
	// myToken: k = RBRANCE word = ""
	token t_token;
	t_token.k = myToken.k;
	t_token.word = "";
	int index = cursor;

	myToken = getToken();
	if (myToken.k == ELSE) { // if () { } else
		myToken = getToken(); 
		phase_state_block(); // expect '}'
	}
	else { // roolback
		cursor = index;
		myToken.k = t_token.k;
		myToken.word = "";
	}

	return;
}

// <ѭ�����> ::= while(<����>) <����>
// �룺previouse token 'while'
// ����current token '}'
void phase_while() {
	myToken = getToken();

	if (myToken.k == LPAREN) {
		// myToken = getToken();
		phase_condition(); // next token,expect:')'

		if (myToken.k == RPAREN) { // (
			myToken = getToken();
			phase_state_block(); // expect '}'
			return;
		}
		else {
			printf("while condition ')' is missing.\n");
			exit(2);
		}
	}
	else {
		printf("while condition '(' is missing.\n");
		exit(2);
	}
}

// <����> ::= <���ʽ> <��ϵ�����> <���ʽ>
// �룺ǰһ��token
// ������һ��token
void phase_condition() {
	phase_express();
	phase_rela_op();
	phase_express();
}

// <��ϵ�����> ::= < | <= | > | >= | == | !=
// entry: current token
// exit: current token
void phase_rela_op() {
	if (
			myToken.k == LT || myToken.k == LE ||
			myToken.k == GT || myToken.k == GE ||
			myToken.k == EQ || myToken.k == NEQ	
		)
	{
		return;
	}
	else {
		printf("relational operator error.\n");
		exit(2);
	}

}