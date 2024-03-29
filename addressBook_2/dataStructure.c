/* 数据的主架构 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"
#include "dataStructure.h"
#include "newAddressBookUserInterface.h"
#include "keyMonitoring.h"

#define true 1
#define false 0
#define ESC 27  //esc键的ascii码
#define X_INITPOS 3    //光标x初始位置
#define Y_INITPOS 10    //光标y初始位置
#define X_POS_LOOKIN 3  //显示查看详情光标x初始位置
#define Y_POS_LOOKIN 24 //显示查看详情光标y初始位置
#define ENTER 10        //回车键的ascii码

static int x = X_INITPOS;       //光标坐标  
static int cont = 0;            //打印个数
static int flag = 1;            //是否终止打印联系人信息，1为不终止，0终止打印
static int printCont = 0;       //本次打印的结点数

/* 获取AVL结点较高的子结点 */
static AVLTreeNode * AVLTreeNodeGetChildTaller(AVLTreeNode *node);
/* 左旋 */
static int AVLTreeCurrentNodeRotateLeft(balanceBinarySearchTree *pBstree, AVLTreeNode *grand);
/* 右旋 */
static int AVLTreeCurrentNodeRotateRight(balanceBinarySearchTree *pBstree, AVLTreeNode *grand);
/* 旋转 */
static int AVLTreeCurrentNodeRotate(balanceBinarySearchTree *pBstree, AVLTreeNode *grand, AVLTreeNode *parent, AVLTreeNode *child);
/* 调整平衡 */
static int AVLTreeNodeAdjustBalance(balanceBinarySearchTree *pBstree,AVLTreeNode *node);
/* 当前结点是父结点的左子树 */
static int AVLTreeCurrentNodeIsLeft(AVLTreeNode *node);
/* 当前结点是父结点的右子树 */
static int AVLTreeCurrentNodeIsRight(AVLTreeNode *node);
/* 计算树结点的平衡因子 */
static int AVLTreeNodeBalanceFactor(AVLTreeNode *node);
/* 判断树是否平衡 */
static int AVLTreeNodeIsBalanced(AVLTreeNode *node);
/* 更新节点的高度 */
static int AVLTreeNodeUpdateHeight(AVLTreeNode *node);
/* 添加结点之后的操作 */
static int insertNodeAfter(balanceBinarySearchTree *pBstree, AVLTreeNode *node);
/* 打印本结点的数据 */
static void printThisData(balanceBinarySearchTree *pBSTree, AVLTreeNode *node);
/* 中序遍历的算法 按照姓名为树结点排序 */
static int inOrderTravel(balanceBinarySearchTree *pBSTree, AVLTreeNode *root);
/* 显示白色背景 */
static void printBackground();
/* 删除结点之后要做的事 */
static int removeNodeAfter(balanceBinarySearchTree *pBstree, AVLTreeNode *node);
/* 判断二叉搜索树度为2 */
static int balanceBinarySearchTreeNodeHasTwochildrens(AVLTreeNode *node);
/* 判断二叉搜索树度为1 */
static int balanceBinarySearchTreeNodeHasOnechildren(AVLTreeNode *node);
/* 判断二叉搜索树度为0 */
static int balanceBinarySearchTreeNodeIsLeaf(AVLTreeNode *node);
/* 获取当前结点的前驱结点 */
static AVLTreeNode * bstreeNodePreDecessor(AVLTreeNode *node);

/* 队列初始化 */
int queueInit(queue **pQueue)
{
    queue *list = (queue*)malloc(sizeof(queue));    
    if (list == NULL)   
    {
        perror("malloc error");
    }
    /* 清空脏数据 */
    memset(list, 0, sizeof(queue));

    list->head = (queueNode *)malloc(sizeof(queueNode));
    if (list->head == NULL)
    {
        return MALLOC_ERROR;
    }
    /* 清空脏数据 */
    memset(list->head, 0, sizeof(queueNode));
    list->head->data = 0;
    list->head->next = NULL;
    /* 虚拟头结点的prev指针置为NULL. */
    list->head->prev = NULL;
    /* 初始化的时候, 尾指针 = 头指针 */
    list->tail = list->head;

    /* 队列的长度为0 */
    list->len = 0;

    /* 将pQueue指针指向初始化后的队列list */
    *pQueue = list;
    return ON_SUCCESS;
}

/* 新建队列的结点 */
static queueNode * newQueueNode(void *pTreeNode)
{
    /* 封装结点 */
    queueNode * newNode = (queueNode *)malloc(sizeof(queueNode) * 1);
    if (newNode == NULL)
    {
        return NULL;
    }
    /* 清除脏数据 */
    memset(newNode, 0, sizeof(queueNode));

    newNode->data = 0;
    newNode->next = NULL;
    /* 新结点的prev指针 */
    newNode->prev = NULL;
    /* 赋值 */
    newNode->data = pTreeNode;

    /* 返回新结点 */
    return newNode;
}

/* 队列指定位置插入 */
static int DoubleLinkListAppointPosInsert(queue * pList, int pos, ELEMENTTYPE val)
{
    if (pList == NULL)
    {
        return NULL_PTR;
    }
    /* 封装结点 */
    queueNode * newNode = (queueNode *)malloc(sizeof(queueNode) * 1);
    if (newNode == NULL)
    {
        return MALLOC_ERROR;
    }
    /* 清除脏数据 */
    memset(newNode, 0, sizeof(queueNode) * 1);

    newNode->data = 0;
    newNode->next = NULL;

    /* 赋值 */
    newNode->data = val;

    /* 从虚拟头结点开始遍历 */
    queueNode * travelNode = pList->head;


    int flag = 0;
    /* 这种情况下需要更改尾指针 */
    if (pos == pList->len)
    {
        /* 修改结点指向 */
        travelNode = pList->tail;

        flag = 1;
    }
    else
    {
        while (pos)
        {
            travelNode = travelNode->next;
            pos--;
        }
        travelNode->next->prev = newNode;       // 3    : 空链表 / 尾插
    }
    newNode->next = travelNode->next;       // 1
    newNode->prev = travelNode;             // 2
    travelNode->next = newNode;             // 4
    
    if (flag)
    {
        /* 尾指针更新位置 */
        pList->tail = newNode;
    }

    /* 更新链表的长度 */
    (pList->len)++;
    return ON_SUCCESS;
}


/* 入队 */
int queuePush(queue * pList, void *val)
{
    return DoubleLinkListAppointPosInsert(pList, pList->len, val);
}


/* 获取队头位置的数据 */
int queueGetHeadVal(queue * pList, ELEMENTTYPE *pVal)
{
    if (pList == NULL)
    {
        return NULL_PTR;
    }
    if (pVal)
    {
        *pVal = pList->head->next->data;
    }
    return ON_SUCCESS;
}

/* 获取队列的长度 */
int queueGetLength(queue * pList)
{
    if (pList == NULL)
    {
        return NULL_PTR;
    }

    /* 返回队列的长度 */
    return pList->len;
}

/* 队列是否为空 */
int queueIsEmpty(queue *pQueue)
{
    int size = 0;
    size = queueGetLength(pQueue);
    return size == 0 ? 1 : 0;
}

/* 队列指定位置删 */
static int DoubleLinkListDelAppointPos(queue * pList, int pos)
{
    if (pList == NULL)
    {
        return NULL_PTR;
    }
    
    queueNode * travelNode = pList->head;

    queueNode * needDelNode = NULL;
    /* 需要修改尾指针 */
    if (pos == pList->len)
    {
        /* 备份尾指针 */
        queueNode * tmpNode = pList->tail;
        /* 移动尾指针 */
        pList->tail = pList->tail->prev;
        needDelNode = tmpNode;
    }
    else
    {
        while (--pos)
        {
            /* 向后移动位置 */
            travelNode = travelNode->next;
        }   
        // 跳出循环找到的是哪一个结点？
        needDelNode = travelNode->next;                 // 1
        travelNode->next = needDelNode->next;           // 2
        needDelNode->next->prev = travelNode;           // 3
    }

    /* 释放内存 */
    if (needDelNode != NULL)
    {
        free(needDelNode);
        needDelNode = NULL;
    }

    /* 链表长度减一 */
    (pList->len)--;
    return ON_SUCCESS;
}


/* 出队 */
int queuePop(queue * pList) 
{
    return DoubleLinkListDelAppointPos(pList, 1);
}

/* 队列的销毁 */
int queueDestroy(queue * pList)
{
    /* 头删释放队列 */
    while (pList->len != 0)
    {
        DoubleLinkListDelAppointPos(pList, 1);
    }

    if (pList->head != NULL)
    {
        free(pList->head);
        /* 指针置为NULL. */
        pList->head = NULL;
        pList->tail = NULL;
    }

    free(pList);
    pList = NULL;
    
    return ON_SUCCESS;
}

/* 函数比较器 */
int compareFunc(char *arg1, char *arg2)
{
    return strcmp(arg1, arg2);
}

/* 平衡二叉搜索树初始化 */
int balanceBinarySearchTreeInit(balanceBinarySearchTree **pBstree, int (*compareFunc)(char *arg1, char *arg2))
{
    balanceBinarySearchTree * bstree = (balanceBinarySearchTree *)malloc(sizeof(balanceBinarySearchTree));
    if (bstree == NULL)
    {
        return MALLOC_ERROR;
    }
    /* 清除脏数据 */
    memset(bstree, 0, sizeof(balanceBinarySearchTree));
    /* 初始化树 */
    {
        bstree->root = NULL;
        bstree->size = 0;
        /* 接入比较器 */
        bstree->compareFunc = compareFunc;
    }

    bstree->root = createBSTreeNewNode(0, NULL);
    if (bstree->root == NULL)
    {
        return MALLOC_ERROR;
    }

    *pBstree = bstree;
    return ON_SUCCESS;
}

/* 创造树的新结点 */
AVLTreeNode *createBSTreeNewNode(contacts *val, AVLTreeNode *parent)
{
    /* 分配根结点 */
    AVLTreeNode * newAVLNode = (AVLTreeNode *)malloc(sizeof(AVLTreeNode) * 1);
    if (newAVLNode == NULL)
    {
        return NULL;
    }
    /* 清除脏数据 */
    memset(newAVLNode, 0, sizeof(AVLTreeNode) * 1);
    /* 初始化根结点 */
    {
        newAVLNode->data = 0;
        /* 结点的高度为1. */
        newAVLNode->height = 1;
        newAVLNode->left = NULL;
        newAVLNode->right = NULL;
        newAVLNode->parent = NULL;
    }

    /* 赋值 */
    newAVLNode->data = val;
    newAVLNode->parent = parent;
    return newAVLNode;
}

/* 获取AVL结点较高的子结点 */
static AVLTreeNode * AVLTreeNodeGetChildTaller(AVLTreeNode *node)
{
    /* 左子树高度 */
    int leftHeight = node->left == NULL ? 0 : node->left->height;
    /* 右子树高度 */
    int rightHeight = node->right == NULL ? 0 : node->right->height;
    if (leftHeight > rightHeight)
    {
        return node->left;
    }
    else if (leftHeight < rightHeight)
    {
        return node->right;
    }
    else
    {
        if (AVLTreeCurrentNodeIsLeft(node))
        {
            return node->left; 
        }
        else if (AVLTreeCurrentNodeIsRight(node))
        {
            return node->right; 
        }
    }
}

/* 左旋 */
static int AVLTreeCurrentNodeRotateLeft(balanceBinarySearchTree *pBstree, AVLTreeNode *grand)
{
    AVLTreeNode *parent = grand->right;
    AVLTreeNode *child = parent->left;

    grand->right = child;                   //1 
    parent->left = grand;                   //2

    /* p成为新的根结点 */
    parent->parent = grand->parent;         //3

    if (AVLTreeCurrentNodeIsLeft(grand))
    {
        grand->parent->left = parent;       //4
    }
    else if (AVLTreeCurrentNodeIsRight(grand))
    {
        grand->parent->right = parent;      //4
    }
    else 
    {
        /* p 成为树的根节点 */
        pBstree->root = parent;             //4
    }
    grand->parent = parent;                 //5

    if (child != NULL)
    {
        child->parent = grand;              //6
    }

    /* 更新高度 */
    AVLTreeNodeUpdateHeight(grand);
    AVLTreeNodeUpdateHeight(parent);
    return ON_SUCCESS;
}
/* 右旋 */
static int AVLTreeCurrentNodeRotateRight(balanceBinarySearchTree *pBstree, AVLTreeNode *grand)
{
    AVLTreeNode *parent = grand->left;
    AVLTreeNode *child = parent->right;

    grand->left = child;                    //1 
    parent->right = grand;                  //2

    /* p成为新的根结点 */
    parent->parent = grand->parent;         //3

    if (AVLTreeCurrentNodeIsLeft(grand))
    {
        grand->parent->left = parent;       //4
    }
    else if (AVLTreeCurrentNodeIsRight(grand))
    {
        grand->parent->right = parent;      //4
    }
    else 
    {
        /* p 成为树的根节点 */
        pBstree->root = parent;             //4
    }
    grand->parent = parent;                 //5

    if (child != NULL)
    {
        child->parent = grand;              //6
    }

    /* 更新高度 */
    AVLTreeNodeUpdateHeight(grand);
    AVLTreeNodeUpdateHeight(parent);
    return ON_SUCCESS;
}

/* 旋转 */
static int AVLTreeCurrentNodeRotate(balanceBinarySearchTree *pBstree, AVLTreeNode *grand, AVLTreeNode *parent, AVLTreeNode *child)
{
    /* p成为新的根结点 */
    parent->parent = grand->parent;         //3

    if (AVLTreeCurrentNodeIsLeft(grand))
    {
        grand->parent->left = parent;       //4
    }
    else if (AVLTreeCurrentNodeIsRight(grand))
    {
        grand->parent->right = parent;      //4
    }
    else 
    {
        /* p 成为树的根节点 */
        pBstree->root = parent;             //4
    }
    grand->parent = parent;                 //5

    if (child != NULL)
    {
        child->parent = grand;              //6
    }

    /* 更新高度 */
    AVLTreeNodeUpdateHeight(grand);
    AVLTreeNodeUpdateHeight(parent);
    return ON_SUCCESS;
}

/* 调整平衡 */
/* node一定是最低的不平衡结点 */
static int AVLTreeNodeAdjustBalance(balanceBinarySearchTree *pBstree,AVLTreeNode *node)
{
    AVLTreeNode *parent = AVLTreeNodeGetChildTaller(node);
    AVLTreeNode *child = AVLTreeNodeGetChildTaller(parent);

    if (AVLTreeCurrentNodeIsLeft(parent))
    {
        /* L */
        if (AVLTreeCurrentNodeIsLeft(child))
        {
            /* LL */  
            AVLTreeCurrentNodeRotateRight(pBstree, node);
        }
        else if (AVLTreeCurrentNodeIsRight(child))
        {
            /* LR */
            AVLTreeCurrentNodeRotateLeft(pBstree, parent);
            AVLTreeCurrentNodeRotateRight(pBstree, node);
        }
    }
    else if (AVLTreeCurrentNodeIsRight(parent))
    {
        /* R */
        if (AVLTreeCurrentNodeIsLeft(child))
        {
            /* RL */  
            AVLTreeCurrentNodeRotateRight(pBstree, parent);
            AVLTreeCurrentNodeRotateLeft(pBstree, node);
        }
        else if (AVLTreeCurrentNodeIsRight(child))
        {
            /* RR */
            AVLTreeCurrentNodeRotateLeft(pBstree, node);
        }
    }
}

/* 当前结点是父结点的左子树 */
static int AVLTreeCurrentNodeIsLeft(AVLTreeNode *node)
{
    return (node->parent != NULL) && (node == node->parent->left);
}
/* 当前结点是父结点的右子树 */
static int AVLTreeCurrentNodeIsRight(AVLTreeNode *node)
{
    return (node->parent != NULL) && (node == node->parent->right);
}

static int tmpMax(int val1, int val2)
{
    return val1 - val2 >= 0 ? val1 : val2;
}
/* 更新结点的高度 */
static int AVLTreeNodeUpdateHeight(AVLTreeNode *node)
{
    /* 左子树的高度 */
    int leftHeight = node->left == NULL ? 0 : node->left->height;
    /* 右子树的高度 */
    int rightHeight = node->right == NULL ? 0 : node->right->height;
    node->height = 1 + tmpMax(leftHeight, rightHeight);
    return ON_SUCCESS;
}

/* 计算树结点的平衡因子 */
static int AVLTreeNodeBalanceFactor(AVLTreeNode *node)
{
    /* 左子树的高度 */
    int leftHeight = node->left == NULL ? 0 : node->left->height;
    /* 右子树的高度 */
    int rightHeight = node->right == NULL ? 0 : node->right->height;

    return leftHeight - rightHeight;
}
/* 判断树是否平衡 */
static int AVLTreeNodeIsBalanced(AVLTreeNode *node)
{
    int nodeFactor = abs(AVLTreeNodeBalanceFactor(node));
    if (nodeFactor <= 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* 添加结点之后的操作 */
static int insertNodeAfter(balanceBinarySearchTree *pBstree, AVLTreeNode *node)
{
    while ((node = node->parent) != NULL)   //  树中有不止一个结点
    {
        /* 程序执行到这个地方一定不止一个结点 */
        if (AVLTreeNodeIsBalanced(node))
        {
            /* 如果结点是平衡的 那就更新高度 */
            AVLTreeNodeUpdateHeight(node);
        }   
        else
        {
            /* node是最低不平衡结点 */
            /* 开始旋转 */
            AVLTreeNodeAdjustBalance(pBstree, node);
            break;
        }
    }
    return ON_SUCCESS;
}

/* 平衡二叉搜索树的插入 */
int balanceBinarySearchTreeInsert(balanceBinarySearchTree *pBstree, contacts *newContacts)
{
    /* 空树 */
    if (pBstree->size == 0)
    {
        /* 更新树的结点 */
        (pBstree->size)++;
        pBstree->root->data = newContacts;
        insertNodeAfter(pBstree, pBstree->root);
        return ON_SUCCESS;
    }

    /* travelNode 指向根结点 */
    AVLTreeNode * travelNode = pBstree->root;
    AVLTreeNode * parentNode = pBstree->root;

    /* 确定符号: 到底放在左边还是右边 */
    int cmp = 0;
    while (travelNode != NULL)
    {
        /* 标记父结点 */
        parentNode = travelNode;
        cmp = pBstree->compareFunc(newContacts->name, travelNode->data->name);

        /* 插入元素 < 遍历到的结点 */
        if (cmp < 0)
        {
            travelNode = travelNode->left;
        }
        else if (cmp > 0)     /* 插入元素 > 遍历到的结点 */
        {
            travelNode = travelNode->right;
        }
        else
        {
            /* 插入元素 = 遍历到的结点 */
            return ON_SUCCESS;
        }
    }
    
    /* 分配根结点 */
    AVLTreeNode * newAVLNode = createBSTreeNewNode(newContacts, parentNode);


    /* 挂在左子树 */
    if (cmp < 0)
    {
        parentNode->left = newAVLNode;
    }
    else
    {   
        /* 挂在右子树 */
        parentNode->right = newAVLNode;
    }
    /* 添加之后的调整 */
    insertNodeAfter(pBstree, newAVLNode);

    /* 更新树的结点 */
    (pBstree->size)++;
    return ON_SUCCESS;
}

/* 中序遍历的算法 按照姓名为树结点排序 */
static int inOrderTravel(balanceBinarySearchTree *pBSTree, AVLTreeNode *root)
{
    if (root == 0)
    {
        return ON_SUCCESS;
    }
    if (flag == 0)
    {
        return ON_SUCCESS;
    }

    AVLTreeNode *travelNode = root;
    /* 左子树 */
    inOrderTravel(pBSTree, travelNode->left);
    /* 根结点 */
    printThisData(pBSTree, travelNode);    //打印本结点的数据
    /* 右子树 */
    inOrderTravel(pBSTree, travelNode->right);
    return ON_SUCCESS;
}

/* 按照姓名排序，显示所有联系人 */
int displayAllContactInfo(balanceBinarySearchTree *pBstree)
{
    printBackground();
    flag = 1;
    printCont = 0;
    x = X_INITPOS;
    cont = 0;
    printf("\033[?25l");    //隐藏光标
    inOrderTravel(pBstree, pBstree->root);
    printf("\033[0;0;0m");
    printf("\033[?25h");    //显示光标

    int key = 0;
    while (1)
    {
        if (flag == 0)
        {
            return ON_SUCCESS;
        }
        key = returnKey();
        if (key == ESC || key == ENTER || key == 's' || key == 'w')
        {
            return ON_SUCCESS;
        }
    }
}

/* 根据指定的值获取二叉搜索树的结点 */
AVLTreeNode * baseAppointValGetAVLTreeNode(balanceBinarySearchTree *pBstree, char *data)
{
    AVLTreeNode * travelNode = pBstree->root;

    int cmp = 0;
    while (travelNode != NULL)
    {
        /* 比较大小 */
        cmp = strcmp(data, travelNode->data->name);
        if (cmp < 0)
        {
            travelNode = travelNode->left;
        }
        else if (cmp > 0)
        {
            travelNode = travelNode->right;
        }
        else
        {
            /* 找到了. */
            return travelNode;
        }
    }

    travelNode = pBstree->root;
    while (travelNode != NULL)
    {
        /* 比较大小 */
        cmp = strcmp(data, travelNode->data->phoneNumber);
        if (cmp < 0)
        {
            travelNode = travelNode->left;
        }
        else if (cmp > 0)
        {
            travelNode = travelNode->right;
        }
        else
        {
            /* 找到了. */
            return travelNode;
        }
    }
    return NULL;
}

/* 显示白色背景 */
static void printBackground()
{
    printf("\033[0;0;0m\n");      
    system("clear");    //清屏
    print_();
    printspace(9);          //显示白色底板
    print_();
    printf("            (请按s键翻页)\n      (按ESC或回车键返回上一级)\n");
}


/* 打印本结点的数据 */
static void printThisData(balanceBinarySearchTree *pBSTree, AVLTreeNode *node)
{
    if (cont == 0)
    {
        printBackground();
    }
    int y = Y_INITPOS;
    printf("\033[%d;10H\033[0;30;47m", x + cont);   //设置光标位置显示“姓名”
    printf("\033[3;30;47m姓名：");
    printf("\033[0;30;47m %s   ", node->data->name);

    printf("\033[%d;10H\033[0;30;47m", x + cont + 1);   //设置光标位置显示“号码”
    printf("\033[3;30;47m号码：");
    printf("\033[0;30;47m %s   ", node->data->phoneNumber);

    x = x + 2;
    cont++;
    printf("\033[0m\n");
    printCont++;

    if (cont == 3)
    {
        int key = 0;
        while (1)
        {
            if (flag == 0)
            {
                break;
            }
            key = returnKey();
            if (key == ESC || key == ENTER)
            {
                flag = 0;   //停止打印并退出
                break;
            }
            if (key == 's')
            {
                printf("\033[0;0;0m\n");
                x = X_INITPOS;
                cont = 0;
                if (printCont == pBSTree->size)
                {
                    flag = 0;
                }
                break;
            }
        }
    }
}

/* 判断二叉搜索树度为2 */
static int balanceBinarySearchTreeNodeHasTwochildrens(AVLTreeNode *node)
{
    return (node->left != NULL) && (node->right != NULL);
}

/* 判断二叉搜索树度为1 */
static int balanceBinarySearchTreeNodeHasOnechildren(AVLTreeNode *node)
{
    return ((node->left == NULL) && (node->right != NULL)) || ((node->left != NULL) && (node->right == NULL));
}

/* 判断二叉搜索树度为0 */
static int balanceBinarySearchTreeNodeIsLeaf(AVLTreeNode *node)
{
    return (node->left == NULL) && (node->right == NULL);
}

/* 获取当前结点的前驱结点 */
/* 中序遍历到结点的前一个结点 */
static AVLTreeNode * bstreeNodePreDecessor(AVLTreeNode *node)
{
    if (node->left != NULL)
    {
        /* 前驱结点是在左子树的右子树的右子树... */
        AVLTreeNode *travelNode = node->left;
        while (travelNode->right != NULL)
        {
            travelNode = travelNode->right;
        }
        return travelNode;
    }

    /* 程序执行到这个地方 说明一定没有左子树。 那就需要向父结点找 */
    while(node->parent != NULL && node == node->parent->left)
    {
        node = node->parent;
    }
    /* node->parent == NULL. */
    /* node == node->parent->right. */
    return node->parent;
}

/* 删除结点之后要做的事 */
static int removeNodeAfter(balanceBinarySearchTree *pBstree, AVLTreeNode *node)
{
    /* 时间复杂度是O(logN) */
    while ( (node = node->parent) != NULL)
    {
        /* 程序执行到这里面的时候, 一定不止一个结点. */
        if (AVLTreeNodeIsBalanced(node))
        {
            /* 如果结点是平衡的. 那就更新高度. */
            AVLTreeNodeUpdateHeight(node);
        }
        else
        {
            /* node是最低不平衡结点 */
            /* 调整平衡 */
            AVLTreeNodeAdjustBalance(pBstree, node);
        }
    }
    return ON_SUCCESS;
}

/* 删除结点 */
int balanceBinarySearchTreeDeleteNode(balanceBinarySearchTree *pBstree, AVLTreeNode *node)
{
    if (node == NULL)
    {
        return NULL_PTR;
    }

    /* 树的结点减一 */
    (pBstree->size)--;
    if (pBstree->size == 0)
    {
        free(pBstree->root->data);
        pBstree->root->data = NULL;
        return ON_SUCCESS;
    }

    if (balanceBinarySearchTreeNodeHasTwochildrens(node))
    {
        /* 找到前驱结点 */
        AVLTreeNode * preNode = bstreeNodePreDecessor(node);
        node->data = preNode->data;
        node = preNode;
    }

    /* 程序执行到这里. 要删除的结点要么是度为1 要么是度为0. */

    /* 假设node结点是度为1的。它的child要么是左要么是右. */
    /* 假设node结点是度为0的, */
    AVLTreeNode * child = node->left != NULL ? node->left : node->right;

    AVLTreeNode *delNode = NULL;
    if (child)
    {
        /* 度为1 */
        child->parent = node->parent;
        if (node->parent == NULL)
        {
            /* 度为1 且 它是根结点 */
            pBstree->root = child;

            delNode = node;
            /* 删除的结点 */
            removeNodeAfter(pBstree, delNode);
        }
        else
        {
            /* 度为1 且 它不是根结点 */
            if (node == node->parent->left)
            {
                node->parent->left = child;
            }
            else if (node == node->parent->right)
            {
                node->parent->right = child;
            }

            delNode = node;
            /* 删除的结点 */
            removeNodeAfter(pBstree, delNode);
        }
    }
    else
    {
        /* 度为0 */
        if (node->parent == NULL)
        {
            /* 度为0 且是根结点 */
            delNode = node;
            /* 删除的结点 */
            removeNodeAfter(pBstree, delNode);
        }
        else
        {
            if (node == node->parent->left)
            {
                node->parent->left = NULL;
            }
            else if (node == node->parent->right)
            {
                node->parent->right = NULL;
            }


            delNode = node;
            /* 删除的结点 */
            removeNodeAfter(pBstree, delNode);
        }
       
    }

    if (delNode)
    {
        free(delNode);
        delNode = NULL;
    }
    
    return ON_SUCCESS;
}

/* 二叉搜索树的销毁 */
int balanceBinarySearchTreeDestroy(balanceBinarySearchTree *pBstree)
{
    if (pBstree == NULL)
    {
        return NULL_PTR;
    }

    queue *pQueue = NULL;
    queueInit(&pQueue);

    /* 将根结点入队 */
    queuePush(pQueue, pBstree->root);
    AVLTreeNode *travelNode = NULL;
    while (!queueIsEmpty(pQueue))
    {   
        queueGetHeadVal(pQueue, (void **)&travelNode);
        queuePop(pQueue);
        if (travelNode->left != NULL)
        {
            queuePush(pQueue, travelNode->left);
        }

        if (travelNode->right != NULL)
        {
            queuePush(pQueue, travelNode->right);
        }
        /* 最后释放 */
        if (travelNode)
        {
            free(travelNode->data);
            free(travelNode);
            travelNode = NULL;
        }
    }
    /* 释放队列 */  
    queueDestroy(pQueue);

    /* 释放树 */
    if (pBstree)
    {
        free(pBstree);
        pBstree = NULL;
    }
    return ON_SUCCESS;
}