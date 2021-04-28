#pragma once
#include <stdbool.h>
/* ͷ�ļ�
	1. �������ݵĽṹ���������ͣ�
	2. ��������
*/

// ע��enum kind �� token ֻ�ǡ��ṹ/�������͡���û������ʲô���������Է�ͷ�ļ���
// token��
enum kind
{
	// �ؼ��ʣ���char *keyword[] ˳�򱣳�һ�£�
	MAIN,	// main
	INT,	// int
	IF,		// if
	ELSE,	// else
	FOR,	// for
	WHILE,	// while

	// ���
	LPAREN, // (	left paren   
	RPAREN, // )	right paren  
	LBRANCE,// {	left brance
	RBRANCE,// }	right brance
	LSQBRA,	// [	left square bracket
	RSQBRA,	// ]	right square bracket

	// ��ϵ�����
	GT,		// >
	GE,		// >=
	LT,		// <
	LE,		// <=
	EQ,		// ==
	NEQ,	// !=
	NOT,	// !

	// ��ֵ�����
	ASSIGN,	// = 

	// ���������
	ADD,	// +
	SUB,	// -
	MUL,	// *
	DIV,	// /

	// �ָ���
	COMMA,		// ,
	SEMICOLON,	// ;

	// ��ʶ��������
	IDENT,	// identifier   
	INTE,	// integer

	// NULL
	NULLP,	// null poniter \0

	// �Ƿ�����
	ERROR = -1	// �ʷ������׶γ���������bug��
};

// word
typedef struct
{
	enum kind k;	// ����
	char *word;		// ֵ    ����ֵ�������� "" �մ���" " ������ǿմ����ǿո��ַ�����
}token;

void roolback(); // �ع�
token getToken();	// ��ȡһ��Token

char getChar();	// ��ȡ�ַ�������һ���ַ�
bool isAlpha(char c);	// �ж��Ƿ�����ĸ�����[a-z] | [A-Z]������ture�����򷵻�false		
bool isDigit(char c);	// �ж��Ƿ�Ϊ���֣����[0-9]������true�����򷵻�false

char* concat(char *strings, char c);	// ���ַ�ת��Ϊ�ַ�������ԭ�����ַ���ƴ�ӣ��������ַ���
char* concatStatic(char *strings, char c); // ����ͬ����strings�Ǿ�̬������

// ���� void empty_blank_char(char *stream);	// ����ַ����������� �ո�\n��\t�����Ƕ���ascii�ַ�

// ����ĳ�������ϣ������˳����Ҵպ�һ��
int isKeyword(char *ident);	// �жϵ�ǰ��ȡ�ı�ʶ���Ƿ�Ϊ�ؼ��֣����ǣ���������ֵ�����򷵻�-1

