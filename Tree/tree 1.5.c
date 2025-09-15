#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ERROR 0
#define TRUE 1
#define FALSE 2

void trim(char* str);
int check_paren(char* str);
int check_binary_tree(char* str, int len);
int parse_node(char* str, int* idx, int len);

int main() {
    char buf[1000];
    fgets(buf, sizeof(buf), stdin);
    trim(buf);

    if (check_paren(buf) == ERROR) {
        printf("ERROR\n");
        return 0;
    }

    int result = check_binary_tree(buf, strlen(buf));
    if (result == TRUE) printf("TRUE\n");
    else if (result == FALSE) printf("FALSE\n");
    else printf("ERROR\n");

    return 0;
}

void trim(char* str) {
    int len = strlen(str);
    int start = 0, end = len - 1;
    while (isspace(str[start])) start++;
    while (end >= 0 && isspace(str[end])) end--;
    int j = 0;
    for (int i = start; i <= end; i++) str[j++] = str[i];
    str[j] = '\0';
}

int check_paren(char* str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '(') count++;
        else if (str[i] == ')') count--;
        if (count < 0) return ERROR;
    }
    return (count == 0) ? TRUE : ERROR;
}

int check_binary_tree(char* str, int len) {
    int idx = 0;
    int res = parse_node(str, &idx, len);
    if (res == FALSE) return FALSE;
    if (idx != len) return ERROR;
    return TRUE;
}

int parse_node(char* str, int* idx, int len) {
    int children = 0;

    while (*idx < len) {
        char ch = str[*idx];

        if (ch == ' ') { (*idx)++; continue; }

        if (isalpha(ch)) {
            children++;
            if (children > 2) return FALSE;

            (*idx)++; 

            if (*idx < len && str[*idx] == '(') {
                (*idx)++; 
                int res = parse_node(str, idx, len);
                if (res != TRUE) return res;

                if (*idx >= len || str[*idx] != ')') return ERROR;
                (*idx)++; 
            }
        }

        else if (ch == '(') {
            (*idx)++; 
            int res = parse_node(str, idx, len);
            if (res != TRUE) return res;
            if (*idx >= len || str[*idx] != ')') return ERROR;
            (*idx)++; 
            children++;
            if (children > 2) return FALSE;
        }

        else if (ch == ')') {
            return TRUE;
        }

        else {
            return ERROR;
        }
    }

    return TRUE;
}
