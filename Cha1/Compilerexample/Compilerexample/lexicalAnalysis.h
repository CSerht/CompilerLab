#pragma once
#include <stdbool.h>
/* 头文件
	1. 声明数据的结构（数据类型）
	2. 声明函数
*/

// 注意enum kind 和 token 只是【结构/数据类型】，没有申请什么变量，所以放头文件。
// token表
enum kind
{
	// 关键词（与char *keyword[] 顺序保持一致）
	MAIN,	// main
	INT,	// int
	IF,		// if
	ELSE,	// else
	FOR,	// for
	WHILE,	// while

	// 界符
	LPAREN, // (	left paren   
	RPAREN, // )	right paren  
	LBRANCE,// {	left brance
	RBRANCE,// }	right brance
	LSQBRA,	// [	left square bracket
	RSQBRA,	// ]	right square bracket

	// 关系运算符
	GT,		// >
	GE,		// >=
	LT,		// <
	LE,		// <=
	EQ,		// ==
	NEQ,	// !=
	NOT,	// !

	// 赋值运算符
	ASSIGN,	// = 

	// 算数运算符
	ADD,	// +
	SUB,	// -
	MUL,	// *
	DIV,	// /

	// 分隔符
	COMMA,		// ,
	SEMICOLON,	// ;

	// 标识符和整数
	IDENT,	// identifier   
	INTE,	// integer

	// NULL
	NULLP,	// null poniter \0

	// 非法单词
	ERROR = -1	// 词法分析阶段出错，程序有bug！
};

// word
typedef struct
{
	enum kind k;	// 类型
	char *word;		// 值    若无值，则需置 "" 空串（" " 这个不是空串，是空格字符串）
}token;

void roolback(); // 回滚
token getToken();	// 获取一个Token

char getChar();	// 获取字符流的下一个字符
bool isAlpha(char c);	// 判断是否是字母，如果[a-z] | [A-Z]，返回ture，否则返回false		
bool isDigit(char c);	// 判断是否为数字，如果[0-9]，返回true，否则返回false

char* concat(char *strings, char c);	// 将字符转换为字符串并于原来的字符串拼接，返回新字符串
char* concatStatic(char *strings, char c); // 与上同，但strings是静态数据区

// 弃用 void empty_blank_char(char *stream);	// 清空字符流缓冲区的 空格、\n、\t，它们都是ascii字符

// 后面改成完美哈希，先用顺序查找凑合一下
int isKeyword(char *ident);	// 判断当前获取的标识符是否为关键字，若是，返回索引值，否则返回-1

