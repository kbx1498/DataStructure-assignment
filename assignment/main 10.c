#define _CRT_SECURE_NO_WARNINGS // Visual Studio 보안 경고 무시 (필수)

#include <stdio.h>
#include <stdlib.h> // malloc, free, qsort, rand, srand, system
#include <time.h>   // time
#include <string.h> // strchr 등 문자열 처리

#define NUM_STUDENTS 35000 // 넉넉하게 잡은 배열 크기
#define MAX_SCORE 100

// 학생 정보 구조체 정의
typedef struct {
    int id;            // 학번
    int score1;        // 국어
    int score2;        // 영어
    int score3;        // 수학
    long long product; // 세 성적의 곱
} Student;

// ----------------------------------------------------
// 퀵 정렬 함수들
// ----------------------------------------------------
void swap(Student* a, Student* b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Student arr[], int low, int high, long long* sortComparisons) {
    long long pivot = arr[high].product;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        (*sortComparisons)++;
        if (arr[j].product < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSortStruct(Student arr[], int low, int high, long long* sortComparisons) {
    if (low < high) {
        int pi = partition(arr, low, high, sortComparisons);
        quickSortStruct(arr, low, pi - 1, sortComparisons);
        quickSortStruct(arr, pi + 1, high, sortComparisons);
    }
}

// ----------------------------------------------------
// 탐색 함수들
// ----------------------------------------------------
long long sequentialSearch(Student arr[], int size, long long target, int* foundIndex) {
    long long comparisons = 0;
    *foundIndex = -1;
    for (int i = 0; i < size; i++) {
        comparisons++;
        if (arr[i].product == target) {
            *foundIndex = i;
            return comparisons;
        }
    }
    return comparisons;
}

long long binarySearch(Student arr[], int size, long long target, int* foundIndex) {
    long long comparisons = 0;
    int low = 0;
    int high = size - 1;
    *foundIndex = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        comparisons++;
        if (arr[mid].product == target) {
            *foundIndex = mid;
            return comparisons;
        }
        else if (arr[mid].product < target) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return comparisons;
}

// ====================================================
// Main 함수
// ====================================================
int main() {
    Student* students = (Student*)malloc(sizeof(Student) * NUM_STUDENTS);
    Student* sorted_students = (Student*)malloc(sizeof(Student) * NUM_STUDENTS);

    if (students == NULL || sorted_students == NULL) {
        printf("메모리 할당 실패\n");
        return -1;
    }

    // ------------------------------------------------------------
    // 1. CSV 파일 읽기
    // ------------------------------------------------------------
    // 주의: 파일은 프로젝트 폴더(소스코드 .c 파일이 있는 곳)에 있어야 합니다.
    FILE* fp = fopen("dataset_id_ascending.csv", "r");

    if (fp == NULL) {
        printf("[오류] 파일을 열 수 없습니다.\n");
        printf("1. 파일명(dataset_id_ascending.csv)을 확인하세요.\n");
        printf("2. 파일이 소스코드(.c)와 같은 폴더에 있는지 확인하세요.\n");
        system("pause"); // 콘솔창 바로 꺼짐 방지
        free(students);
        free(sorted_students);
        return 1;
    }

    printf("CSV 파일에서 데이터 읽는 중...\n");

    char line[1024];
    // 첫 번째 줄(헤더) 건너뛰기
    if (fgets(line, sizeof(line), fp) == NULL) {
        printf("파일이 비어있습니다.\n");
        fclose(fp);
        return 1;
    }

    int count = 0;
    char temp_name[100];
    char temp_gender[10];

    // 한 줄씩 읽어서 파싱 (fgets + sscanf 방식이 가장 안정적)
    while (count < NUM_STUDENTS && fgets(line, sizeof(line), fp) != NULL) {
        // CSV 형식: ID,NAME,GENDER,KOR,ENG,MATH
        // 예: 100001,Liam,M,89,16,66
        // sscanf로 콤마(,)를 건너뛰며 데이터 추출
        int parsed = sscanf(line, "%d,%[^,],%[^,],%d,%d,%d",
            &students[count].id,
            temp_name,        // 이름 (임시 저장)
            temp_gender,      // 성별 (임시 저장)
            &students[count].score1,
            &students[count].score2,
            &students[count].score3);

        if (parsed >= 6) { // 6개 항목을 제대로 읽었을 때만 처리
            // [과제 요구사항 1] 세 성적의 곱 계산
            students[count].product = (long long)students[count].score1 * students[count].score2 * students[count].score3;
            count++;
        }
    }
    fclose(fp);

    printf("총 %d명의 데이터를 읽었습니다.\n\n", count);
    int actual_size = count;

    // ------------------------------------------------------------
    // 2. 임의의 타겟 값 선정 (0 ~ 1,000,000)
    // ------------------------------------------------------------
    srand((unsigned int)time(NULL));
    long long target = rand() % 1000001;
    printf("찾을 목표 값(성적 곱): %lld\n\n", target);

    // ------------------------------------------------------------
    // [과제 요구사항 2] 순차 탐색 수행
    // ------------------------------------------------------------
    int seqIndex;
    long long seqComparisons = sequentialSearch(students, actual_size, target, &seqIndex);

    printf("--- [실험 1] 순차 탐색 ---\n");
    if (seqIndex != -1)
        printf("결과: 찾음 (ID: %d, 점수: %d/%d/%d, 곱: %lld)\n",
            students[seqIndex].id,
            students[seqIndex].score1, students[seqIndex].score2, students[seqIndex].score3,
            students[seqIndex].product);
    else
        printf("결과: 못 찾음\n");
    printf("비교 횟수: %lld회\n\n", seqComparisons);

    // ------------------------------------------------------------
    // [과제 요구사항 3] 정렬 후 이진 탐색 수행
    // ------------------------------------------------------------
    // 원본 데이터를 복사해서 사용 (원본 순서 유지를 위해)
    for (int i = 0; i < actual_size; i++) {
        sorted_students[i] = students[i];
    }

    long long sortComparisons = 0;
    long long binSearchComparisons = 0;
    int binIndex;

    // 3-1. 퀵 정렬 수행
    quickSortStruct(sorted_students, 0, actual_size - 1, &sortComparisons);

    // 3-2. 이진 탐색 수행
    binSearchComparisons = binarySearch(sorted_students, actual_size, target, &binIndex);

    printf("--- [실험 2] 정렬 + 이진 탐색 ---\n");
    if (binIndex != -1)
        printf("결과: 찾음 (ID: %d, 점수: %d/%d/%d, 곱: %lld)\n",
            sorted_students[binIndex].id,
            sorted_students[binIndex].score1, sorted_students[binIndex].score2, sorted_students[binIndex].score3,
            sorted_students[binIndex].product);
    else
        printf("결과: 못 찾음\n");

    printf("정렬 비교 횟수: %lld회\n", sortComparisons);
    printf("탐색 비교 횟수: %lld회\n", binSearchComparisons);
    printf("총 합계 비용: %lld회\n", sortComparisons + binSearchComparisons);

    // ------------------------------------------------------------
    // 메모리 해제 및 종료 대기
    // ------------------------------------------------------------
    free(students);
    free(sorted_students);

    printf("\n프로그램을 종료하려면 아무 키나 누르세요...");
    system("pause"); // Visual Studio에서 창이 바로 꺼지는 것 방지

    return 0;
}