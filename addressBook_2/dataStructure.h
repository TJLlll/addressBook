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
    int (*compareFunc)(char *arg1, char *arg2);
} balanceBinarySearchTree;

/* 队列的结点 */
typedef struct queueNode
{
    void* data;
    /* 指向下一个结点的指针 */
    struct queueNode *next;
    struct queueNode *prev;
} queueNode;

/* 队列 */
typedef struct queue
{
    /* 队列的虚拟头结点 */
    queueNode * head;
    queueNode * tail;
    /* 队列的长度 */
    int len;
} queue;

/* 队列初始化 */
int queueInit(queue **pQueue);
/* 入队 */
int queuePush(queue * pList, void *val);
/* 获取队头位置的数据 */
int queueGetHeadVal(queue * pList, ELEMENTTYPE *pVal);
/* 获取队列的长度 */
int queueGetLength(queue * pList);
/* 队列是否为空 */
int queueIsEmpty(queue *pQueue);
/* 出队 */
int queuePop(queue * pList);
/* 队列的销毁 */
int queueDestroy(queue * pList);
/* 函数比较器 */
int compareFunc(char *arg1, char *arg2);
/* 平衡二叉搜索树初始化 */
int balanceBinarySearchTreeInit(balanceBinarySearchTree **pBstree, int (*compareFunc)(char *arg1, char *arg2));
/* 创造树的新结点 */
AVLTreeNode *createBSTreeNewNode(ELEMENTTYPE val, AVLTreeNode *parent);
/* 平衡二叉搜索树的插入 */
int balanceBinarySearchTreeInsert(balanceBinarySearchTree *pBstree, contacts *newContacts);
/* 按照姓名排序，显示所有联系人 */
int displayAllContactInfo(balanceBinarySearchTree *pBstree);
/* 根据指定的值获取二叉搜索树的结点 */
AVLTreeNode * baseAppointValGetAVLTreeNode(balanceBinarySearchTree *pBstree, char *data);
/* 删除结点 */
int balanceBinarySearchTreeDeleteNode(balanceBinarySearchTree *pBstree, AVLTreeNode *node);
/* 二叉搜索树的销毁 */
int balanceBinarySearchTreeDestroy(balanceBinarySearchTree *pBstree);

#endif