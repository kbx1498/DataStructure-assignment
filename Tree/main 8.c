#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_ELEMENTS 10000    // 데이터 개수
#define MAX_VALUE 1000001     // 데이터 범위 0 ~ 1,000,000
#define NUM_TRIALS 100        // 실행 횟수

// 단순 삽입 정렬
long long insertion_sort(int arr[], int size) {
    long long comparisonCount = 0;
    int i, j, key;

    for (i = 1; i < size; i++) {
        key = arr[i];

        for (j = i - 1; j >= 0; j--) {
            comparisonCount++; // 비교 횟수 카운트

            if (arr[j] > key)        // 삽입 위치 찾기
                arr[j + 1] = arr[j]; // 뒤로 이동
            else
                break;               // 삽입 위치 찾음
        }
        arr[j + 1] = key;           // key 삽입
    }
    return comparisonCount;
}

// 기본 셸 정렬 (N/2, N/4 ...)
long long shell_sort_basic(int arr[], int size) {
    long long comparisonCount = 0;
    int i, j, temp;

    for (int gap = size / 2; gap > 0; gap /= 2) { // 간격 줄이기
        for (i = gap; i < size; i++) {
            temp = arr[i];
            for (j = i; j >= gap; j -= gap) {
                comparisonCount++;   // 비교 카운트
                if (arr[j - gap] > temp)
                    arr[j] = arr[j - gap]; // 뒤로 이동
                else
                    break;
            }
            arr[j] = temp; // 값 삽입
        }
    }
    return comparisonCount;
}

// 최적 셸 정렬 (Knuth 수열)
long long shell_sort_knuth(int arr[], int size) {
    long long comparisonCount = 0;
    int i, j, temp;

    int h = 1;
    while (h < size / 3)           // Knuth 수열 최대값 찾기
        h = h * 3 + 1;

    for (int gap = h; gap > 0; gap = (gap - 1) / 3) { // 간격 줄이면서 정렬
        for (i = gap; i < size; i++) {
            temp = arr[i];
            for (j = i; j >= gap; j -= gap) {
                comparisonCount++; // 비교 카운트
                if (arr[j - gap] > temp)
                    arr[j] = arr[j - gap]; // 뒤로 이동
                else
                    break;
            }
            arr[j] = temp; // 값 삽입
        }
    }
    return comparisonCount;
}



int main() {
    // 배열 힙 할당
    int* original_array = malloc(sizeof(int) * NUM_ELEMENTS);
    int* test_array = malloc(sizeof(int) * NUM_ELEMENTS);

    if (!original_array || !test_array) { // 실패 시 종료
        printf("메모리 할당 실패!\n");
        return 1;
    }

    srand(time(NULL)); // 랜덤 시드

    long double total_insertion = 0;
    long double total_shell_basic = 0;
    long double total_shell_knuth = 0;

    printf("데이터 %d개, 총 %d회 실행\n\n", NUM_ELEMENTS, NUM_TRIALS);

    // NUM_TRIALS 반복
    for (int i = 0; i < NUM_TRIALS; i++) {
        // 랜덤 데이터 생성
        for (int j = 0; j < NUM_ELEMENTS; j++) {
            original_array[j] = rand() % MAX_VALUE;
        }

        // 삽입 정렬
        memcpy(test_array, original_array, sizeof(int) * NUM_ELEMENTS);
        total_insertion += insertion_sort(test_array, NUM_ELEMENTS);

        // 기본 셸 정렬
        memcpy(test_array, original_array, sizeof(int) * NUM_ELEMENTS);
        total_shell_basic += shell_sort_basic(test_array, NUM_ELEMENTS);

        // 최적 셸 정렬
        memcpy(test_array, original_array, sizeof(int) * NUM_ELEMENTS);
        total_shell_knuth += shell_sort_knuth(test_array, NUM_ELEMENTS);

        if (i == NUM_TRIALS - 1) // 마지막 1회만 출력
            printf("...%d회 실행 완료\n", NUM_TRIALS);
    }

    // 결과 출력
    printf("\n_______________________________________________________\n");
    printf(" [결과] 데이터 %d개, %d회 실행 평균 비교 횟수\n",
        NUM_ELEMENTS, NUM_TRIALS);
    printf("_______________________________________________________\n");

    printf(" 1. 단순 삽입 정렬:           %.0Lf 회\n",
        total_insertion / NUM_TRIALS);
    printf(" 2. 기본 셸 정렬 (N/2):       %.0Lf 회\n",
        total_shell_basic / NUM_TRIALS);
    printf(" 3. 최적 셸 정렬 (Knuth):     %.0Lf 회\n",
        total_shell_knuth / NUM_TRIALS);

    printf("_______________________________________________________\n");
    printf(" * 최적 셸 정렬은 Knuth 수열 사용\n");

    free(original_array);
    free(test_array);
    return 0;
}
