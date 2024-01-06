#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructure.h"

#define true 1
#define false 0

/* 创造新的树结点 */
static AVLTreeNode *createBSTreeNewNode(ELEMENTTYPE val, AVLTreeNode *parent);
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
/* 按照姓名排序，依次显示信息 */
static int inOrderTravel(balanceBinarySearchTree *pBstree, AVLTreeNode *node);


/* 队列初始化 */
int doubleLinkListQueueInit(queue **pQueue)
{
    queue *list = (queue *)malloc(sizeof(queue) * 1);
    if (list == NULL)
    {
        return MALLOC_ERROR;
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
static AVLTreeNode *createBSTreeNewNode(ELEMENTTYPE val, AVLTreeNode *parent)
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

/* 中序遍历：按照姓名排序，依次显示信息 */
static int inOrderTravel(balanceBinarySearchTree *pBstree, AVLTreeNode *node)
{
    int ret = 0;
    if (node == NULL)
    {
        return ret;
    }
    /* 左子树 */
    inOrderTravel(pBstree, node->left);
    /* 根结点 */
    printf("name:%s\tphoneNumer:%s\n", node->data->name, node->data->phoneNumber);

    /* 右子树 */
    inOrderTravel(pBstree, node->right);
    return ON_SUCCESS;
}

/* 按照姓名排序，显示所有联系人 */
int displayAllContactInfo(balanceBinarySearchTree *pBstree)
{
    int ret = 0;
    inOrderTravel(pBstree, pBstree->root);
    return ret;
}

/* 测试插入功能 */
int main(int argc, char const *argv[])
{
    struct contacts *iam1 = (contacts*)malloc(sizeof(contacts));
    struct contacts *iam2 = (contacts*)malloc(sizeof(contacts));;
    struct contacts *iam3 = (contacts*)malloc(sizeof(contacts));;
    balanceBinarySearchTree *dataTree;
    balanceBinarySearchTreeInit(&dataTree, compareFunc);

    // printf("输入name1\n");
    // scanf("%s", iam1->name);
    // printf("输入number1\n");
    // scanf("%s", iam1->phoneNumber);    
    // printf("输入name2\n");
    // scanf("%s", iam2->name);
    // printf("输入number2\n");
    // scanf("%s", iam2->phoneNumber);

    strcpy(iam1->name, "1");
    strcpy(iam1->phoneNumber, "11111");     

    strcpy(iam2->name, "2");
    strcpy(iam2->phoneNumber, "22222");

    strcpy(iam3->name, "3");
    strcpy(iam3->phoneNumber, "33333");


    balanceBinarySearchTreeInsert(dataTree, iam1); 
    balanceBinarySearchTreeInsert(dataTree, iam2); 
    balanceBinarySearchTreeInsert(dataTree, iam3); 
    
    displayAllContactInfo(dataTree);

    return 0;
}


