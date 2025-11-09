#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VERTICES 10
#define EDGES 20
#define MAX_WEIGHT 10
#define INF 99999

int main() {
    int dist[VERTICES][VERTICES];
    srand(time(NULL));    // 실행할 때마다 다른 난수를 생성하기 위한 시드 설정

    // 모든 정점 쌍 거리 초기화
    for (int i = 0; i < VERTICES; i++) {
        for (int j = 0; j < VERTICES; j++) {
            dist[i][j] = (i == j) ? 0 : INF;   // 자기 자신은 거리 0, 나머지는 무한대로 설정
        }
    }

    // 무작위 간선 생성 (무방향 그래프)
    int edge_count = 0;
    while (edge_count < EDGES) {
        int u = rand() % VERTICES;             // 랜덤한 출발 정점
        int v = rand() % VERTICES;             // 랜덤한 도착 정점
        int w = rand() % MAX_WEIGHT + 1;       // 1~MAX_WEIGHT 범위의 가중치

        // 자기 자신으로 가는 간선과 중복 간선 방지
        if (u != v && dist[u][v] == INF) {
            dist[u][v] = w;
            dist[v][u] = w;                    // 무방향이므로 반대 방향도 동일하게 설정
            edge_count++;
        }
    }

    // Floyd–Warshall 알고리즘: 모든 정점 쌍의 최단거리 계산
    for (int k = 0; k < VERTICES; k++) {       // k는 중간 경유 정점
        for (int i = 0; i < VERTICES; i++) {   // i는 출발 정점
            for (int j = 0; j < VERTICES; j++) { // j는 도착 정점
                
                // i→k→j 경로가 존재하고, 그 경로가 더 짧으면 거리 갱신
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    printf("=== 모든 노드 쌍(45쌍) 최단 거리 ===\n");
    for (int i = 0; i < VERTICES; i++) {
        for (int j = i + 1; j < VERTICES; j++) {
            if (dist[i][j] == INF)
                printf("(%d, %d): 연결 없음\n", i, j);
            else
                printf("(%d, %d): %d\n", i, j, dist[i][j]);
        }
    }

    return 0;
}