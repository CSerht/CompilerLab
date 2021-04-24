#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 栈破坏，函数结束就检测，因为函数结束的时候会清空栈
void testStackCorrupt() {
	char a[2];
	a[2] = 'a';
	printf("%c\n", a[2]);
}

// 堆破坏，只有free的时候才检测到，如果一直不free或者地址丢失，就内存泄漏
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

	// 字符指针到底怎么赋值啊……草
	//char *w = "1234";
	//printf("%s\n", w);
	//// char *id; // 本质就是字符指针！可以指向一个字符，也可以指向一串字符，就像 int *动态数组内样
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

	//p = a;	// 两边其实都是地址，就是让p指针的指向发生了改变。
	//		// 不能返回！返回了栈区的东西就没了，书本示例是打印的……

	//printf("%s\n", p);
	
	//char x = 'a';
	//char *xx = "a";  【\0 的 ASCII 是 0x00 】

	//struct a {
	//	int i;
	//	char *c;
	//};


	// 第一个字符
	//char *p1 = (char *)malloc(sizeof(char));
	//char c1 = 'a';
	//*p1 = c1; // p1  = "a"
	//printf("p1 = %c\n", p1);

	// 字符串要添加结束符 \0
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

	printf("%c\n", p1[0]); // 可以读取，但不能写入，这是常量
	printf("%c\n", p1[3]); 
	// p1[0] = 'c'; 该区域禁止写入！

	char p2[20];
	strcpy_s(p2, strlen(p1) + 1, p1); 
	printf("p2 = %s", p2);

	// 把p1指向的内存，拷贝strlen(p1) + 1字节大小，给p2指向的内存
	// 并且，缓冲区如果比strlen(p1) + 1大，那么缓冲区空余区域的值，是未知(随机)的
	// 并不是连着p1内存后面的一起搬过来，内部分是非法区域！
	// 但是缓冲区拷贝给p2的时候，是把整个缓冲区扔给p2，所以p2要比缓冲区大，否则非法

	// 中间的值，表示缓冲区大小，会把这么大的缓冲区，拷贝给p2
	// 这个缓冲区大小，是字符串p1的长度即可【strlen(p1) + 1】注意+1，因为还有 \0
	// 缓冲区大了也没必要，会把不必要的东西拷贝给p2,p2也用不着
	// 太大了也不行，比如strlen(p1) + 20，超过了p2的大小，p2就会访问非法内存，报错！
	// 所以，缓冲区尺寸  strlen(p1)+1 <= size <= strlen(p2) + 1，注意\0也算上
	// 其他带缓冲区的应该都类似
	// 对于strcat_t，拼接的时候，会把p2的\0覆盖，p1的\0会拷过来，至于size
	// strlen(p1)+1 <= size <= p2剩余可用空间
	// 最大的bug点！char *p1 = "abcd"; 这种形式下，“abcd”就完全固定了，这段内存不可动！
	// 也就是常量，不是变量，p1指向的这段[abcd]内存，大小和内容完全固定，一点不能动
	// 想要改动p1，那就让p1指向其他地方吧，这个内存你是别想动了。

	// char *p2;
	// strcpy_s(p2, strlen(p1) + 4, p1); 
	// 为啥上面的不行，很容易理解了！因为p2指向的空间根本没有啊！那怎么复制过去？
	// 开个数组的话，内存分配了，就能够拷贝过去
	// 底层的理解非常清晰，如果停留在C语言怕是不可能理解了……

	// 所以，给p3分配空间，再拷贝，就ok了！记得最后释放哦！
	char *p3;
	// p3 = (char *)malloc(sizeof(strlen(p1) + 1)); // 你是不是疯了，这是分配了多少内存？
	p3 = (char *)malloc(strlen(p1) + 1);
	printf("%d\n", strlen(p1) + 1); // 它的值是5，类型是size_t
	printf("%d\n", sizeof(strlen(p1) + 1));  // sizeof的参数是数据类型，所以size_t类型的值是4
											 // size_t 就是 unsigned int，在源文件可以看见的
	// 因此实际分配了【4字节大小的堆空间】
	// 但是写入了5字节的空间
	// 打印没报错，写入也没报错，释放的时候报错了【见鬼……】
	// 不应该不允许写入吗？
	// 【栈空间被破坏 - 局部变量】同样读写都没问题，函数结束（自动释放栈空间）才报错，
	// 【堆空间破坏 - 动态申请变量】，在释放的时候才报错，神奇。

	strcpy_s(p3, strlen(p1) + 1, p1); // 因为这个导致不能free(p3)吗？？
	printf("\np3 = %s", p3); // 打印%s的时候，是以'\0'作为结束标志的
	free(p3); // 不用了要记得释放 【error????【】【】】

	// 注意char *p1 = "abcd" 获取字符串大小的方式，要调用函数strlen
	// 毕竟不能读也不能写……
	// 对于char p2[20]... 获取长度的方式就容易了，与一般数组是一样的！
	// "a" 这种形式的常量，会被编译为2个字符，'a'和'\0'，字符串默认结尾是'\0'
	// \0 ASCII 是 0x00，Linux里面的内个【换行，回车】挺诡异的……


	// *************** //
	/* char --> char*  */
	char c = 'a';
	char *p4 = (char *)malloc(sizeof(char) + 1);
	p4[0] = c;
	p4[1] = '\0';

	printf("\n%s\n", p4); // OK! 成功让字符'a'转换为字符串"a"

	char *p5 = (char *)malloc(strlen(p1) + strlen(p3) + 1);
	// 目标只是个分配大小的可写内存
	// 缓冲区大小要在范围内 [源，目标]
	// 源字符串必须以 \0 结尾
	strcpy_s(p5, strlen(p1) + 1, p1); // 应该是执行完它之后，导致的不能free
	strcat_s(p5, strlen(p5) + strlen(p3) + 1, p3); // 拼接 ？？？
	//strcat_s(p5, strlen(p4) + 1, p4); // 拼接  ？？？ 还是debug error……why

	printf("\n p5 = %s\n", p5); // OK! 

	free(p4); // ok! why free(p3) --> debug error ?


	char *firstName = "Theo";
	char *lastName = "Tsao";
	char *name = (char *)malloc(strlen(firstName) + strlen(lastName) + 1);
	// strcpy_s(name, strlen(firstName) + 1,firstName);
	// strcat_s(name, strlen(lastName) + 1,lastName); // 不管执行不执行上面的，这个也不行，是用错了？
	printf("%s\n", name);					// 将一个无效参数传递给了将无效参数视为严重错误的函数。?


	char *ret = (char*)malloc(120);
	memset(ret, 0, sizeof(ret)); // 非常重要，如果没有\0,就不能拼接
	char *str1 = "This is a demonstration program, ";
	char *str2 = "considerations for using strcat_s.";

	int len1 = strlen(str1) + 1;
	strcat_s(ret, len1, str1); // ok？？
	// strcat_s(ret, strlen(str1) + 1, str1); // not ok...buffer is to small,but why?

	int len2 = strlen(ret) + strlen(str2) + 1;
	strcat_s(ret, len2, str2);

	char a[20] = "abcd";
	int l = strlen(a);	// 以后字符串获取都用这个了，不管数组多大，人家就检查到\0之前的字符数量
						// 不管是 char *c = "abcd",char *c = malloc,char c[100]
	char *acc = "abcde";
	l = strlen(acc);

	//char *acc2 = (char *)malloc(100);
	//acc2 = "ac1234"; // 这个时候不是让分配的区域改变为ac1234，而是直接分配一个静态数据区，让acc2指向
	//ll = strlen(acc2);

	//*acc2 = "ac12345";	// *acc2指的就是acc2[0]，字符串不能全部赋值给它
	//l = strlen(acc2);	// 会一直从acc2指向的起始地址开始，直到有\0(0x00)，
						//所以这函数在这里没用了！ 

	// 这个时候要想acc2使用动态内存赋值，就只能【内存拷贝】，一个个字符拷贝过去
	// 把char *表达的字符串当作普通指针也一样，唯一的区别就是可以赋值"acd"
	// 内存拷贝，任何情况都可以使用的，只不过字符串常用，另外这个是专门用于
	// 字符串拷贝的，其他的好像是memcpy什么的。
	//strcpy_s(acc2, strlen(acc2) + 1, acc);	// it's ok,the second argument 
											// is equal the size of destination string buffer 
	// 内存拷贝其实在范围内都可以，拼接好像不是那么回事！

	/* 对于strcat_s再次测试 */
	char *t1 = "hello world";
	char *t2 = "java is the best language!";
	char *t12 = (char *)malloc(100); // 此时，这个字符串，没有\0
					// 如果前面的100也申请，这个就会报错，可能是超过堆的空间范围了。
	t12[0] = '\0';


	/* It's ok! */
	// 第一个，用copy最好！这个东西的【拼接起点】，是从'\0'开始的
	strcat_s(t12, strlen(t12) + strlen(t1) + 1, t1); // 因此这里，t12没有可以覆盖的\0，就不能拼接
	strcat_s(t12, strlen(t12) + strlen(t2) + 1, t2);
	// 注意参数2的缓冲区大小，是【目标缓冲区大小】，这里是拼接
	// 所以应该是，目标缓冲区大小 = 旧目标大小 + 源大小


	// C语言中，数组的参数传递的是指针而不是值复制，数组作为参数可以是(char *) (char[])
	// 但是数组作为返回值只能是 char *，其实就是把指针传过去，注意不要传递局部变量


	// 小结
	// 1. 字符串拷贝，缓冲区大小用源字符串大小，因为目标字符串未必有\0
	// 2. 字符串拼接，缓冲区大小必须是拼接后的新串大小
	// 3. 两个字符串拼接成一个，需要申请更大区域，然后先拷贝一个，再拼接另一个



	return 0;
}

