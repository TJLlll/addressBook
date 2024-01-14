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

/* 队列初始化 */
static int doubleLinkListQueueInit(queue **pQueue);
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
/* 中序遍历的算法 按照姓名为树结点排序 */
static int inOrderTravel(AVLTreeNode *root);
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
static int queueInit(queue **pQueue)
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
    /* 队列的长度为0 */
    list->len = 0;

    /* 将pQueue指针指向初始化后的队列list */
    *pQueue = list;
    return ON_SUCCESS;
}

/* 新建队列的结点 */
static queueNode * newQueueNode(AVLTreeNode *pTreeNode)
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

    /* 赋值 */
    newNode->data = pTreeNode;

    /* 返回新结点 */
    return newNode;
}

/* 入队 */
static int queuePush(queue * pQueue, AVLTreeNode *pTreeNode)
{
    if (pQueue == NULL)
    {
        return NULL_PTR;
    }
    
    /* 新建新结点封装成函数. */
    queueNode * newNode = newQueueNode(pTreeNode);
    if (newNode == NULL)
    {
        return NULL_PTR;
    }
    newNode->data = 0;
    newNode->next = NULL;

    newNode->data = pTreeNode;
    
    queueNode * travelNode = pQueue->head->next;
    while (travelNode->next != NULL)
    {
        travelNode = travelNode->next;
    }
    
    travelNode->data = pTreeNode;

    /* 更新队列的长度 */
    (pQueue->len)++;

    return ON_SUCCESS;
}

/* 获取队头位置的数据 */
static int queueGetHeadVal(queue * pList, ELEMENTTYPE *pVal)
{
    if (pList == NULL)
    {
        return NULL_PTR;
    }
    if (pVal)
    {
        *pVal = pList->head->data;
    }
    return ON_SUCCESS;
}

/* 出队 */
static int queuePop(queue * pList) 
{
    if (pList == NULL)
    {
        return NULL_PTR;
    }
    queueNode * needDelNode = pList->head;

    /* 释放内存 */
    if (needDelNode != NULL)
    {
        free(needDelNode);
        needDelNode = NULL;
    }

    /* 队列长度减一 */
    (pList->len)--;
    return ON_SUCCESS;
}

/* 队列的销毁 */
static int queueDestroy(queue * pList)
{
    /* 释放队列 */
    while (pList->len != 0)
    {
        if (pList == NULL)
        {
            return NULL_PTR;
        }
    
        queueNode * travelNode = pList->head;
        queueNode * needDelNode = NULL;

        /* 释放内存 */
        if (needDelNode != NULL)
        {
            free(needDelNode);
            needDelNode = NULL;
        }

        /* 链表长度减一 */
        (pList->len)--;
    }

    if (pList->head != NULL)
    {
        free(pList->head);
        /* 指针置为NULL. */
        pList->head = NULL;
    }
    return ON_SUCCESS;
}

/* 函数比较器 */
int compareFunc(contacts *arg1, AVLTreeNode *arg2)
{
    char *val1 = arg1->name;
    char *val2 = arg2->data->name;

    return strcmp(val1, val2);
}

/* 平衡二叉搜索树初始化 */
int balanceBinarySearchTreeInit(balanceBinarySearchTree **pBstree, int (*compareFunc)(contacts *arg1, AVLTreeNode *arg2))
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
AVLTreeNode *createBSTreeNewNode(ELEMENTTYPE val, AVLTreeNode *parent)
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
        cmp = pBstree->compareFunc(newContacts, travelNode);

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
static int inOrderTravel(AVLTreeNode *root)
{
    if (root == 0)
    {
        return NULL_PTR;
    }
    AVLTreeNode *travelNode = root;
    /* 左子树 */
    inOrderTravel(travelNode->left);
    /* 根结点 */
    //printf("name:%s\tphoneNumer:%s\n", node->data->name, node->data->phoneNumber);
    printThisData(travelNode);    //打印本结点的数据
    /* 右子树 */
    inOrderTravel(travelNode->right);
    return ON_SUCCESS;
}

/* 按照姓名排序，显示所有联系人 */
int displayAllContactInfo(balanceBinarySearchTree *pBstree)
{
    printBackground();

    x = X_INITPOS;
    cont = 0;
    printf("\033[?25l");    //隐藏光标
    inOrderTravel(pBstree->root);
    printf("\033[0;0;0m");
    printf("\033[?25h");    //显示光标


    int key = 0;
    while (1)
    {
        key = returnKey();

        if (key == ESC || key == ENTER)
        {
            return ON_SUCCESS;
        }
        if (key == 's' || key == 'w')
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
void printThisData(AVLTreeNode *node)
{
    int y = Y_INITPOS;
    printf("\033[%d;10H\033[0;30;47m", x + cont);   //设置光标位置显示“姓名”
    printf("\033[3;30;47m姓名：");
    printf("\033[0;30;47m %s   ", node->data->name);
    //printf("\033[3;24H");
    //printf("\033[3;37;40m详情");        //选择这个
    //printf("\n\t\033[0;30;47m|\033[2;30;47m     ˉˉˉˉˉˉˉˉˉˉˉˉ \033[0;30;47m|\033[0;0;0m\n");

    printf("\033[%d;10H\033[0;30;47m", x + cont + 1);   //设置光标位置显示“号码”
    printf("\033[3;30;47m号码：");
    printf("\033[0;30;47m %s   ", node->data->phoneNumber);
   // printf("\n\t\033[0;30;47m|\033[2;30;47m     ˉˉˉˉˉˉˉˉˉˉˉˉ \033[0;30;47m|\033[0;0;0m\n");

    x = x + 2;
    cont++;
    printf("\033[0m\n");

    if (cont == 3)
    {
        int key = 0;
        while (1)
        {
            key = returnKey();
            if (key == ESC || key == ENTER)
            {
                break;
            }
            if (key == 's')
            {
                printf("\033[0;0;0m\n");
                printBackground();
                x = X_INITPOS;
                cont = 0;
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
            #if 0
            if (node)
            {
                free(node);
                node = NULL;
            }
            #endif
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
    while (pQueue->len != 0)
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

