/* ʵ��C�����Ӽ��ı����� */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>  

// �򵥵Ĵʷ������� 

// �ؼ��ֱ�
enum kind
{
	// �ؼ��ʣ���char *keyword[] ˳�򱣳�һ�£�
	IF,		// if

	// ������ͽ��
	LPAREN, // (	left paren   
	RPAREN, // )	right paren  
	LBRANCE,// {	left brance
	RBRANCE,// }	right brance

	GT,		// >
	GE,		// >=
	LT,		// <
	LE,		// <=
	EQ,		// ==
	ASSIGN,	// = ��ֵ���


	// ��ʶ��������
	IDENT,	// identifier   
	INT		// integer
};

// �������ֱ�ʶ���͹ؼ��ֵ�table��δ���ǹ�ϣ��
// �ؼ���˳����enum kind ����һ��
char *keyword[] = { "if","table_end" };

// word
typedef struct 
{
	enum kind k;	// ����
	char *word;		// ֵ    ����ֵ��������0
}token;



char getChar(char *stream, int *cursor);	// ��ȡ�ַ�������һ���ַ�
token getToken(char *stream, int *cursor);	// ��ȡһ��Token
bool isAlpha(char c);	// �ж��Ƿ�����ĸ�����[a-z] | [A-Z]������ture�����򷵻�false		
bool isDigit(char c);	// �ж��Ƿ�Ϊ���֣����[0-9]������true�����򷵻�false
char* concat(char *strings, char c);	// ���ַ�ת��Ϊ�ַ�������ԭ�����ַ���ƴ�ӣ��������ַ���
void empty_blank_char(char *stream);	// ����ַ����������� �ո�\n��\t�����Ƕ���ascii�ַ�

// ����ĳ�������ϣ������˳����Ҵպ�һ��
int isKeyword(char *ident);	// �жϵ�ǰ��ȡ�ı�ʶ���Ƿ�Ϊ�ؼ��֣����ǣ���������ֵ�����򷵻�-1

/*
	Ŀǰ��˵������ֱ�ӻ�ȡһ��token����������ô�洢��
	ȡ���ں�����﷨����������Ҫ�γ�AST��
	�����������ܹ���ȡһ��token�Ϳ�����
*/
token getToken(char *stream, int *cursor) {
	char c = getChar(stream, cursor);

	token temp_token = { 0,0 };

	if (isAlpha(c) || c == '_') { // ������ĸ���»��ߣ�һ�ɵ��ɱ�ʶ�����������ٱȽ��Ƿ��ǹؼ���
		char *id = (char *)malloc(1);
		id[0] = '\0'; // ��ʼ��Ϊ�մ�

		// ��ȡ��ʶ��
		do
		{
			id = concat(id, c); 
			c = getChar(stream, cursor);
		} while (isAlpha(c) || isDigit(c) || c == '_');
		
		// roolback
		(*cursor)--;
		// �ؼ��֡���ʶ�����ж�
		int key_index = isKeyword(id);
		if(key_index != -1){   // �ǹؼ���
			temp_token.k = key_index;
			temp_token.word = id;
		} else {
			temp_token.k = IDENT;
			temp_token.word = id;
		}
		
		return temp_token;

	}
	else if (isDigit(c)) {	// ��������
		char *id = (char *)malloc(1);
		id[0] = '\0'; // ��ʼ��Ϊ�մ�

		// ��ȡ����
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

/* ��ȡ�ַ�������������һ���ַ� */
char getChar(char *stream, int *cursor) { // ���߶�Ӧ����ָ�봫�ݣ�����ֵ����
	// if(��곬���ַ���) Ҫ���� ������˵Ӧ���ڵ����������֮ǰ�ʹ����ˣ�
	char c = '\0';
	if (*cursor < (int)strlen(stream)) { // �����ƶ���'\0'��
		c = stream[*cursor];
		cursor[0]++;
		//*cursor++; ���ȼ������� ��Ҫ(*cursor)++
	}
	
	return c;
}


bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

/* ƴ���ַ��� */
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


/* 
 * Ԥ����׶�
 *
 * ����ַ��������Ŀհ׷� 
*/
void empty_blank_char(char *stream) {
	// ��������������ɾ��ĳԪ�أ�ԭ��һ��
	// ֱ����ԭ�����޸�

	int step_size = 0;
	int len = strlen(stream) + 1; // ��ֱ�ӷ���ѭ����ÿ�ζ����ã�̫����������� \0 �ƶ���ֵ��ı�
	for (int i = 0; i < len; i++) {
		if (stream[i] == ' ' || stream[i] == '\t' || stream[i] == '\n') {
			step_size++;
		} else {
			stream[i - step_size] = stream[i];
		}
	}
}

/* ����ʶ���ǹؼ��֣����عؼ������������򷵻�-1 */
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
	//scanf_s("%s", a, size); // ��β��Ĭ�ϼ��� \0 ��

	// ע��temp_char����ָ�룬��ָ��ĵ�ַ�ռ䲻���޸ģ�
	// ָ���Ǹ������������޸ı������ݸ���ָ�������ǣ��������Լ��ڿ�ط�
	char char_buffer[255] = "if (_abc2 > 20)"; // �Ժ�������ζ�ȡtxt�ļ���
	empty_blank_char(char_buffer);			// Ԥ����

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


