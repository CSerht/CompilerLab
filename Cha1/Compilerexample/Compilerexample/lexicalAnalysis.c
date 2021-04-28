/* 实现C语言子集的编译器 */
// 【重要bug！】关键字和标识符之间，必须用空格分开，因此不能删去这部分空格！
// 例如 int i = 10; 不能变成 inti=10;，这样的话 inti 就是个标识符了
// 这里的判断逻辑是有问题的！

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexicalAnalysis.h"

// 简单的词法分析器 


// 用于区分标识符和关键字的table，未来是哈希表
// 关键词顺序与enum kind 保持一致
// 【全局变量的值放在静态数据区，必须是常量，不能是变量！编译时就要定下来】
// e.g. int *a = (int *)malloc(4); 非法
// int j = 2; int i = j; 后者非法！因为是变量；前者OK
char *keyword[] = { "main","int","if","else",
					"for","while","table_end" };

// error code
char *error_code[] = { 
						"illegal symbol",	// 非法字符，输入了不可识别的字符
						"integer error"		// 整数错误，例如 0200
					 };

int cursor = 0;					// 指向字符流缓冲区的索引
char stream[255] = "3 + 4 * 5 + 10 * 9";	// 字符流缓冲区

extern token myToken;

// 回滚
void roolback() {
	/// if(c != '\0') // 不是字符流的结尾，就回滚 
	/// 上面的，暂时去除，解决i = 10语法分析出错问题，为啥回滚忘了。
	cursor--;
}


/*
	目前来说，可以直接获取一个token，至于它怎么存储，
	取决于后面的语法分析，最终要形成AST，
	所以现在先能够获取一个token就可以了
*/
token getToken() {
	// 如果原来的token是标识符or数字，需要先free(word)
	// 遇到空白符会递归调用，注意防止多次free，内个地方用递归，其实不合适。
	if (myToken.k == IDENT || myToken.k == INTE) 
	{
		free(myToken.word);
	}

	char c = getChar();

	token temp_token = { ERROR,error_code[0] }; // 字符串可以保存错误类型

	if (isAlpha(c) || c == '_') { // 若是字母或下划线，一律当成标识符处理，后期再比较是否是关键字
		char *id = (char *)malloc(1);
		id[0] = '\0'; // 初始化为空串

		// 获取标识符
		do
		{
			id = concat(id, c); 
			c = getChar();
		} while (isAlpha(c) || isDigit(c) || c == '_');
		
		roolback();

		// 关键字、标识符的判定
		int key_index = isKeyword(id);
		if(key_index != -1){   // 是关键字
			temp_token.k = key_index;
			temp_token.word = "keyword"; // 便于识别关键字，充分利用空间
		} else {
			temp_token.k = IDENT;
			temp_token.word = id;
		}
		

		return temp_token;

	}
	else if (isDigit(c)) {	// 若是数字
		char *id = (char *)malloc(1);
		id[0] = '\0'; // 初始化为空串

		// 对于0开头的数字，只能是 0(0)*，不能是0([1-9])*
		// C语言中以0开头的是八进制了，我们只支持十进制整数
		if (c == '0') {
			do
			{
				id = concat(id, c);
				c = getChar();
			} while (c == '0');
			if (isDigit(c)) { // 0(0)*[1-9]
				// 记录错误类型和错误值
				id = concat(id, c);
				id = concat(id, '.');
				id = concat(id, '.');
				id = concat(id, '.');
				temp_token.word = (char *)malloc(strlen(error_code[1]) + 1);
				strcpy_s(temp_token.word, strlen(error_code[1]) + 1, error_code[1]);
				temp_token.word = concat(temp_token.word, ':');
				strcat_s(temp_token.word, strlen(temp_token.word) + strlen(id) + 1, id);
				return temp_token;
			}
			else { // 0(0)* others
				temp_token.k = INTE;
				temp_token.word = (char *)malloc(2);
				temp_token.word[0] = '0';
				temp_token.word[1] = '\0';
				roolback();
				return temp_token;
			}
		}


		

		// 获取整数
		do
		{
			id = concat(id, c);
			c = getChar();
		} while (isDigit(c));

		roolback();

		temp_token.k = INTE;
		temp_token.word = id;

		return temp_token;

	} else switch (c){
	//************ 界符 *************//
	case '(':
		temp_token.k = LPAREN;
		temp_token.word = "";
		break;

	case ')':
		temp_token.k = RPAREN;
		temp_token.word = "";
		break;

	case '{':
		temp_token.k = LBRANCE;
		temp_token.word = "";
		break;

	case '}':
		temp_token.k = RBRANCE;
		temp_token.word = "";
		break;

	case '[':
		temp_token.k = LSQBRA;
		temp_token.word = "";
		break;

	case ']':
		temp_token.k = RSQBRA;
		temp_token.word = "";
		break;
	//************ 比较运算符 *************//
	case '>': {
		c = getChar();
		switch (c)
		{
		case '=':
			temp_token.k = GE;
			temp_token.word = "";
			break;

		default:
			temp_token.k = GT;
			temp_token.word = "";

			roolback();
			break;
		}
		break;
	}
		
	case '<': {
		c = getChar();
		switch (c)
		{
		case '=':
			temp_token.k = LE;
			temp_token.word = "";
			break;

		default:
			temp_token.k = LT;
			temp_token.word = "";

			roolback();
			break;
		}
		break;
	}
	
	case '!': {
		c = getChar();
		switch (c)
		{
		case '=':
			temp_token.k = NEQ;
			temp_token.word = "";
			break;

		default:
			temp_token.k = NOT;
			temp_token.word = "";

			roolback();
			break;
		}
		break;
	}


	//************ 赋值+比较 运算符 *************//
	case '=': {
		c = getChar();
		switch (c)
		{
		case '=':
			temp_token.k = EQ;
			temp_token.word = "";
			break;

		default:
			temp_token.k = ASSIGN;
			temp_token.word = "";

			roolback();
			break;
		}
		break;
	}
	
	//************ 算数运算符 *************//
	case '+':
		temp_token.k = ADD;
		temp_token.word = "";
		break;

	case '-':
		temp_token.k = SUB;
		temp_token.word = "";
		break;

	case '*':
		temp_token.k = MUL;
		temp_token.word = "";
		break;

	case '/':
		temp_token.k = DIV;
		temp_token.word = "";
		break;

	//************ 分隔符 *************//
	case ',':
		temp_token.k = COMMA;
		temp_token.word = "";
		break;

	case ';':
		temp_token.k = SEMICOLON;
		temp_token.word = "";
		break;

	//************ 分隔符 *************//
	case ' ':	// 若是空白符，就再获取一遍！
	case '\t':
	case '\n':
		myToken.k = NULLP; // 防止多次free(myToken.word)
		temp_token = getToken();
		break;
	case '\0': // ascii \0 = null = 0x00
		/// exit(0);  
		// 给语法分析器一个识别  ';'缺失的机会
		// 对于字符串缓冲，\0只可能在结尾出现
		temp_token.k = NULLP;
		temp_token.word = "";
		break;

	default:
		temp_token.word = concatStatic(temp_token.word, ':');
		temp_token.word = concatStatic(temp_token.word, c);
		break;
	}

	return temp_token;
}

/* 获取字符流缓冲区的下一个字符 */
char getChar() { // 二者都应该是指针传递，而非值传递
	// if(光标超出字符串) 要处理 （按理说应该在调用这个函数之前就处理了）
	char c = '\0';
	if (cursor < (int)strlen(stream) + 1) { // 最多移动到'\0'处，就退出程序
		c = stream[cursor];
		cursor++;
		//对于 int i; *i++; 优先级有问题 需要(*i)++
	}
	else { // 访问了 \0 之后的程序
		printf("Sorry,the stream index is out of range.\n");
		exit(1);
	}
	
	return c;
}


bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

/* 拼接字符串，strings是动态申请 */
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

/* 拼接字符串，strings是静态数据区 */
char* concatStatic(char *strings, char c) {
	// 字符转换为字符串
	char *c_to_string = (char *)malloc(2);
	c_to_string[0] = c;
	c_to_string[1] = '\0';

	// 字符串拼接
	char *new_string = (char *)malloc(strlen(strings) + sizeof(char) + 1);
	strcpy_s(new_string, strlen(strings) + 1, strings);
	strcat_s(new_string, strlen(new_string) + strlen(c_to_string) + 1, c_to_string);

	free(c_to_string);

	return new_string;
}

/* 
 * 预处理阶段【停用】
 *
 * 清空字符缓冲区的空白符 
*/
//void empty_blank_char(char *stream) {
//	// 本问题与数组中删除某元素，原理一样
//	// 直接在原数组修改
//
//	int step_size = 0;
//	int len = strlen(stream) + 1; // 不直接放入循环，每次都调用，太慢，并且最后 \0 移动，值会改变
//	for (int i = 0; i < len; i++) {
//		if (stream[i] == ' ' || stream[i] == '\t' || stream[i] == '\n') {
//			step_size++;
//		} else {
//			stream[i - step_size] = stream[i];
//		}
//	}
//}

/* 若标识符是关键字，返回关键字索引，否则返回-1 */
int isKeyword(char *ident) {
	int cursor_t = 0;
	while (strcmp(keyword[cursor_t],"table_end"))
	{
		if (!strcmp(ident, keyword[cursor_t])) {
			return cursor_t;
		}
		cursor_t++;
	}

	return -1;
}




