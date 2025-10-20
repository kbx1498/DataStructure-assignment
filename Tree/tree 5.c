#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   // time

#define NUM_DATA 1000
#define SEARCH_KEYS 1000
#define MAX_VAL 10001

// ����ü ����
// ============================================================
typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// ��ƿ �Լ�
// ============================================================
int getHeight(Node* n) {
    return (n == NULL) ? 0 : n->height;
}
int mymax(int a, int b) { // �̸�: mymax
    return (a > b) ? a : b;
}

Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

// ȸ�� �Լ� (AVL ����)
// ============================================================
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = mymax(getHeight(y->left), getHeight(y->right)) + 1; // max -> mymax
    x->height = mymax(getHeight(x->left), getHeight(x->right)) + 1; // max -> mymax

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = mymax(getHeight(x->left), getHeight(x->right)) + 1; // max -> mymax
    y->height = mymax(getHeight(y->left), getHeight(y->right)) + 1; // max -> mymax

    return y;
}

int getBalance(Node* n) {
    return (n == NULL) ? 0 : getHeight(n->left) - getHeight(n->right);
}

// ���� �Լ� (BST, AVL)
// ============================================================
Node* insertBST(Node* node, int key) {
    if (node == NULL)
        return newNode(key);
    if (key < node->key)
        node->left = insertBST(node->left, key);
    else if (key > node->key)
        node->right = insertBST(node->right, key);
    return node; // �ߺ� ����
}

Node* insertAVL(Node* node, int key) {
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insertAVL(node->left, key);
    else if (key > node->key)
        node->right = insertAVL(node->right, key);
    else
        return node;

    node->height = 1 + mymax(getHeight(node->left), getHeight(node->right)); // max -> mymax
    int balance = getBalance(node);

    // LL
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
    // RR
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    // LR
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // RL
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Ž�� �Լ� (�� Ƚ�� ��ȯ)
// ============================================================
long long searchArray(int arr[], int n, int key) {
    long long comparisons = 0;
    for (int i = 0; i < n; i++) {
        comparisons++;
        if (arr[i] == key)
            break;
    }
    return comparisons; // ���� �� 1000ȸ
}

long long searchBST(Node* root, int key) {
    long long comparisons = 0;
    Node* current = root;
    while (current != NULL) {
        comparisons++;
        if (key == current->key)
            break;
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return comparisons;
}

long long searchAVL(Node* root, int key) {
    return searchBST(root, key); // ������ ����
}


// ������ ���� �Լ�
// ============================================================
void generateData1(int arr[]) { // ������ (�ߺ� X)
    int* used = (int*)calloc(MAX_VAL, sizeof(int));
    if (!used) {
        printf("�޸� ����\n");
        exit(1);
    }
    int count = 0;
    while (count < NUM_DATA) {
        int r = rand() % MAX_VAL;
        if (!used[r]) {
            used[r] = 1;
            arr[count++] = r;
        }
    }
    free(used);
}

void generateData2(int arr[]) { // ��������
    for (int i = 0; i < NUM_DATA; i++)
        arr[i] = i;
}

void generateData3(int arr[]) { // ��������
    for (int i = 0; i < NUM_DATA; i++)
        arr[i] = 999 - i;
}

void generateData4(int arr[]) { // ���� ���
    for (int i = 0; i < NUM_DATA; i++)
        arr[i] = i * (i % 2 + 2);
}

void generateSearchKeys(int keys[]) {
    for (int i = 0; i < SEARCH_KEYS; i++) {
        keys[i] = rand() % MAX_VAL; // 0~10000 ������ ���ο� ���� ����
    }
}

// Ʈ�� ���� �Լ�
// ============================================================
void freeTree(Node* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// ���� �Լ�
// ============================================================
typedef void (*DataGenerator)(int*);

int main() {
    srand((unsigned int)time(NULL));

    DataGenerator generators[] = { generateData1, generateData2, generateData3, generateData4 };
    int data[NUM_DATA];
    int searchKeys[SEARCH_KEYS];

    for (int i = 0; i < 4; i++) {
        Node* bstRoot = NULL;
        Node* avlRoot = NULL;

        generators[i](data); // ������ ����

        for (int j = 0; j < NUM_DATA; j++) {
            bstRoot = insertBST(bstRoot, data[j]);
            avlRoot = insertAVL(avlRoot, data[j]);
        }

        generateSearchKeys(searchKeys);

        long long totalArrayComps = 0, totalBSTComps = 0, totalAVLComps = 0;

        for (int j = 0; j < SEARCH_KEYS; j++) {
            int key = searchKeys[j];
            totalArrayComps += searchArray(data, NUM_DATA, key);
            totalBSTComps += searchBST(bstRoot, key);
            totalAVLComps += searchAVL(avlRoot, key);
        }

        printf("--- ������ (%d) ��� ---\n", i + 1);
        printf("Array: ������ (%d)���� ��� %.2fȸ Ž��\n", i + 1, (double)totalArrayComps / SEARCH_KEYS);
        printf("BST:   ������ (%d)���� ��� %.2fȸ Ž��\n", i + 1, (double)totalBSTComps / SEARCH_KEYS);
        printf("AVL:   ������ (%d)���� ��� %.2fȸ Ž��\n\n", i + 1, (double)totalAVLComps / SEARCH_KEYS);

        freeTree(bstRoot);
        freeTree(avlRoot);
    }

    return 0;
}