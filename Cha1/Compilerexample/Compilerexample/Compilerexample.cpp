#include <stdio.h>
#include <stdlib.h>

// 你是看不见这个字么……在这里添加你的代码啊……眼瞎
// 给你框架，让你写代码，就这个意思。
#define TODO() \
do{ \
	printf("\nAdd your code here: file \"%s\", line %d\n", __FILE__, __LINE__); \
} while (0)


/////////////////////////////////////////////// 
// Data structures for the Sum language. 

// 0：整数表达式 ， 1：求和表达式
enum Exp_Kind_t { EXP_INT, EXP_SUM }; // 两种受支持的字符类型，“整数 -- 0”和“+ -- 1”

// 保存“+”的Token结构
struct Exp_t {	// 这个结构体纯碎是为了让Int和Sum结构体能够混合使用，是一个过渡。
	enum Exp_Kind_t kind;
};

// 保存“整数”的Token结构
struct Exp_Int {
	enum Exp_Kind_t kind;	// 类型
	int i;					// 值
};

// 一颗二叉树
struct Exp_Sum {
	enum Exp_Kind_t kind;
	struct Exp_t *left;
	struct Exp_t *right;
};


// "constructors" 创建一个指向“整数Tonken”的指针，并初始化这个结构
// 为什么使用Exp_t指针指向Exp_Int结构？？【？】这是咋访问的...
struct Exp_t *Exp_Int_new(int i) {
	struct Exp_Int *p = (Exp_Int *)malloc(sizeof(*p));
	p->kind = EXP_INT;
	p->i = i;
	return (struct Exp_t *)p;
}


// 创建一个二叉树
// 体会oop思想，这个函数一直在操作 结构体Exp_Sum
struct Exp_t *Exp_Sum_new(struct Exp_t *left, struct Exp_t *right) {
	struct Exp_Sum *p = (Exp_Sum *)malloc(sizeof(*p)); // 堆空间不会消失
	p->kind = EXP_SUM;
	p->left = left;
	p->right = right;
	return (struct Exp_t *)p; //	为了书写复杂表达式嵌套，才换的指针 
							  //	这个指针可以让Sum_new函数参数嵌套使用 
							  //	也可以保存sum树形结构 
							  //	并且能够保证，嵌套使用之后，左右子树直接指向 
							  //	kind，因为他是结构体最开始的部分
}


// "printer" 
void Exp_print(struct Exp_t *exp) {
	switch (exp->kind) {
	case EXP_INT: {
		struct Exp_Int *p = (struct Exp_Int *)exp; // Exp_t的目的是让其他两个结构体混合使用 
												   //  它做中间媒介，并且存了地址，之后还能换回来
		printf("%d", p->i);
		break;
	}
	case EXP_SUM: {
		struct Exp_Sum *p = (struct Exp_Sum *)exp;
		// 递归，中序遍历整个树
		Exp_print(p->left); 
		printf("+");
		Exp_print(p->right);
		break;
	}
	default:
		break;
	}
}


//////////////////////////////////////////////
// Data structures for the Stack language.
// Sum（A语言）编译目标：栈计算机的结构定义（或者这就是B语言）
enum Stack_Kind_t { STACK_ADD, STACK_PUSH }; // 栈计算机支持 push add 两条指令

// 作为add和push的过渡指针
struct Stack_t {
	enum Stack_Kind_t kind;
};

// add
struct Stack_Add {
	enum Stack_Kind_t kind;
};


// push n
struct Stack_Push {
	enum Stack_Kind_t kind;
	int i;
};


// "constructors"
// create [add] instruction
struct Stack_t *Stack_Add_new() {
	struct Stack_Add *p = (Stack_Add *)malloc(sizeof(*p));
	p->kind = STACK_ADD;
	return (struct Stack_t *)p;
}

// create [push n] instruction
struct Stack_t *Stack_Push_new(int i) {
	struct Stack_Push *p = (Stack_Push *)malloc(sizeof(*p));
	p->kind = STACK_PUSH;
	p->i = i;
	return (struct Stack_t *)p;
}


// instruction list
// 链表结构，存储指令序列
struct List_t {
	struct Stack_t *instr;
	struct List_t *next;
};

// create instruction list
struct List_t *List_new(struct Stack_t *instr, struct List_t *next) {
	struct List_t *p = (List_t *)malloc(sizeof(*p));
	p->instr = instr;
	p->next = next;
	return p;
}


// "printer" 指令序列是倒着添加的，list pointer指向的是最后一条指令
// 单链表倒序打印 -- 递归实现（C语言没有栈函数吧……）
void List_reverse_print(struct List_t *list) {
	// TODO(); 自己写这部分代码
	if (list == 0)
		return;

	List_reverse_print(list->next);

	// struct List_t *p = list;
	struct Stack_t *inst = list ->instr;

	if (inst->kind == STACK_PUSH) {
		struct Stack_Push *push = (struct Stack_Push *)inst;
		printf("push  %d\n", push->i);
	}
	else if (inst->kind == STACK_ADD){
		printf("add\n");
	}

}


//////////////////////////////////////////////////
// a compiler from Sum to Stack
// 编译器：从Sum语言转换为Stack语言
struct List_t *all = 0; // instruction list


// 初始化存储指令列表，链表向前添加元素
void emit(struct Stack_t *instr) {
	all = List_new(instr, all);
}

// 编译，直接把AST进行后序遍历，得到目标代码
// case语句内的变量，用不用 { } 还有讲究，有一个小问题我记得
// 这也没编译啊……是让我自己补充么？
void compile(struct Exp_t *exp) {
	switch (exp->kind) {
		case EXP_INT: { // push n
			struct Exp_Int *p = (struct Exp_Int *)exp;
			emit(Stack_Push_new(p->i));
			break;
		}
		case EXP_SUM: { // add
			// 这部分代码自己写
			// TODO();
			// 后序遍历
			struct Exp_Sum *sum = (struct Exp_Sum *)exp;
			compile(sum->left);
			compile(sum->right);
			emit(Stack_Add_new());
			break;
		}
		default:
			break;
	}
}

//////////////////////////////////////////////////
// program entry
int main() {
	printf("Compile starting\n***********\n");

	// build an expression tree:
	//            +
	//           / \
	//          +   3
	//         / \
	//        1   2

	// 在初始化完成之后，就已经是一颗AST了
	// 相当于初始化直接就完成了【词法分析】和【语法分析】
	// 画图分析！！
	struct Exp_t *exp = Exp_Sum_new(
		Exp_Sum_new(
			Exp_Int_new(11),
			Exp_Int_new(22)
		),
		Exp_Sum_new(
			Exp_Int_new(33),
			Exp_Int_new(44)
		)
	); // 后期，就应该，直接写 1 + 2 + 3，然后自动生成AST，而不是手写这部分。

	// print out this tree:
	printf("\nthe expression is:\n");
	Exp_print(exp);

	// 编译
	// compile this tree to Stack machine instructions
	compile(exp);

	// print out the generated Stack instructons:
	printf("\n\nthe instruction list is:\n");
	List_reverse_print(all);
	printf("\n*************\nCompile finished\n");

	return 0;
}