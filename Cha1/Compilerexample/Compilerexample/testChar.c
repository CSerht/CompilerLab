#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ջ�ƻ������������ͼ�⣬��Ϊ����������ʱ������ջ
void testStackCorrupt() {
	char a[2];
	a[2] = 'a';
	printf("%c\n", a[2]);
}

// ���ƻ���ֻ��free��ʱ��ż�⵽�����һֱ��free���ߵ�ַ��ʧ�����ڴ�й©
char* testHeadCorruption(){
	char *c = (char *)malloc(3);
	c[3] = 'c';
	printf("%c\n", c[3]);
	return c;
}

int main22() {
	char *c11 = testHeadCorruption();
	testStackCorrupt();
	free(c11);

	// �ַ�ָ�뵽����ô��ֵ��������
	//char *w = "1234";
	//printf("%s\n", w);
	//// char *id; // ���ʾ����ַ�ָ�룡����ָ��һ���ַ���Ҳ����ָ��һ���ַ������� int *��̬��������
	//char c = 'a';

	//char *id = (char *)malloc(sizeof(char));

	//*id = c;

	//printf("%c\n", *id);

	//char *id2 = (char *)malloc(sizeof(char));
	//*id2 = c;
	//printf("%c\n", *id2);

	//char *id3 = (char *)malloc(sizeof(char) * 2);

	//strcpy_s(id3, 255, id);
	//printf("%s\n", id3);
	//strcat_s(id3, 255, id2);
	//printf("%s\n", id3);

	//char a[255] = "";
	//char *p;

	//a[1] = 'a';
	//a[2] = 'b';
	//a[0] = '_';
	//a[3] = '2';

	//p = a;	// ������ʵ���ǵ�ַ��������pָ���ָ�����˸ı䡣
	//		// ���ܷ��أ�������ջ���Ķ�����û�ˣ��鱾ʾ���Ǵ�ӡ�ġ���

	//printf("%s\n", p);
	
	//char x = 'a';
	//char *xx = "a";  ��\0 �� ASCII �� 0x00 ��

	//struct a {
	//	int i;
	//	char *c;
	//};


	// ��һ���ַ�
	//char *p1 = (char *)malloc(sizeof(char));
	//char c1 = 'a';
	//*p1 = c1; // p1  = "a"
	//printf("p1 = %c\n", p1);

	// �ַ���Ҫ��ӽ����� \0
	//char *p2 = (char *)malloc(sizeof(char) *2);
	//strcpy_s(p2, strlen(p1)+1, p1);

	//int *p;
	//p = (int *)malloc(sizeof(int));		// ok
	//char *ac;
	//ac = (char *)malloc(sizeof(char));	// ok

	printf("size of size_t = %d\n", sizeof(size_t)); // 4

	char cccc[3];
	cccc[3] = 1; // ok
	printf("%d\n", cccc[3]); // ok
	// but you can't  release the spase. it's an error.

	char *p1 = "abcd";

	printf("%c\n", p1[0]); // ���Զ�ȡ��������д�룬���ǳ���
	printf("%c\n", p1[3]); 
	// p1[0] = 'c'; �������ֹд�룡

	char p2[20];
	strcpy_s(p2, strlen(p1) + 1, p1); 
	printf("p2 = %s", p2);

	// ��p1ָ����ڴ棬����strlen(p1) + 1�ֽڴ�С����p2ָ����ڴ�
	// ���ң������������strlen(p1) + 1����ô���������������ֵ����δ֪(���)��
	// ����������p1�ڴ�����һ���������ڲ����ǷǷ�����
	// ���ǻ�����������p2��ʱ���ǰ������������Ӹ�p2������p2Ҫ�Ȼ������󣬷���Ƿ�

	// �м��ֵ����ʾ��������С�������ô��Ļ�������������p2
	// �����������С�����ַ���p1�ĳ��ȼ��ɡ�strlen(p1) + 1��ע��+1����Ϊ���� \0
	// ����������Ҳû��Ҫ����Ѳ���Ҫ�Ķ���������p2,p2Ҳ�ò���
	// ̫����Ҳ���У�����strlen(p1) + 20��������p2�Ĵ�С��p2�ͻ���ʷǷ��ڴ棬����
	// ���ԣ��������ߴ�  strlen(p1)+1 <= size <= strlen(p2) + 1��ע��\0Ҳ����
	// ��������������Ӧ�ö�����
	// ����strcat_t��ƴ�ӵ�ʱ�򣬻��p2��\0���ǣ�p1��\0�´����������size
	// strlen(p1)+1 <= size <= p2ʣ����ÿռ�
	// ����bug�㣡char *p1 = "abcd"; ������ʽ�£���abcd������ȫ�̶��ˣ�����ڴ治�ɶ���
	// Ҳ���ǳ��������Ǳ�����p1ָ������[abcd]�ڴ棬��С��������ȫ�̶���һ�㲻�ܶ�
	// ��Ҫ�Ķ�p1���Ǿ���p1ָ�������ط��ɣ�����ڴ����Ǳ��붯�ˡ�

	// char *p2;
	// strcpy_s(p2, strlen(p1) + 4, p1); 
	// Ϊɶ����Ĳ��У�����������ˣ���Ϊp2ָ��Ŀռ����û�а�������ô���ƹ�ȥ��
	// ��������Ļ����ڴ�����ˣ����ܹ�������ȥ
	// �ײ�����ǳ����������ͣ����C�������ǲ���������ˡ���

	// ���ԣ���p3����ռ䣬�ٿ�������ok�ˣ��ǵ�����ͷ�Ŷ��
	char *p3;
	// p3 = (char *)malloc(sizeof(strlen(p1) + 1)); // ���ǲ��Ƿ��ˣ����Ƿ����˶����ڴ棿
	p3 = (char *)malloc(strlen(p1) + 1);
	printf("%d\n", strlen(p1) + 1); // ����ֵ��5��������size_t
	printf("%d\n", sizeof(strlen(p1) + 1));  // sizeof�Ĳ������������ͣ�����size_t���͵�ֵ��4
											 // size_t ���� unsigned int����Դ�ļ����Կ�����
	// ���ʵ�ʷ����ˡ�4�ֽڴ�С�Ķѿռ䡿
	// ����д����5�ֽڵĿռ�
	// ��ӡû����д��Ҳû�����ͷŵ�ʱ�򱨴��ˡ���������
	// ��Ӧ�ò�����д����
	// ��ջ�ռ䱻�ƻ� - �ֲ�������ͬ����д��û���⣬�����������Զ��ͷ�ջ�ռ䣩�ű���
	// ���ѿռ��ƻ� - ��̬��������������ͷŵ�ʱ��ű������档

	strcpy_s(p3, strlen(p1) + 1, p1); // ��Ϊ������²���free(p3)�𣿣�
	printf("\np3 = %s", p3); // ��ӡ%s��ʱ������'\0'��Ϊ������־��
	free(p3); // ������Ҫ�ǵ��ͷ� ��error????����������

	// ע��char *p1 = "abcd" ��ȡ�ַ�����С�ķ�ʽ��Ҫ���ú���strlen
	// �Ͼ����ܶ�Ҳ����д����
	// ����char p2[20]... ��ȡ���ȵķ�ʽ�������ˣ���һ��������һ���ģ�
	// "a" ������ʽ�ĳ������ᱻ����Ϊ2���ַ���'a'��'\0'���ַ���Ĭ�Ͻ�β��'\0'
	// \0 ASCII �� 0x00��Linux������ڸ������У��س���ͦ����ġ���


	// *************** //
	/* char --> char*  */
	char c = 'a';
	char *p4 = (char *)malloc(sizeof(char) + 1);
	p4[0] = c;
	p4[1] = '\0';

	printf("\n%s\n", p4); // OK! �ɹ����ַ�'a'ת��Ϊ�ַ���"a"

	char *p5 = (char *)malloc(strlen(p1) + strlen(p3) + 1);
	// Ŀ��ֻ�Ǹ������С�Ŀ�д�ڴ�
	// ��������СҪ�ڷ�Χ�� [Դ��Ŀ��]
	// Դ�ַ��������� \0 ��β
	strcpy_s(p5, strlen(p1) + 1, p1); // Ӧ����ִ������֮�󣬵��µĲ���free
	strcat_s(p5, strlen(p5) + strlen(p3) + 1, p3); // ƴ�� ������
	//strcat_s(p5, strlen(p4) + 1, p4); // ƴ��  ������ ����debug error����why

	printf("\n p5 = %s\n", p5); // OK! 

	free(p4); // ok! why free(p3) --> debug error ?


	char *firstName = "Theo";
	char *lastName = "Tsao";
	char *name = (char *)malloc(strlen(firstName) + strlen(lastName) + 1);
	// strcpy_s(name, strlen(firstName) + 1,firstName);
	// strcat_s(name, strlen(lastName) + 1,lastName); // ����ִ�в�ִ������ģ����Ҳ���У����ô��ˣ�
	printf("%s\n", name);					// ��һ����Ч�������ݸ��˽���Ч������Ϊ���ش���ĺ�����?


	char *ret = (char*)malloc(120);
	memset(ret, 0, sizeof(ret)); // �ǳ���Ҫ�����û��\0,�Ͳ���ƴ��
	char *str1 = "This is a demonstration program, ";
	char *str2 = "considerations for using strcat_s.";

	int len1 = strlen(str1) + 1;
	strcat_s(ret, len1, str1); // ok����
	// strcat_s(ret, strlen(str1) + 1, str1); // not ok...buffer is to small,but why?

	int len2 = strlen(ret) + strlen(str2) + 1;
	strcat_s(ret, len2, str2);

	char a[20] = "abcd";
	int l = strlen(a);	// �Ժ��ַ�����ȡ��������ˣ������������˼Ҿͼ�鵽\0֮ǰ���ַ�����
						// ������ char *c = "abcd",char *c = malloc,char c[100]
	char *acc = "abcde";
	l = strlen(acc);

	//char *acc2 = (char *)malloc(100);
	//acc2 = "ac1234"; // ���ʱ�����÷��������ı�Ϊac1234������ֱ�ӷ���һ����̬����������acc2ָ��
	//ll = strlen(acc2);

	//*acc2 = "ac12345";	// *acc2ָ�ľ���acc2[0]���ַ�������ȫ����ֵ����
	//l = strlen(acc2);	// ��һֱ��acc2ָ�����ʼ��ַ��ʼ��ֱ����\0(0x00)��
						//�����⺯��������û���ˣ� 

	// ���ʱ��Ҫ��acc2ʹ�ö�̬�ڴ渳ֵ����ֻ�ܡ��ڴ濽������һ�����ַ�������ȥ
	// ��char *�����ַ���������ָͨ��Ҳһ����Ψһ��������ǿ��Ը�ֵ"acd"
	// �ڴ濽�����κ����������ʹ�õģ�ֻ�����ַ������ã����������ר������
	// �ַ��������ģ������ĺ�����memcpyʲô�ġ�
	//strcpy_s(acc2, strlen(acc2) + 1, acc);	// it's ok,the second argument 
											// is equal the size of destination string buffer 
	// �ڴ濽����ʵ�ڷ�Χ�ڶ����ԣ�ƴ�Ӻ�������ô���£�

	/* ����strcat_s�ٴβ��� */
	char *t1 = "hello world";
	char *t2 = "java is the best language!";
	char *t12 = (char *)malloc(100); // ��ʱ������ַ�����û��\0
					// ���ǰ���100Ҳ���룬����ͻᱨ�������ǳ����ѵĿռ䷶Χ�ˡ�
	t12[0] = '\0';


	/* It's ok! */
	// ��һ������copy��ã���������ġ�ƴ����㡿���Ǵ�'\0'��ʼ��
	strcat_s(t12, strlen(t12) + strlen(t1) + 1, t1); // ������t12û�п��Ը��ǵ�\0���Ͳ���ƴ��
	strcat_s(t12, strlen(t12) + strlen(t2) + 1, t2);
	// ע�����2�Ļ�������С���ǡ�Ŀ�껺������С����������ƴ��
	// ����Ӧ���ǣ�Ŀ�껺������С = ��Ŀ���С + Դ��С


	// C�����У�����Ĳ������ݵ���ָ�������ֵ���ƣ�������Ϊ����������(char *) (char[])
	// ����������Ϊ����ֵֻ���� char *����ʵ���ǰ�ָ�봫��ȥ��ע�ⲻҪ���ݾֲ�����


	// С��
	// 1. �ַ�����������������С��Դ�ַ�����С����ΪĿ���ַ���δ����\0
	// 2. �ַ���ƴ�ӣ���������С������ƴ�Ӻ���´���С
	// 3. �����ַ���ƴ�ӳ�һ������Ҫ�����������Ȼ���ȿ���һ������ƴ����һ��



	return 0;
}

