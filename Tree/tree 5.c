#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   // time

#define NUM_DATA 1000
#define SEARCH_KEYS 1000
#define MAX_VAL 10001

// 구조체 정의
// ============================================================
typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

// 유틸 함수
// ============================================================
int getHeight(Node* n) {
    return (n == NULL) ? 0 : n->height;
}
int mymax(int a, int b) { // 이름: mymax
    return (a > b) ? a : b;
}

Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

// 회전 함수 (AVL 전용)
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

// 삽입 함수 (BST, AVL)
// ============================================================
Node* insertBST(Node* node, int key) {
    if (node == NULL)
        return newNode(key);
    if (key < node->key)
        node->left = insertBST(node->left, key);
    else if (key > node->key)
        node->right = insertBST(node->right, key);
    return node; // 중복 무시
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

// 탐색 함수 (비교 횟수 반환)
// ============================================================
long long searchArray(int arr[], int n, int key) {
    long long comparisons = 0;
    for (int i = 0; i < n; i++) {
        comparisons++;
        if (arr[i] == key)
            break;
    }
    return comparisons; // 실패 시 1000회
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
    return searchBST(root, key); // 동일한 구조
}


// 데이터 생성 함수
// ============================================================
void generateData1(int arr[]) { // 무작위 (중복 X)
    int* used = (int*)calloc(MAX_VAL, sizeof(int));
    if (!used) {
        printf("메모리 오류\n");
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

void generateData2(int arr[]) { // 오름차순
    for (int i = 0; i < NUM_DATA; i++)
        arr[i] = i;
}

void generateData3(int arr[]) { // 내림차순
    for (int i = 0; i < NUM_DATA; i++)
        arr[i] = 999 - i;
}

void generateData4(int arr[]) { // 수식 기반
    for (int i = 0; i < NUM_DATA; i++)
        arr[i] = i * (i % 2 + 2);
}

void generateSearchKeys(int keys[]) {
    for (int i = 0; i < SEARCH_KEYS; i++) {
        keys[i] = rand() % MAX_VAL; // 0~10000 사이의 새로운 난수 생성
    }
}

// 트리 해제 함수
// ============================================================
void freeTree(Node* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// 메인 함수
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

        generators[i](data); // 데이터 생성

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

        printf("--- 데이터 (%d) 결과 ---\n", i + 1);
        printf("Array: 데이터 (%d)에서 평균 %.2f회 탐색\n", i + 1, (double)totalArrayComps / SEARCH_KEYS);
        printf("BST:   데이터 (%d)에서 평균 %.2f회 탐색\n", i + 1, (double)totalBSTComps / SEARCH_KEYS);
        printf("AVL:   데이터 (%d)에서 평균 %.2f회 탐색\n\n", i + 1, (double)totalAVLComps / SEARCH_KEYS);

        freeTree(bstRoot);
        freeTree(avlRoot);
    }

    return 0;
}