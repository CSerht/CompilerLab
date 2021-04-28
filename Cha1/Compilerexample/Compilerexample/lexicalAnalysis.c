/* ʵ��C�����Ӽ��ı����� */
// ����Ҫbug�����ؼ��ֺͱ�ʶ��֮�䣬�����ÿո�ֿ�����˲���ɾȥ�ⲿ�ֿո�
// ���� int i = 10; ���ܱ�� inti=10;�������Ļ� inti ���Ǹ���ʶ����
// ������ж��߼���������ģ�

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexicalAnalysis.h"

// �򵥵Ĵʷ������� 


// �������ֱ�ʶ���͹ؼ��ֵ�table��δ���ǹ�ϣ��
// �ؼ���˳����enum kind ����һ��
// ��ȫ�ֱ�����ֵ���ھ�̬�������������ǳ����������Ǳ���������ʱ��Ҫ��������
// e.g. int *a = (int *)malloc(4); �Ƿ�
// int j = 2; int i = j; ���߷Ƿ�����Ϊ�Ǳ�����ǰ��OK
char *keyword[] = { "main","int","if","else",
					"for","while","table_end" };

// error code
char *error_code[] = { 
						"illegal symbol",	// �Ƿ��ַ��������˲���ʶ����ַ�
						"integer error"		// ������������ 0200
					 };

int cursor = 0;					// ָ���ַ���������������
char stream[255] = "3 + 4 * 5 + 10 * 9";	// �ַ���������

extern token myToken;

// �ع�
void roolback() {
	/// if(c != '\0') // �����ַ����Ľ�β���ͻع� 
	/// ����ģ���ʱȥ�������i = 10�﷨�����������⣬Ϊɶ�ع����ˡ�
	cursor--;
}


/*
	Ŀǰ��˵������ֱ�ӻ�ȡһ��token����������ô�洢��
	ȡ���ں�����﷨����������Ҫ�γ�AST��
	�����������ܹ���ȡһ��token�Ϳ�����
*/
token getToken() {
	// ���ԭ����token�Ǳ�ʶ��or���֣���Ҫ��free(word)
	// �����հ׷���ݹ���ã�ע���ֹ���free���ڸ��ط��õݹ飬��ʵ�����ʡ�
	if (myToken.k == IDENT || myToken.k == INTE) 
	{
		free(myToken.word);
	}

	char c = getChar();

	token temp_token = { ERROR,error_code[0] }; // �ַ������Ա����������

	if (isAlpha(c) || c == '_') { // ������ĸ���»��ߣ�һ�ɵ��ɱ�ʶ�����������ٱȽ��Ƿ��ǹؼ���
		char *id = (char *)malloc(1);
		id[0] = '\0'; // ��ʼ��Ϊ�մ�

		// ��ȡ��ʶ��
		do
		{
			id = concat(id, c); 
			c = getChar();
		} while (isAlpha(c) || isDigit(c) || c == '_');
		
		roolback();

		// �ؼ��֡���ʶ�����ж�
		int key_index = isKeyword(id);
		if(key_index != -1){   // �ǹؼ���
			temp_token.k = key_index;
			temp_token.word = "keyword"; // ����ʶ��ؼ��֣�������ÿռ�
		} else {
			temp_token.k = IDENT;
			temp_token.word = id;
		}
		

		return temp_token;

	}
	else if (isDigit(c)) {	// ��������
		char *id = (char *)malloc(1);
		id[0] = '\0'; // ��ʼ��Ϊ�մ�

		// ����0��ͷ�����֣�ֻ���� 0(0)*��������0([1-9])*
		// C��������0��ͷ���ǰ˽����ˣ�����ֻ֧��ʮ��������
		if (c == '0') {
			do
			{
				id = concat(id, c);
				c = getChar();
			} while (c == '0');
			if (isDigit(c)) { // 0(0)*[1-9]
				// ��¼�������ͺʹ���ֵ
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


		

		// ��ȡ����
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
	//************ ��� *************//
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
	//************ �Ƚ������ *************//
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


	//************ ��ֵ+�Ƚ� ����� *************//
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
	
	//************ ��������� *************//
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

	//************ �ָ��� *************//
	case ',':
		temp_token.k = COMMA;
		temp_token.word = "";
		break;

	case ';':
		temp_token.k = SEMICOLON;
		temp_token.word = "";
		break;

	//************ �ָ��� *************//
	case ' ':	// ���ǿհ׷������ٻ�ȡһ�飡
	case '\t':
	case '\n':
		myToken.k = NULLP; // ��ֹ���free(myToken.word)
		temp_token = getToken();
		break;
	case '\0': // ascii \0 = null = 0x00
		/// exit(0);  
		// ���﷨������һ��ʶ��  ';'ȱʧ�Ļ���
		// �����ַ������壬\0ֻ�����ڽ�β����
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

/* ��ȡ�ַ�������������һ���ַ� */
char getChar() { // ���߶�Ӧ����ָ�봫�ݣ�����ֵ����
	// if(��곬���ַ���) Ҫ���� ������˵Ӧ���ڵ����������֮ǰ�ʹ����ˣ�
	char c = '\0';
	if (cursor < (int)strlen(stream) + 1) { // ����ƶ���'\0'�������˳�����
		c = stream[cursor];
		cursor++;
		//���� int i; *i++; ���ȼ������� ��Ҫ(*i)++
	}
	else { // ������ \0 ֮��ĳ���
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

/* ƴ���ַ�����strings�Ƕ�̬���� */
char* concat(char *strings, char c) {
	// �ַ�ת��Ϊ�ַ���
	char *c_to_string = (char *)malloc(2);
	c_to_string[0] = c;
	c_to_string[1] = '\0';

	// �ַ���ƴ��
	char *new_string = (char *)malloc(strlen(strings) + sizeof(char) + 1);
	strcpy_s(new_string, strlen(strings) + 1, strings);
	strcat_s(new_string, strlen(new_string) + strlen(c_to_string) + 1, c_to_string);

	free(c_to_string);
	free(strings);

	return new_string;
}

/* ƴ���ַ�����strings�Ǿ�̬������ */
char* concatStatic(char *strings, char c) {
	// �ַ�ת��Ϊ�ַ���
	char *c_to_string = (char *)malloc(2);
	c_to_string[0] = c;
	c_to_string[1] = '\0';

	// �ַ���ƴ��
	char *new_string = (char *)malloc(strlen(strings) + sizeof(char) + 1);
	strcpy_s(new_string, strlen(strings) + 1, strings);
	strcat_s(new_string, strlen(new_string) + strlen(c_to_string) + 1, c_to_string);

	free(c_to_string);

	return new_string;
}

/* 
 * Ԥ����׶Ρ�ͣ�á�
 *
 * ����ַ��������Ŀհ׷� 
*/
//void empty_blank_char(char *stream) {
//	// ��������������ɾ��ĳԪ�أ�ԭ��һ��
//	// ֱ����ԭ�����޸�
//
//	int step_size = 0;
//	int len = strlen(stream) + 1; // ��ֱ�ӷ���ѭ����ÿ�ζ����ã�̫����������� \0 �ƶ���ֵ��ı�
//	for (int i = 0; i < len; i++) {
//		if (stream[i] == ' ' || stream[i] == '\t' || stream[i] == '\n') {
//			step_size++;
//		} else {
//			stream[i - step_size] = stream[i];
//		}
//	}
//}

/* ����ʶ���ǹؼ��֣����عؼ������������򷵻�-1 */
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




