#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100   // 정점 개수
#define SPARSE_EDGES 100
#define DENSE_EDGES 4000

// 그래프 구조 정의
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// 전역 변수
int adjMatrix[N][N];
Node* adjList[N];

// 그래프 초기화 함수
void initMatrix() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            adjMatrix[i][j] = 0;
}

void initList() {
    for (int i = 0; i < N; i++)
        adjList[i] = NULL;
}

// 간선 추가 함수
void addEdgeMatrix(int u, int v, int* compareCount) {
    (*compareCount)++;
    adjMatrix[u][v] = 1;
    adjMatrix[v][u] = 1;
}

void addEdgeList(int u, int v, int* compareCount) {
    (*compareCount)++;
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = adjList[u];
    adjList[u] = newNode;

    newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = u;
    newNode->next = adjList[v];
    adjList[v] = newNode;
}

// 랜덤 그래프 생성
void generateGraphMatrix(int edgeCount, int* compareCount) {
    int added = 0;
    while (added < edgeCount) {
        int u = rand() % N;
        int v = rand() % N;
        if (u != v && adjMatrix[u][v] == 0) {
            addEdgeMatrix(u, v, compareCount);
            added++;
        }
    }
}

void generateGraphList(int edgeCount, int* compareCount) {
    int added = 0;
    while (added < edgeCount) {
        int u = rand() % N;
        int v = rand() % N;
        if (u != v) {
            // 중복 여부 확인 (비교 증가)
            Node* cur = adjList[u];
            int duplicate = 0;
            while (cur) {
                (*compareCount)++;
                if (cur->vertex == v) {
                    duplicate = 1;
                    break;
                }
                cur = cur->next;
            }
            if (!duplicate) {
                addEdgeList(u, v, compareCount);
                added++;
            }
        }
    }
}

// 두 정점 연결 확인
int isConnectedMatrix(int u, int v, int* compareCount) {
    (*compareCount)++;
    return adjMatrix[u][v];
}

int isConnectedList(int u, int v, int* compareCount) {
    Node* cur = adjList[u];
    while (cur) {
        (*compareCount)++;
        if (cur->vertex == v)
            return 1;
        cur = cur->next;
    }
    return 0;
}

// 인접 노드 출력(비교 횟수만 )
void printAdjNodesMatrix(int u, int* compareCount) {
    for (int v = 0; v < N; v++) {
        (*compareCount)++;
        if (adjMatrix[u][v])
            ; 
    }
}

void printAdjNodesList(int u, int* compareCount) {
    Node* cur = adjList[u];
    while (cur) {
        (*compareCount)++;
        cur = cur->next;
    }
}

// 메모리 계산 (대략)
size_t getMemoryMatrix() {
    return sizeof(adjMatrix);
}

size_t getMemoryList() {
    size_t total = 0;
    for (int i = 0; i < N; i++) {
        Node* cur = adjList[i];
        while (cur) {
            total += sizeof(Node);
            cur = cur->next;
        }
    }
    return total;
}

int main() {
    srand((unsigned)time(NULL));

    int compareInsert, compareCheck, comparePrint;
    size_t mem;

    printf("케이스 1: 희소그래프 (인접행렬)\n");
    initMatrix();
    compareInsert = compareCheck = comparePrint = 0;
    generateGraphMatrix(SPARSE_EDGES, &compareInsert);
    isConnectedMatrix(0, 1, &compareCheck);
    printAdjNodesMatrix(0, &comparePrint);
    mem = getMemoryMatrix();
    printf("메모리 사용량: %zu Bytes\n", mem);
    printf("간선 삽입/삭제 비교: %d번\n", compareInsert);
    printf("두 정점 연결 확인 비교: %d번\n", compareCheck);
    printf("한 노드 인접 노드 출력 비교: %d번\n\n", comparePrint);

    printf("케이스 2: 희소그래프 (인접리스트)\n");
    initList();
    compareInsert = compareCheck = comparePrint = 0;
    generateGraphList(SPARSE_EDGES, &compareInsert);
    isConnectedList(0, 1, &compareCheck);
    printAdjNodesList(0, &comparePrint);
    mem = getMemoryList();
    printf("메모리 사용량: %zu Bytes\n", mem);
    printf("간선 삽입/삭제 비교: %d번\n", compareInsert);
    printf("두 정점 연결 확인 비교: %d번\n", compareCheck);
    printf("한 노드 인접 노드 출력 비교: %d번\n\n", comparePrint);

    printf("케이스 3: 밀집그래프 (인접행렬)\n");
    initMatrix();
    compareInsert = compareCheck = comparePrint = 0;
    generateGraphMatrix(DENSE_EDGES, &compareInsert);
    isConnectedMatrix(0, 1, &compareCheck);
    printAdjNodesMatrix(0, &comparePrint);
    mem = getMemoryMatrix();
    printf("메모리 사용량: %zu Bytes\n", mem);
    printf("간선 삽입/삭제 비교: %d번\n", compareInsert);
    printf("두 정점 연결 확인 비교: %d번\n", compareCheck);
    printf("한 노드 인접 노드 출력 비교: %d번\n\n", comparePrint);

    printf("케이스 4: 밀집그래프 (인접리스트)\n");
    initList();
    compareInsert = compareCheck = comparePrint = 0;
    generateGraphList(DENSE_EDGES, &compareInsert);
    isConnectedList(0, 1, &compareCheck);
    printAdjNodesList(0, &comparePrint);
    mem = getMemoryList();
    printf("메모리 사용량: %zu Bytes\n", mem);
    printf("간선 삽입/삭제 비교: %d번\n", compareInsert);
    printf("두 정점 연결 확인 비교: %d번\n", compareCheck);
    printf("한 노드 인접 노드 출력 비교: %d번\n", comparePrint);

    return 0;
}
