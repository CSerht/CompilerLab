// 语法分析器，token来源与词法分析器
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexicalAnalysis.h"
#include "syntacticAnalysis.h"

// 语法分析，判别方式：（输入 --> [分析器] --> 输出）
// 1. 写一个支持的语法，会返回success；
// 2. 写一个不支持的语法，会返回error

// 关键点：什么时候读取下一个token！什么时候不读取？
// 当前阶段的特殊标志，是需要当前阶段读取token的
// 其他阶段的标志，就直接调用其他阶段，需要的时候再读取token
// 不仅如此……不合理的设置，当复合调用的时候，会导致token冲突，
// 有的语句返回的下一个token，有的是当前token，但是后面的判断又一样……

extern int cursor;
token myToken = { NULLP,0 };

// <程序> ::= main() <语句块>
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

// <语句块> ::= '{' <语句串> '}' 返回时，token是'}'
// 入：{
// 出：}
void phase_state_block() {
	// myToken = getToken();
	if (myToken.k == LBRANCE) {
		myToken = getToken();
		phase_state_string(); // 返回则语句串正确，当前是';'的下一个token
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

// <语句串> ::= <语句>[;] {<语句>[;]} 返回之后，指向下一个token
// 入：当前token
// 出：
void phase_state_string() {
	// myToken = getToken();

	// 识别 <语句>;
	if (myToken.k == IDENT) {
		phase_statement(); // 正常返回，说明<语句>正确，且获取了语句后面的token
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

	

	// 识别 {<语句>;}
	myToken = getToken();
	// 【】暂时不能解决 int = 20; 的问题
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
	/// 上面的语句执行结束，说明状态是：<语句>{;<语句>}
	/// 并且当前指向的字符是 上面状态的下一个字符
	/// 【如何识别 { } 为0 的情况？/ 如何识别最后一个分号】
	/// 当前情况下，不管有没有<语句>，都会获取到';'后面的那个字符
	/// roolback(); 回滚不行，因为会有空白符，除非回滚到上一个token
	
}

// <语句> ::= <赋值语句> | <条件语句> | <循环语句>
// <赋值语句>  入：当前token；出：下一个token
// <条件、循环> 入：当前token；出：current token '}'
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


// <赋值语句> ::= ID = <表达式> 
// int i = 10; 暂时不支持，另外对于未定义的标识符的赋值，是语法分析的任务吗？
// 入：上一个token
// 出：下一个token
void phase_assign() {

	myToken = getToken();
	if (myToken.k == ASSIGN) {
		phase_express();
	} else {
		printf("assignment statement error,'=' is missing.\n");
		exit(2);
	}
}


// 执行之前，指向<表达式>的上一个token
// 返回之后，指向<表达式>的下一个token
void phase_express() {
	// 在该根节点下创建一个分支
	phase_term();	// 若返回，说明当前token是“+”或“-”
					// 不管返回的是什么！因为此时是非终结符，不能判断对错
					// 非终结符就要勇敢地走下去，半路上不知道对错的
					// 只有到了某个终结符，才能根据逻辑判断对错
	
	// token myToken = getToken();
	
	// { +<项> | -<项> } 
	while (myToken.k == ADD || myToken.k == SUB) // 相当于根节点
	{
		phase_term(); // 在该根节点下创建一个分支
		// myToken = getToken();
	}

	return;
}

void phase_term() {
	// 在该根节点下创建一个分支
	phase_factor(); // 从这里返回来，说明是标识符或整数

	myToken = getToken(); // 读取下一个字符（+、-、*、/）

	//  { *<因子> | /<因子> } 
	while (myToken.k == MUL || myToken.k == DIV) // 相当于根节点
	{
		phase_factor();// 在该根节点下创建一个分支
		myToken = getToken();
	}

	return; // 从这里返回，说明读取的字符是“+”或“-”
}

// 叶子结点
void phase_factor() {
	myToken = getToken();

	// IDENT | INTE
	if (myToken.k == IDENT || myToken.k == INTE) 
		return;
	else if(myToken.k == LPAREN) // (
	{
		// myToken = getToken();
		phase_express(); // 若是：'( 正确表达式'
		if (myToken.k == RPAREN) // )
			return;
		else{
			printf("')' is missing.\n");
			exit(2);
		}
	}
	else {
		printf("expression error.\n");
		exit(2); // 退出码2，代表语法分析阶段的错误
	}
}

// <条件语句> ::= if(<条件>) <语句块> [else <语句块>]
// 入：previouse token 'if'
// 出：current token '}'
void phase_if() {
	myToken = getToken();
	if (myToken.k == LPAREN) { // if (
		phase_condition(); // next token,expect:')'

		if (myToken.k == RPAREN) { // if ( <条件> )
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

	// 实现[else <语句块>]，不管有无else，token不变：'}'
	// 偷偷看看下一个token是不是else，如果不是，就还原；否则处理 else <语句块>
	// 注意字符串指针的拷贝，防止指针悬挂
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

// <循环语句> ::= while(<条件>) <语句块>
// 入：previouse token 'while'
// 出：current token '}'
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

// <条件> ::= <表达式> <关系运算符> <表达式>
// 入：前一个token
// 出：后一个token
void phase_condition() {
	phase_express();
	phase_rela_op();
	phase_express();
}

// <关系运算符> ::= < | <= | > | >= | == | !=
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