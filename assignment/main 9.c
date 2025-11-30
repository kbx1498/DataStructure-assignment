#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

 // [0] 구조체 및 데이터 로드
#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
} Student;

long long compare_count = 0;

typedef enum { CRIT_ID, CRIT_NAME, CRIT_GENDER, CRIT_GRADE } Criteria;
typedef enum { ORDER_ASC, ORDER_DESC } Order;

Student* load_students(const char* filename, int* out_count) {
    FILE* fp = NULL;
    if (fopen_s(&fp, filename, "r") != 0 || !fp) {
        printf("[오류] 파일 '%s'을(를) 찾을 수 없습니다.\n", filename);
        return NULL;
    }

    char line[MAX_LINE_LEN];
    int capacity = 1000;
    int count = 0;

    Student* arr = (Student*)malloc(sizeof(Student) * capacity);
    if (!arr) { fclose(fp); return NULL; }

    fgets(line, sizeof(line), fp); 

    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* temp = (Student*)realloc(arr, sizeof(Student) * capacity);
            if (!temp) { free(arr); fclose(fp); return NULL; }
            arr = temp;
        }

        Student s;
        char* context = NULL;
        char* token = strtok_s(line, ",", &context);
        if (token) s.id = atoi(token);

        token = strtok_s(NULL, ",", &context);
        if (token) strncpy_s(s.name, sizeof(s.name), token, _TRUNCATE);

        token = strtok_s(NULL, ",", &context);
        if (token) s.gender = token[0];

        token = strtok_s(NULL, ",", &context);
        if (token) s.korean = atoi(token);
        token = strtok_s(NULL, ",", &context);
        if (token) s.english = atoi(token);
        token = strtok_s(NULL, ",", &context);
        if (token) s.math = atoi(token);

        arr[count++] = s;
    }
    fclose(fp);
    *out_count = count;
    return arr;
}

int compare(const Student* a, const Student* b, Criteria crit, Order ord) {
    compare_count++;
    int res = 0;
    switch (crit) {
    case CRIT_ID: res = a->id - b->id; break;
    case CRIT_NAME: res = strcmp(a->name, b->name); break;
    case CRIT_GENDER: res = a->gender - b->gender; break;
    case CRIT_GRADE: {
        int sumA = a->korean + a->english + a->math;
        int sumB = b->korean + b->english + b->math;
        if (sumA != sumB) res = sumA - sumB;
        else {
            compare_count++;
            if (a->korean != b->korean) res = a->korean - b->korean;
            else {
                compare_count++;
                if (a->english != b->english) res = a->english - b->english;
                else {
                    compare_count++;
                    res = a->math - b->math;
                }
            }
        }
    } break;
    }
    return (ord == ORDER_ASC) ? res : -res;
}

void swap(Student* a, Student* b) {
    Student temp = *a; *a = *b; *b = temp;
}

// [Part A] 기본 정렬 알고리즘 9종
void bubble_sort(Student* arr, int n, Criteria crit, Order ord) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (compare(&arr[j], &arr[j + 1], crit, ord) > 0)
                swap(&arr[j], &arr[j + 1]);
}

void selection_sort(Student* arr, int n, Criteria crit, Order ord) {
    for (int i = 0; i < n - 1; i++) {
        int best = i;
        for (int j = i + 1; j < n; j++)
            if (compare(&arr[j], &arr[best], crit, ord) < 0) best = j;
        if (best != i) swap(&arr[i], &arr[best]);
    }
}

void insertion_sort(Student* arr, int n, Criteria crit, Order ord) {
    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;
        while (j >= 0 && compare(&arr[j], &key, crit, ord) > 0) {
            arr[j + 1] = arr[j]; j--;
        }
        arr[j + 1] = key;
    }
}

void shell_sort(Student* arr, int n, Criteria crit, Order ord) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j;
            for (j = i; j >= gap && compare(&arr[j - gap], &temp, crit, ord) > 0; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

// Quick (Standard) - 중간값 피벗으로 변경 (정렬된 데이터에서 스택오버플로우 방지)
void quick_sort_recur(Student* arr, int l, int r, Criteria crit, Order ord) {
    if (l >= r) return;

    // [패치] 정렬된 데이터 대응용 중간값 피벗
    int mid = (l + r) / 2;
    swap(&arr[l], &arr[mid]);

    int pivot = l, i = l + 1, j = r;
    while (i <= j) {
        while (i <= r && compare(&arr[i], &arr[pivot], crit, ord) <= 0) i++;
        while (j > l && compare(&arr[j], &arr[pivot], crit, ord) >= 0) j--;
        if (i > j) swap(&arr[j], &arr[pivot]); else swap(&arr[i], &arr[j]);
    }
    quick_sort_recur(arr, l, j - 1, crit, ord);
    quick_sort_recur(arr, j + 1, r, crit, ord);
}
void quick_sort(Student* arr, int n, Criteria crit, Order ord) {
    quick_sort_recur(arr, 0, n - 1, crit, ord);
}

// Heap (Standard)
void heapify(Student* arr, int n, int i, Criteria crit, Order ord) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && compare(&arr[l], &arr[largest], crit, ord) > 0) largest = l;
    if (r < n && compare(&arr[r], &arr[largest], crit, ord) > 0) largest = r;
    if (largest != i) {
        swap(&arr[i], &arr[largest]); heapify(arr, n, largest, crit, ord);
    }
}
void heap_sort(Student* arr, int n, Criteria crit, Order ord) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i, crit, ord);
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]); heapify(arr, i, 0, crit, ord);
    }
}

// Merge (Standard)
void merge(Student* arr, int l, int m, int r, Criteria crit, Order ord) {
    int n1 = m - l + 1, n2 = r - m;
    Student* L = (Student*)malloc(sizeof(Student) * n1);
    Student* R = (Student*)malloc(sizeof(Student) * n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (compare(&L[i], &R[j], crit, ord) <= 0) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}
void merge_sort_recur(Student* arr, int l, int r, Criteria crit, Order ord) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_recur(arr, l, m, crit, ord);
        merge_sort_recur(arr, m + 1, r, crit, ord);
        merge(arr, l, m, r, crit, ord);
    }
}
void merge_sort(Student* arr, int n, Criteria crit, Order ord) {
    merge_sort_recur(arr, 0, n - 1, crit, ord);
}

// Tree (BST - Standard) - [패치] 모든 과정을 반복문으로 구현 (재귀 제거)
typedef struct Node {
    Student data;
    struct Node* left, * right;
} Node;
Node* create_node(Student d) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->data = d; n->left = n->right = NULL; return n;
}

// 반복문 삽입
Node* insert_node(Node* root, Student d, Criteria crit, Order ord) {
    if (!root) return create_node(d);
    Node* current = root;
    while (1) {
        if (compare(&d, &current->data, crit, ord) < 0) {
            if (!current->left) { current->left = create_node(d); break; }
            current = current->left;
        }
        else {
            if (!current->right) { current->right = create_node(d); break; }
            current = current->right;
        }
    }
    return root;
}

// 반복문 중위 순회
void inorder(Node* root, Student* arr, int* idx) {
    if (!root) return;
    int capacity = 40000;
    Node** stack = (Node**)malloc(sizeof(Node*) * capacity);
    int top = -1;
    Node* curr = root;

    while (curr != NULL || top != -1) {
        while (curr != NULL) {
            stack[++top] = curr;
            curr = curr->left;
        }
        curr = stack[top--];
        arr[(*idx)++] = curr->data;
        curr = curr->right;
    }
    free(stack);
}

// 반복문 트리 해제
void free_tree(Node* root) {
    if (!root) return;
    int capacity = 40000;
    Node** stack = (Node**)malloc(sizeof(Node*) * capacity);
    int top = -1;
    stack[++top] = root;

    while (top >= 0) {
        Node* curr = stack[top--];
        if (curr->left) stack[++top] = curr->left;
        if (curr->right) stack[++top] = curr->right;
        free(curr);
    }
    free(stack);
}
void tree_sort(Student* arr, int n, Criteria crit, Order ord) {
    Node* root = NULL;
    for (int i = 0; i < n; i++) root = insert_node(root, arr[i], crit, ord);
    int idx = 0; inorder(root, arr, &idx); free_tree(root);
}

// Radix (ID Only)
void radix_sort(Student* arr, int n, Criteria crit, Order ord) {
    if (crit != CRIT_ID) return;
    Student* output = (Student*)malloc(sizeof(Student) * n);
    int max_id = arr[0].id;
    for (int i = 1; i < n; i++) if (arr[i].id > max_id) max_id = arr[i].id;
    for (int exp = 1; max_id / exp > 0; exp *= 10) {
        int count[10] = { 0 };
        for (int i = 0; i < n; i++) count[(arr[i].id / exp) % 10]++;
        if (ord == ORDER_ASC) {
            for (int i = 1; i < 10; i++) count[i] += count[i - 1];
            for (int i = n - 1; i >= 0; i--) { output[count[(arr[i].id / exp) % 10] - 1] = arr[i]; count[(arr[i].id / exp) % 10]--; }
        }
        else {
            for (int i = 8; i >= 0; i--) count[i] += count[i + 1];
            for (int i = n - 1; i >= 0; i--) { output[count[(arr[i].id / exp) % 10] - 1] = arr[i]; count[(arr[i].id / exp) % 10]--; }
        }
        for (int i = 0; i < n; i++) arr[i] = output[i];
    }
    free(output);
}


// [Part B] 최적화 알고리즘
// 1. Shell Sort Optimized (Tokuda Gap Sequence)
void shell_sort_opt(Student* arr, int n, Criteria crit, Order ord) {
    int gaps[] = { 1182, 525, 233, 103, 46, 20, 9, 4, 1 };
    int gap_count = sizeof(gaps) / sizeof(int);

    for (int g = 0; g < gap_count; g++) {
        int gap = gaps[g];
        if (gap >= n) continue;
        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j;
            for (j = i; j >= gap && compare(&arr[j - gap], &temp, crit, ord) > 0; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

// 2. Quick Sort Optimized (Hybrid: Median-3 + Insertion
void insertion_sort_range(Student* arr, int left, int right, Criteria crit, Order ord) {
    for (int i = left + 1; i <= right; i++) {
        Student key = arr[i];
        int j = i - 1;
        while (j >= left && compare(&arr[j], &key, crit, ord) > 0) {
            arr[j + 1] = arr[j]; j--;
        }
        arr[j + 1] = key;
    }
}
int get_median_idx(Student* arr, int l, int r, Criteria crit, Order ord) {
    int m = (l + r) / 2;
    if (compare(&arr[l], &arr[m], crit, ord) < 0) {
        if (compare(&arr[m], &arr[r], crit, ord) < 0) return m;
        else if (compare(&arr[l], &arr[r], crit, ord) < 0) return r;
        else return l;
    }
    else {
        if (compare(&arr[l], &arr[r], crit, ord) < 0) return l;
        else if (compare(&arr[m], &arr[r], crit, ord) < 0) return r;
        else return m;
    }
}
void quick_sort_hybrid_recur(Student* arr, int l, int r, Criteria crit, Order ord) {
    if (r - l + 1 <= 15) {
        insertion_sort_range(arr, l, r, crit, ord);
        return;
    }
    int pivotIdx = get_median_idx(arr, l, r, crit, ord);
    swap(&arr[l], &arr[pivotIdx]);
    int pivot = l, i = l + 1, j = r;
    while (i <= j) {
        while (i <= r && compare(&arr[i], &arr[pivot], crit, ord) <= 0) i++;
        while (j > l && compare(&arr[j], &arr[pivot], crit, ord) >= 0) j--;
        if (i > j) swap(&arr[j], &arr[pivot]); else swap(&arr[i], &arr[j]);
    }
    quick_sort_hybrid_recur(arr, l, j - 1, crit, ord);
    quick_sort_hybrid_recur(arr, j + 1, r, crit, ord);
}
void quick_sort_opt(Student* arr, int n, Criteria crit, Order ord) {
    quick_sort_hybrid_recur(arr, 0, n - 1, crit, ord);
}

// 3. Tree Sort Optimized (AVL Tree - 상세 구현)
typedef struct AVLNode {
    Student data; struct AVLNode* left, * right; int height;
} AVLNode;
int get_height(AVLNode* n) { return (n == NULL) ? 0 : n->height; }
int max_val(int a, int b) { return (a > b) ? a : b; }
AVLNode* create_avl(Student d) {
    AVLNode* n = (AVLNode*)malloc(sizeof(AVLNode));
    n->data = d; n->left = NULL; n->right = NULL; n->height = 1; return n;
}
AVLNode* r_rot(AVLNode* y) {
    AVLNode* x = y->left; AVLNode* T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max_val(get_height(y->left), get_height(y->right)) + 1;
    x->height = max_val(get_height(x->left), get_height(x->right)) + 1;
    return x;
}
AVLNode* l_rot(AVLNode* x) {
    AVLNode* y = x->right; AVLNode* T2 = y->left;
    y->left = x; x->right = T2;
    x->height = max_val(get_height(x->left), get_height(x->right)) + 1;
    y->height = max_val(get_height(y->left), get_height(y->right)) + 1;
    return y;
}
int get_bal(AVLNode* n) { return (n == NULL) ? 0 : get_height(n->left) - get_height(n->right); }
AVLNode* insert_avl(AVLNode* node, Student d, Criteria crit, Order ord) {
    if (!node) return create_avl(d);
    if (compare(&d, &node->data, crit, ord) < 0) node->left = insert_avl(node->left, d, crit, ord);
    else node->right = insert_avl(node->right, d, crit, ord);
    node->height = 1 + max_val(get_height(node->left), get_height(node->right));
    int bal = get_bal(node);
    if (bal > 1 && compare(&d, &node->left->data, crit, ord) < 0) return r_rot(node);
    if (bal < -1 && compare(&d, &node->right->data, crit, ord) > 0) return l_rot(node);
    if (bal > 1 && compare(&d, &node->left->data, crit, ord) > 0) { node->left = l_rot(node->left); return r_rot(node); }
    if (bal < -1 && compare(&d, &node->right->data, crit, ord) < 0) { node->right = r_rot(node->right); return l_rot(node); }
    return node;
}
void inorder_avl(AVLNode* root, Student* arr, int* idx) {
    if (root) { inorder_avl(root->left, arr, idx); arr[(*idx)++] = root->data; inorder_avl(root->right, arr, idx); }
}
void free_avl(AVLNode* root) {
    if (root) { free_avl(root->left); free_avl(root->right); free(root); }
}
void tree_sort_opt(Student* arr, int n, Criteria crit, Order ord) {
    AVLNode* root = NULL;
    for (int i = 0; i < n; i++) root = insert_avl(root, arr[i], crit, ord);
    int idx = 0; inorder_avl(root, arr, &idx); free_avl(root);
}

// [Framework] 테스트 엔진
typedef void (*SortFunc)(Student*, int, Criteria, Order);

void run_test(const char* name, SortFunc func, Student* origin, int n,
    Criteria crit, Order ord, int is_stable) {

    if (crit == CRIT_GENDER && !is_stable) return;
    if (strcmp(name, "Radix") == 0 && crit != CRIT_ID) return;

    int has_dup = (crit != CRIT_ID);
    if (has_dup && (strncmp(name, "Tree", 4) == 0 || strncmp(name, "Heap", 4) == 0)) {
 
    }

    long long total = 0;
    int repeat = 1000;

    // [수정] 느린 정렬 10회 반복 (시간 최적화)
    if (strcmp(name, "Bubble") == 0 ||
        strcmp(name, "Selection") == 0 ||
        strcmp(name, "Insertion") == 0 ||
        strcmp(name, "Tree") == 0 ||       // Part A Tree (BST)
        strcmp(name, "Tree(BST)") == 0) {  // Part B Tree (BST)
        repeat = 10;
    }

    for (int k = 0; k < repeat; k++) {
        Student* copy = (Student*)malloc(sizeof(Student) * n);
        memcpy(copy, origin, sizeof(Student) * n);
        compare_count = 0;
        func(copy, n, crit, ord);
        total += compare_count;
        free(copy);
    }

    long long mem = 0;
    if (strncmp(name, "Merge", 5) == 0) mem = (long long)n * sizeof(Student);
    else if (strncmp(name, "Tree", 4) == 0) mem = (long long)n * sizeof(Node);
    else if (strncmp(name, "Radix", 5) == 0) mem = (long long)n * sizeof(Student);

    printf("  %-12s (x%4d) | 평균 비교: %10lld회 | 추가 메모리: %8lld 바이트\n",
        name, repeat, total / repeat, mem);
}

// ==========================================
// [Main] 실행 진입점
// ==========================================
int main() {
    int count = 0;
    Student* students = load_students("dataset_id_ascending.csv", &count);

    if (!students) {
        printf("[알림] dataset_id_ascending.csv 파일을 찾을 수 없습니다.\n");
        return 1;
    }

    printf("=== [Part A] 기본 정렬 알고리즘 (데이터: %d개, 전체 로드됨) ===\n", count);

    Criteria crits[] = { CRIT_ID, CRIT_NAME, CRIT_GENDER, CRIT_GRADE };
    char* c_names[] = { "ID(학번)", "NAME(이름)", "GENDER(성별)", "GRADE(성적)" };
    Order ords[] = { ORDER_ASC, ORDER_DESC };
    char* o_names[] = { "오름차순", "내림차순" };

    for (int c = 0; c < 4; c++) {
        for (int o = 0; o < 2; o++) {
            printf("\n[정렬 기준] %s - %s\n", c_names[c], o_names[o]);
            printf("-----------------------------------------------------------------------------\n");
            run_test("Bubble", bubble_sort, students, count, crits[c], ords[o], 1);
            run_test("Selection", selection_sort, students, count, crits[c], ords[o], 0);
            run_test("Insertion", insertion_sort, students, count, crits[c], ords[o], 1);
            run_test("Shell", shell_sort, students, count, crits[c], ords[o], 0);
            run_test("Quick", quick_sort, students, count, crits[c], ords[o], 0);
            run_test("Heap", heap_sort, students, count, crits[c], ords[o], 0);
            run_test("Merge", merge_sort, students, count, crits[c], ords[o], 1);
            run_test("Tree", tree_sort, students, count, crits[c], ords[o], 0);
            run_test("Radix", radix_sort, students, count, crits[c], ords[o], 1);
        }
    }

    printf("\n\n============================================================\n");
    printf("=== [Part B] 정렬 최적화 경쟁 (비교 횟수 최소화) ===\n");
    printf("============================================================\n");

    Criteria c = CRIT_ID; Order o = ORDER_ASC;
    printf("\n[테스트 시나리오] ID 오름차순 (이미 정렬된 데이터 최적화 검증)\n");

    printf("\n--- 1. 셸 정렬 (기본 vs Tokuda 간격 최적화) ---\n");
    run_test("Shell(기본)", shell_sort, students, count, c, o, 0);
    run_test("Shell(최적)", shell_sort_opt, students, count, c, o, 0);

    printf("\n--- 2. 퀵 정렬 (기본 vs Hybrid 최적화) ---\n");
    run_test("Quick(기본)", quick_sort, students, count, c, o, 0);
    run_test("Quick(최적)", quick_sort_opt, students, count, c, o, 0);

    printf("\n--- 3. 트리 정렬 (BST vs AVL 트리 최적화) ---\n");
    run_test("Tree(BST)", tree_sort, students, count, c, o, 0);
    run_test("Tree(AVL)", tree_sort_opt, students, count, c, o, 0);

    free(students);
    return 0;
}