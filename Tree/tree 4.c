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
    TreeNode* root = NULL; // ��Ʈ��� �ʱ� NULL
    const int SIZE = 100;
    const int MAX_NUM = 1001;

    printf("1. ������ ���� �� ����\n");

    // 1. 0~1000 ���� ���� 100�� �迭, Ʈ�� ����
    printf("������ ����: \n");
    for (int i = 0; i < SIZE; i++) {
        int num = rand() % MAX_NUM;
        numbers[i] = num;       // �迭�� ������.
        root = insert(root, num); // ���� Ž�� Ʈ���� ������.
        printf("%d ", num);
        if ((i + 1) % 20 == 0) { // 20�� ��� �ø��� �ٹٲ�
            printf("\n");
        }
    }
    printf("\n----------------------------------------\n\n");

    // 2. ������ ����(�迭�� 50��° ���)�� �˻� ������� ������.
    int searchKey = numbers[50];
    printf("2. Ž�� ���� (Ž���� ����: %d) \n", searchKey);

    // 3. ���� Ž���� ������.
    int linear_search_count = 0;
    linearSearch(numbers, SIZE, searchKey, &linear_search_count);

    // 4. ���� Ž�� Ʈ�� Ž���� ������.
    int bst_search_count = 0;
    TreeNode* foundNode = searchBST(root, searchKey, &bst_search_count);

    printf("----------------------------------------\n\n");
    printf("3. Ž�� ��� ��\n");
    printf("Ž�� ��� ����: %d\n", searchKey);
    printf("1. ���� Ž�� (�迭)\n");
    printf("   - �� Ƚ��: %d\n", linear_search_count);

    printf("2. ���� Ž�� (Ʈ��)\n");
    if (foundNode != NULL) {
        printf("   - �� Ƚ��: %d\n", bst_search_count);
    }
    else {
        printf("   - Ʈ������ ���ڸ� ã�� ������.\n");
    }
    printf("\n----------------------------------------\n\n");
    // �޸� ����
    freeTree(root);

    return 0;
}

TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        printf("�޸� �Ҵ� ���� �߻�. ���α׷� ������.\n");
        exit(1);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// ���� Ž�� Ʈ���� �����͸� �����ϴ� ��� �Լ���.
TreeNode* insert(TreeNode* root, int data) {
    // Ʈ���� ��������� ���ο� ��带 �����Ͽ� ��Ʈ�� ��ȯ��.
    if (root == NULL) {
        return createNode(data);
    }
    // �����Ͱ� ���� ��庸�� ������ ���� ����Ʈ���� �̵��Ͽ� ���� ������.
    if (data < root->data) {
        root->left = insert(root->left, data);
    }
    // �����Ͱ� ���� ��庸�� ũ�� ������ ����Ʈ���� �̵��Ͽ� ���� ������. (�ߺ����� ���õ�)
    else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

// �迭���� ���� Ž���� �����ϴ� �Լ���.
void linearSearch(int arr[], int size, int key, int* count) {
    *count = 0; // �� Ƚ�� �ʱ�ȭ��.
    for (int i = 0; i < size; i++) {
        (*count)++; // �� ������ ���������Ƿ� ī��Ʈ 1 ������Ŵ.
        if (arr[i] == key) {
            printf("[���� Ž��] %d��° ��ġ���� %d�� ã����.\n", i + 1, key);
            return; // ã�����Ƿ� �Լ��� ��� ������.
        }
    }
    printf("[���� Ž��] �迭���� %d�� ã�� ������.\n", key);
}

// ���� Ž�� Ʈ������ �����͸� �˻��ϴ� ��� �Լ���.
TreeNode* searchBST(TreeNode* root, int key, int* count) {
    (*count)++; // ��带 �湮�Ͽ� ���ϹǷ�, ī��Ʈ�� 1 ������Ŵ.

    // ���� ��尡 NULL�̰ų� �����͸� ã������ Ž���� ������.
    if (root == NULL || root->data == key) {
        if (root != NULL) {
            printf("[���� Ž��] Ʈ������ %d�� ã����.\n", key);
        }
        else {
            printf("[���� Ž��] Ʈ������ %d�� ã�� ������.\n", key);
        }
        return root;
    }

    // ã������ Ű�� �� �����Ƿ� ���� ����Ʈ���� Ž����.
    if (key < root->data) {
        return searchBST(root->left, key, count);
    }
    // ã������ Ű�� �� ũ�Ƿ� ������ ����Ʈ���� Ž����.
    else {
        return searchBST(root->right, key, count);
    }
}

// Ʈ�� ��ü�� �޸𸮸� ����
void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}