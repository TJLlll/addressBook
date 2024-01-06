#ifndef __BINARY_SEARCH_TREE_H_
#define __BINARY_SEARCH_TREE_H_

#define ELEMENTTYPE void*
#define NAME_SIZE 21    //最多支持十个汉字、二十个字母的名字
#define PHONENUMBER_SIZE 15    //最多支持十四个数字的号码

/* 状态码 */
enum STATUS_CODE
{
    ON_SUCCESS,
    NULL_PTR = -1,
    MALLOC_ERROR = -2,
    OTHER_ERROR = -3,
};

/* 联系人数据结构体 */
typedef struct contacts
{
    /* 联系人姓名 */
    char name[NAME_SIZE];
    /* 联系人号码 */
    char phoneNumber[PHONENUMBER_SIZE];

} contacts;

/* 平衡二叉搜索树的结点 */
typedef struct AVLTreeNode
{
    struct contacts *data;           /* 结点中存放联系人数据 */
    int height;                      /* 树高*/
    struct AVLTreeNode *left;        /* 左子树 */
    struct AVLTreeNode *right;       /* 右子树 */
    struct AVLTreeNode *parent;      /* 父结点 */
} AVLTreeNode;

/* 平衡二叉搜索树 */
typedef struct balanceBinarySearchTree
{   
    /* 根结点 */
    AVLTreeNode * root;
    /* 树的结点个数 */
    int size;
    /* 函数比较器 */
    int (*compareFunc)(contacts *arg1, AVLTreeNode *arg2);
} balanceBinarySearchTree;

/* 队列的结点 */
typedef struct queueNode
{
    ELEMENTTYPE data;
    /* 指向下一个结点的指针 */
    struct queueNode *next;
} queueNode;

/* 队列 */
typedef struct queue
{
    /* 队列的虚拟头结点 */
    queueNode * head;
    /* 队列的长度 */
    int len;
} queue;

#endif