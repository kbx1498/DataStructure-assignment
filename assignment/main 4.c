#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createNode(int data);
TreeNode* insert(TreeNode* root, int data);
void linearSearch(int arr[], int size, int key, int* count);
TreeNode* searchBST(TreeNode* root, int key, int* count);
void freeTree(TreeNode* root);

int main() {
    srand(time(NULL)); 
    int numbers[100];
    TreeNode* root = NULL; // 루트노드 초기 NULL
    const int SIZE = 100;
    const int MAX_NUM = 1001;

    printf("1. 데이터 생성 및 저장\n");

    // 1. 0~1000 사이 숫자 100개 배열, 트리 저장
    printf("생성된 난수: \n");
    for (int i = 0; i < SIZE; i++) {
        int num = rand() % MAX_NUM;
        numbers[i] = num;       // 배열에 저장함.
        root = insert(root, num); // 이진 탐색 트리에 저장함.
        printf("%d ", num);
        if ((i + 1) % 20 == 0) { // 20개 출력 시마다 줄바꿈
            printf("\n");
        }
    }
    printf("\n----------------------------------------\n\n");

    // 2. 임의의 숫자(배열의 50번째 요소)를 검색 대상으로 선택함.
    int searchKey = numbers[50];
    printf("2. 탐색 시작 (탐색할 숫자: %d) \n", searchKey);

    // 3. 선형 탐색을 수행함.
    int linear_search_count = 0;
    linearSearch(numbers, SIZE, searchKey, &linear_search_count);

    // 4. 이진 탐색 트리 탐색을 수행함.
    int bst_search_count = 0;
    TreeNode* foundNode = searchBST(root, searchKey, &bst_search_count);

    printf("----------------------------------------\n\n");
    printf("3. 탐색 결과 비교\n");
    printf("탐색 대상 숫자: %d\n", searchKey);
    printf("1. 선형 탐색 (배열)\n");
    printf("   - 비교 횟수: %d\n", linear_search_count);

    printf("2. 이진 탐색 (트리)\n");
    if (foundNode != NULL) {
        printf("   - 비교 횟수: %d\n", bst_search_count);
    }
    else {
        printf("   - 트리에서 숫자를 찾지 못했음.\n");
    }
    printf("\n----------------------------------------\n\n");
    // 메모리 해제
    freeTree(root);

    return 0;
}

TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        printf("메모리 할당 오류 발생. 프로그램 종료함.\n");
        exit(1);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// 이진 탐색 트리에 데이터를 삽입하는 재귀 함수임.
TreeNode* insert(TreeNode* root, int data) {
    // 트리가 비어있으면 새로운 노드를 생성하여 루트로 반환함.
    if (root == NULL) {
        return createNode(data);
    }
    // 데이터가 현재 노드보다 작으면 왼쪽 서브트리로 이동하여 삽입 진행함.
    if (data < root->data) {
        root->left = insert(root->left, data);
    }
    // 데이터가 현재 노드보다 크면 오른쪽 서브트리로 이동하여 삽입 진행함. (중복값은 무시됨)
    else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

// 배열에서 선형 탐색을 수행하는 함수임.
void linearSearch(int arr[], int size, int key, int* count) {
    *count = 0; // 비교 횟수 초기화함.
    for (int i = 0; i < size; i++) {
        (*count)++; // 비교 연산을 수행했으므로 카운트 1 증가시킴.
        if (arr[i] == key) {
            printf("[선형 탐색] %d번째 위치에서 %d를 찾았음.\n", i + 1, key);
            return; // 찾았으므로 함수를 즉시 종료함.
        }
    }
    printf("[선형 탐색] 배열에서 %d를 찾지 못했음.\n", key);
}

// 이진 탐색 트리에서 데이터를 검색하는 재귀 함수임.
TreeNode* searchBST(TreeNode* root, int key, int* count) {
    (*count)++; // 노드를 방문하여 비교하므로, 카운트를 1 증가시킴.

    // 현재 노드가 NULL이거나 데이터를 찾았으면 탐색을 종료함.
    if (root == NULL || root->data == key) {
        if (root != NULL) {
            printf("[이진 탐색] 트리에서 %d를 찾았음.\n", key);
        }
        else {
            printf("[이진 탐색] 트리에서 %d를 찾지 못했음.\n", key);
        }
        return root;
    }

    // 찾으려는 키가 더 작으므로 왼쪽 서브트리를 탐색함.
    if (key < root->data) {
        return searchBST(root->left, key, count);
    }
    // 찾으려는 키가 더 크므로 오른쪽 서브트리를 탐색함.
    else {
        return searchBST(root->right, key, count);
    }
}

// 트리 전체의 메모리를 해제
void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}