#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexicalAnalysis.h"
#include "syntacticAnalysis.h"

extern int cursor;		// �ַ�������������
extern char stream[255];// �ַ���������
// ��ע����Ա��ʽ���������ֻ��ʶ����β���Ȿ����Ǵ���ģ����򱨴���������Ϊ����ʵ�\0

int main() {
	/* ����һ�龲̬�����������ַ�ָ��ָ���������� */
	//char *abc = "";		// �ո��ַ���	0x20 0x00	�ո� \0
	//char *abc2 = " ";	// �մ�			0x00		\0

	//char *abc3 = '\0';  // ָ�븳ֵ����ָ��ָ��null�� \0 ���� null

	// �ַ�����ֵ����κ����޸ġ��ڴ濽��������һ���ֽ�һ���ֽڸģ�����һ�����顿
	// char buffer[20] = "abcd";
	/// *buffer = "aaaa"; error�� *buffer�ȼ���buffer[0] 
	// ��������֪��"aaaa"���ص���ɶ�����½���� buffer[0] = 0xe0��
	// char b[20] = "aaaa";
	// strcpy_s(buffer, strlen(b) + 1, b);

	//char a[20];
	//int size = sizeof(a) / sizeof(a[0]);
	//scanf_s("%s", a, size); // ��β��Ĭ�ϼ��� \0 ��

	// ע��temp_char����ָ�룬��ָ��ĵ�ַ�ռ䲻���޸ģ�
	// ָ���Ǹ������������޸ı������ݸ���ָ�������ǣ��������Լ��ڿ�ط�
	// �Ժ�������ζ�ȡtxt�ļ������η��뻺������
	/// char char_buffer[255] = "3 + 4 * 5";

// ����������Ҫ�����������
//"if(x >= 0 ) {\
//	int i = 20;\
//	char c = 1;\
//} else {\
//	for(int j = 0; j <= 20; j++)\
//	int a = 0, b = 1, c;\
//	c = a + b * 10 / 2;\
//}"; 
	// error������������գ�int i = 10; �� int i�Ͳ�������ո�
	// empty_blank_char(char_buffer);			// Ԥ����


	// ���ڴ�ӡ����Ŀ�ĵ�table
	char *printType[] = {
		"main","int","if","else","for","while", // 0-5
		"(",")","{","}","[","]",	// 6-11
		">",">=","<","<=","==","!=","!", // 12-18
		"=",	// 19
		"+","-","*","/",	// 20-23
		",",";",	// 24-25
		"ident", "inte",	// 26 27
		"nullp"	// 28 
	}; // error �� -1 ���ܴ�ӡ�ģ�

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