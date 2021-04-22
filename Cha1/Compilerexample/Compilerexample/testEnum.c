// 测试C语言的enum
#include <stdio.h>
#include <stdlib.h>

#define aa1 1
#define aa2 2

/*
	不管是结构体、联合体还是枚举类型，都仅仅是
	C语言层级的结构声明而已，如果不创建变量，
	并为变量赋值，编译之后就没了。

	对于基本数据类型，它其实跟结构体这些变量一样的，
	都仅仅是C语言层级的声明，定义变量如果不赋值，
	也不会出现在汇编层级，编译完就没了。

	所以这些东西，其实就是只是放在了表明，真正的程序
	不会出现，因为根本没有使用，占着茅坑不拉屎，我才不要你呢
*/


int a() {
// #define aa2 3 // 不要重定义！这个都是全局的！
	enum aa { a1, a2, a3 }; //	枚举类型的重要作用，就是构建{名称，值}的对应关系 \
								用名称等同于用int值，在编译原理很受用啊！
	return a1;
}


int main333() {
	/* 
     * 以下枚举的使用，相当于是
	 * int a1 = 10;
	 * int a2 = 11;
	 * int a3 = 12;
	 */
	// 只是一个声明，编译后不占空间，只是建立起变量名和值的对应关系
	// 这一点，功能和define是一样的
	// 也需要注意，enum内的变量名，在本区域内，不能再用做变量名，这容易理解
	// define占用的变量名同样不行，而且会全局不可用
	// 不过define可以定义很多内容，浮点数也可以，复杂表达式也可以，enum只能是int类型的数
	// 所以说，在需要定义整数与变量名的对应关系的时候，用enum更好
	// 其他情况也只能用define了。
	enum aa { a1 = 10, a2, a3 }; // 局部枚举类型aa

	enum aa abc = a2;  // 可以理解为 int a2 = 11;  int abc = a2;

	printf("%d\n", a());
	printf("%d\n", a1);
	printf("%d\n", abc);

	abc = 100;
	printf("***%d\n", abc); // ok，这个时候abc就当成int类型用了。

	//int a1 = 100; // error！ al就是一个变量名了！他的值是10
	// 疑问！enum的size？
	// 只占4个字节，也就是一个int的大小
	printf("size = %d\n", sizeof(enum aa)); // 4 这个和union联合体类似，只占一块空间
	printf("size = %d\n", sizeof(abc));		// 4

	// int aa2 = 100;

	struct myAA
	{
		enum aa abcc;
	} myAAa;

	printf("size = %d\n", sizeof(myAAa));	// 4

	myAAa.abcc = a1;
	myAAa.abcc = 200; // 可以超范围吗

	return 0;
}
