#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_GENDER_LEN 10
#define MAX_LINE_LEN 1024

// 학생 구조체 정의
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender[MAX_GENDER_LEN];
    int korean;
    int english;
    int math;
} Student;

// 비교 횟수 전역 변수
long long global_comparisons = 0;

// [1] 비정렬 배열 (Unsorted Array) 구현
typedef struct {
    Student* data;
    int size;
    int capacity;
} UnsortedArray;

void initUnsortedArray(UnsortedArray* arr, int capacity) {
    arr->data = (Student*)malloc(sizeof(Student) * capacity);
    arr->size = 0;
    arr->capacity = capacity;
}

void insertUnsorted(UnsortedArray* arr, Student s) {
    if (arr->size < arr->capacity) {
        arr->data[arr->size++] = s;
    }
}

int searchUnsorted(UnsortedArray* arr, int targetId) {
    global_comparisons = 0;
    for (int i = 0; i < arr->size; i++) {
        global_comparisons++;
        if (arr->data[i].id == targetId) return i;
    }
    return -1;
}

void deleteUnsorted(UnsortedArray* arr, int targetId) {
    int idx = searchUnsorted(arr, targetId);
    if (idx != -1) {
        for (int i = idx; i < arr->size - 1; i++) {
            arr->data[i] = arr->data[i + 1];
        }
        arr->size--;
    }
}

// [2] 정렬된 배열 (Sorted Array) 구현
typedef struct {
    Student* data;
    int size;
    int capacity;
} SortedArray;

void initSortedArray(SortedArray* arr, int capacity) {
    arr->data = (Student*)malloc(sizeof(Student) * capacity);
    arr->size = 0;
    arr->capacity = capacity;
}

int binarySearch(SortedArray* arr, int targetId, int* found) {
    int left = 0;
    int right = arr->size - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        global_comparisons++;
        if (arr->data[mid].id == targetId) {
            *found = 1;
            return mid;
        }
        else if (arr->data[mid].id < targetId) left = mid + 1;
        else right = mid - 1;
    }
    *found = 0;
    return left;
}

void insertSorted(SortedArray* arr, Student s) {
    if (arr->size >= arr->capacity) return;
    int found = 0;
    int pos = binarySearch(arr, s.id, &found);
    for (int i = arr->size; i > pos; i--) {
        arr->data[i] = arr->data[i - 1];
    }
    arr->data[pos] = s;
    arr->size++;
}

int searchSorted(SortedArray* arr, int targetId) {
    global_comparisons = 0;
    int found = 0;
    int idx = binarySearch(arr, targetId, &found);
    if (found) return idx;
    return -1;
}

void deleteSorted(SortedArray* arr, int targetId) {
    global_comparisons = 0;
    int found = 0;
    int idx = binarySearch(arr, targetId, &found);
    if (found) {
        for (int i = idx; i < arr->size - 1; i++) {
            arr->data[i] = arr->data[i + 1];
        }
        arr->size--;
    }
}

// [3] AVL Tree 구현
typedef struct AVLNode {
    Student data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

int getHeight(AVLNode* n) { return (n == NULL) ? 0 : n->height; }
int maxVal(int a, int b) { return (a > b) ? a : b; }

AVLNode* newAVLNode(Student s) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->data = s;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = maxVal(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = maxVal(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = maxVal(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = maxVal(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

int getBalance(AVLNode* N) { return (N == NULL) ? 0 : getHeight(N->left) - getHeight(N->right); }

AVLNode* insertAVL(AVLNode* node, Student s) {
    if (node == NULL) return newAVLNode(s);
    global_comparisons++;
    if (s.id < node->data.id) node->left = insertAVL(node->left, s);
    else if (s.id > node->data.id) node->right = insertAVL(node->right, s);
    else return node;

    node->height = 1 + maxVal(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && s.id < node->left->data.id) return rightRotate(node);
    if (balance < -1 && s.id > node->right->data.id) return leftRotate(node);
    if (balance > 1 && s.id > node->left->data.id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && s.id < node->right->data.id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

AVLNode* searchAVL(AVLNode* root, int targetId) {
    if (root == NULL) return NULL;
    global_comparisons++;
    if (targetId == root->data.id) return root;
    if (targetId < root->data.id) return searchAVL(root->left, targetId);
    return searchAVL(root->right, targetId);
}

AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL) current = current->left;
    return current;
}

AVLNode* deleteAVL(AVLNode* root, int targetId) {
    if (root == NULL) return root;
    global_comparisons++;
    if (targetId < root->data.id) root->left = deleteAVL(root->left, targetId);
    else if (targetId > root->data.id) root->right = deleteAVL(root->right, targetId);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (temp == NULL) { temp = root; root = NULL; }
            else *root = *temp;
            free(temp);
        }
        else {
            AVLNode* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteAVL(root->right, temp->data.id);
        }
    }
    if (root == NULL) return root;
    root->height = 1 + maxVal(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

void freeAVL(AVLNode* root) {
    if (root == NULL) return;
    freeAVL(root->left);
    freeAVL(root->right);
    free(root);
}

// ==========================================
// Main Function
// ==========================================
int main() {
    FILE* fp = fopen("dataset_id_ascending.csv", "r");
    if (fp == NULL) {
        printf("Error: dataset_id_ascending.csv 파일을 찾을 수 없습니다.\n");
        return 1;
    }

    int max_capacity = 40000;
    Student* raw_data = (Student*)malloc(sizeof(Student) * max_capacity);
    int count = 0;

    char line[MAX_LINE_LEN];
    fgets(line, MAX_LINE_LEN, fp); // 헤더 스킵

    while (fgets(line, MAX_LINE_LEN, fp) && count < max_capacity) {
        Student s;
        sscanf(line, "%d,%[^,],%[^,],%d,%d,%d",
            &s.id, s.name, s.gender, &s.korean, &s.english, &s.math);
        raw_data[count++] = s;
    }
    fclose(fp);
    printf("데이터 로드 완료: %d개 레코드\n\n", count);

    int search_target_id = 116000;
    int delete_target_id = raw_data[count - 1].id;

    if (search_target_id < raw_data[0].id || search_target_id > raw_data[count - 1].id) {
        search_target_id = raw_data[count / 2].id;
    }

    // 1. Unsorted Array 테스트
    printf("=== 1. 비정렬 배열 (Unsorted Array) ===\n");
    UnsortedArray uArr;
    initUnsortedArray(&uArr, max_capacity);

    global_comparisons = 0;
    for (int i = 0; i < count; i++) insertUnsorted(&uArr, raw_data[i]);
    printf(" [삽입] 전체 %d개 데이터 삽입 완료 (비교 횟수: %lld)\n", count, global_comparisons);

    global_comparisons = 0;
    deleteUnsorted(&uArr, delete_target_id);
    printf(" [삭제] ID %d 삭제 완료 (비교 횟수: %lld)\n", delete_target_id, global_comparisons);

    global_comparisons = 0;
    int idx = searchUnsorted(&uArr, search_target_id);
    printf(" [검색] ID %d 검색 결과 인덱스: %d (비교 횟수: %lld)\n\n", search_target_id, idx, global_comparisons);

    free(uArr.data);


    // 2. Sorted Array 테스트
    printf("=== 2. 정렬된 배열 (Sorted Array) ===\n");
    SortedArray sArr;
    initSortedArray(&sArr, max_capacity);

    global_comparisons = 0;
    for (int i = 0; i < count; i++) insertSorted(&sArr, raw_data[i]);
    printf(" [삽입] 전체 %d개 데이터 삽입 완료 (비교 횟수: %lld)\n", count, global_comparisons);

    global_comparisons = 0;
    deleteSorted(&sArr, delete_target_id);
    printf(" [삭제] ID %d 삭제 완료 (비교 횟수: %lld)\n", delete_target_id, global_comparisons);

    global_comparisons = 0;
    idx = searchSorted(&sArr, search_target_id);
    printf(" [검색] ID %d 검색 결과 인덱스: %d (비교 횟수: %lld)\n\n", search_target_id, idx, global_comparisons);

    free(sArr.data);


    // 3. AVL Tree 테스트
    printf("=== 3. AVL Tree ===\n");
    AVLNode* root = NULL;

    global_comparisons = 0;
    for (int i = 0; i < count; i++) root = insertAVL(root, raw_data[i]);
    printf(" [삽입] 전체 %d개 데이터 삽입 완료 (비교 횟수: %lld)\n", count, global_comparisons);

    global_comparisons = 0;
    root = deleteAVL(root, delete_target_id);
    printf(" [삭제] ID %d 삭제 완료 (비교 횟수: %lld)\n", delete_target_id, global_comparisons);

    global_comparisons = 0;
    AVLNode* res = searchAVL(root, search_target_id);
    printf(" [검색] ID %d 검색 결과: %s (비교 횟수: %lld)\n", search_target_id, (res ? "성공" : "실패"), global_comparisons);

    printf("\n종료\n");

    freeAVL(root);
    free(raw_data);

    return 0;
}