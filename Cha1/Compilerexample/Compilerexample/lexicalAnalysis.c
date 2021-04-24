/* 实现C语言子集的编译器 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>  

// 简单的词法分析器 

// 关键字表
enum kind
{
	// 关键词（与char *keyword[] 顺序保持一致）
	IF,		// if

	// 运算符和界符
	LPAREN, // (	left paren   
	RPAREN, // )	right paren  
	LBRANCE,// {	left brance
	RBRANCE,// }	right brance

	GT,		// >
	GE,		// >=
	LT,		// <
	LE,		// <=
	EQ,		// ==
	ASSIGN,	// = 赋值语句


	// 标识符和整数
	IDENT,	// identifier   
	INT		// integer
};

// 用于区分标识符和关键字的table，未来是哈希表
// 关键词顺序与enum kind 保持一致
char *keyword[] = { "if","table_end" };

// word
typedef struct 
{
	enum kind k;	// 类型
	char *word;		// 值    若无值，则需置0
}token;



char getChar(char *stream, int *cursor);	// 获取字符流的下一个字符
token getToken(char *stream, int *cursor);	// 获取一个Token
bool isAlpha(char c);	// 判断是否是字母，如果[a-z] | [A-Z]，返回ture，否则返回false		
bool isDigit(char c);	// 判断是否为数字，如果[0-9]，返回true，否则返回false
char* concat(char *strings, char c);	// 将字符转换为字符串并于原来的字符串拼接，返回新字符串
void empty_blank_char(char *stream);	// 清空字符流缓冲区的 空格、\n、\t，它们都是ascii字符

// 后面改成完美哈希，先用顺序查找凑合一下
int isKeyword(char *ident);	// 判断当前获取的标识符是否为关键字，若是，返回索引值，否则返回-1

/*
	目前来说，可以直接获取一个token，至于它怎么存储，
	取决于后面的语法分析，最终要形成AST，
	所以现在先能够获取一个token就可以了
*/
token getToken(char *stream, int *cursor) {
	char c = getChar(stream, cursor);

	token temp_token = { 0,0 };

	if (isAlpha(c) || c == '_') { // 若是字母或下划线，一律当成标识符处理，后期再比较是否是关键字
		char *id = (char *)malloc(1);
		id[0] = '\0'; // 初始化为空串

		// 获取标识符
		do
		{
			id = concat(id, c); 
			c = getChar(stream, cursor);
		} while (isAlpha(c) || isDigit(c) || c == '_');
		
		// roolback
		(*cursor)--;
		// 关键字、标识符的判定
		int key_index = isKeyword(id);
		if(key_index != -1){   // 是关键字
			temp_token.k = key_index;
			temp_token.word = id;
		} else {
			temp_token.k = IDENT;
			temp_token.word = id;
		}
		
		return temp_token;

	}
	else if (isDigit(c)) {	// 若是数字
		char *id = (char *)malloc(1);
		id[0] = '\0'; // 初始化为空串

		// 获取整数
		do
		{
			id = concat(id, c);
			c = getChar(stream, cursor);
		} while (isDigit(c));

		(*cursor)--; // roolback

		temp_token.k = INT;
		temp_token.word = id;

		return temp_token;

	} else switch (c){
	case '(':
		temp_token.k = LPAREN;
		temp_token.word = "0";
		break;

	case ')':
		temp_token.k = RPAREN;
		temp_token.word = "0";
		break;

	case '>':
		temp_token.k = GT;
		temp_token.word = "0";
		break;

	
	default:
		break;
	}

	return temp_token;
}

/* 获取字符流缓冲区的下一个字符 */
char getChar(char *stream, int *cursor) { // 二者都应该是指针传递，而非值传递
	// if(光标超出字符串) 要处理 （按理说应该在调用这个函数之前就处理了）
	char c = '\0';
	if (*cursor < (int)strlen(stream)) { // 不能移动到'\0'处
		c = stream[*cursor];
		cursor[0]++;
		//*cursor++; 优先级有问题 需要(*cursor)++
	}
	
	return c;
}


bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

/* 拼接字符串 */
char* concat(char *strings, char c) {
	// 字符转换为字符串
	char *c_to_string = (char *)malloc(2);
	c_to_string[0] = c;
	c_to_string[1] = '\0';

	// 字符串拼接
	char *new_string = (char *)malloc(strlen(strings) + sizeof(char) + 1);
	strcpy_s(new_string, strlen(strings) + 1, strings);
	strcat_s(new_string, strlen(new_string) + strlen(c_to_string) + 1, c_to_string);

	free(c_to_string);
	free(strings);

	return new_string;
}


/* 
 * 预处理阶段
 *
 * 清空字符缓冲区的空白符 
*/
void empty_blank_char(char *stream) {
	// 本问题与数组中删除某元素，原理一样
	// 直接在原数组修改

	int step_size = 0;
	int len = strlen(stream) + 1; // 不直接放入循环，每次都调用，太慢，并且最后 \0 移动，值会改变
	for (int i = 0; i < len; i++) {
		if (stream[i] == ' ' || stream[i] == '\t' || stream[i] == '\n') {
			step_size++;
		} else {
			stream[i - step_size] = stream[i];
		}
	}
}

/* 若标识符是关键字，返回关键字索引，否则返回-1 */
int isKeyword(char *ident) {
	int cursor = 0;
	while (strcmp(keyword[cursor],"table_end"))
	{
		if (!strcmp(ident, keyword[cursor])) {
			return cursor;
		}
		cursor++;
	}

	return -1;
}

int main() {
	//char a[20];
	//int size = sizeof(a) / sizeof(a[0]);
	//scanf_s("%s", a, size); // 结尾会默认加上 \0 的

	// 注意temp_char不是指针，它指向的地址空间不能修改！
	// 指针是个变量，可以修改变量内容更换指向，它不是，它就有自己内块地方
	char char_buffer[255] = "if (_abc2 > 20)"; // 以后就是依次读取txt文件了
	empty_blank_char(char_buffer);			// 预处理

	int *cursor = (int *)malloc(sizeof(int));
	*cursor = 0;

	token my_token = { 0,0 };
	while (char_buffer[*cursor] != '\0')
	{
		my_token = getToken(char_buffer, cursor);
		printf("%d\t%s\n", my_token.k, my_token.word);
	}
	
	/* 
	 ** concat() test successful **
	 
	char *strings = (char *)malloc(5);
	char *t = "abcd";
	strcpy_s(strings, strlen(t) + 1, t);
	char c = '9';
	strings = concat(strings, c);
	*/

	free(cursor);
	return 0;
}


