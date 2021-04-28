#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexicalAnalysis.h"
#include "syntacticAnalysis.h"

extern int cursor;		// 字符流缓冲区索引
extern char stream[255];// 字符流缓冲区
// 【注意测试表达式不能是数字或标识符结尾】这本身就是错误的，程序报错正常！因为会访问到\0

int main() {
	/* 创建一块静态数据区，让字符指针指向这块儿区域 */
	//char *abc = "";		// 空格字符串	0x20 0x00	空格 \0
	//char *abc2 = " ";	// 空串			0x00		\0

	//char *abc3 = '\0';  // 指针赋值，让指针指向null， \0 就是 null

	// 字符串赋值后，如何后期修改【内存拷贝！或者一个字节一个字节改，就像一般数组】
	// char buffer[20] = "abcd";
	/// *buffer = "aaaa"; error！ *buffer等价于buffer[0] 
	// 不过【不知道"aaaa"返回的是啥，导致结果是 buffer[0] = 0xe0】
	// char b[20] = "aaaa";
	// strcpy_s(buffer, strlen(b) + 1, b);

	//char a[20];
	//int size = sizeof(a) / sizeof(a[0]);
	//scanf_s("%s", a, size); // 结尾会默认加上 \0 的

	// 注意temp_char不是指针，它指向的地址空间不能修改！
	// 指针是个变量，可以修改变量内容更换指向，它不是，它就有自己内块地方
	// 以后就是依次读取txt文件，依次放入缓冲区了
	/// char char_buffer[255] = "3 + 4 * 5";

// 测试用例需要增加例外测试
//"if(x >= 0 ) {\
//	int i = 20;\
//	char c = 1;\
//} else {\
//	for(int j = 0; j <= 20; j++)\
//	int a = 0, b = 1, c;\
//	c = a + b * 10 / 2;\
//}"; 
	// error！不能无脑清空，int i = 10; 的 int i就不能清除空格！
	// empty_blank_char(char_buffer);			// 预处理


	// 基于打印测试目的的table
	char *printType[] = {
		"main","int","if","else","for","while", // 0-5
		"(",")","{","}","[","]",	// 6-11
		">",">=","<","<=","==","!=","!", // 12-18
		"=",	// 19
		"+","-","*","/",	// 20-23
		",",";",	// 24-25
		"ident", "inte",	// 26 27
		"nullp"	// 28 
	}; // error 是 -1 不能打印的！

	//token my_token = { ERROR, 0 };
	//while (stream[cursor] != '\0')
	//{
	//	my_token = getToken();
	//	if (my_token.k == -1) {
	//		printf("Compilation fails. The program has an error,%s\n", my_token.word);
	//		break;
	//	}

	//	printf("%s\t%s\n", printType[my_token.k], my_token.word);
	//}

	char stream_t[255] =
"main() {				\
	int i;				\
	int j = 20;			\
	if (i == 0){		\
		tt = 40;		\
	}					\
	t = 30 * (3 - 8);	\
}";
	strcpy_s(stream, strlen(stream_t) + 1, stream_t);

	phase_program();

	printf("success\n");
	
	/*
	 ** concat() test successful **

	char *strings = (char *)malloc(5);
	char *t = "abcd";
	strcpy_s(strings, strlen(t) + 1, t);
	char c = '9';
	strings = concat(strings, c);
	*/

	return 0;

}