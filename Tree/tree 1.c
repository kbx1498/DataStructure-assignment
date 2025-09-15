#include <stdio.h>
#include <string.h>

int main() {
    char input[100];       
    int childCount[100];   
    int top, i;

    while (1) {
        printf("괄호 표기법으로 입력 or exit: ");
        fgets(input, sizeof(input), stdin);
        
        int len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') input[len - 1] = '\0';

        if (strcmp(input, "exit") == 0) break;

        if (strlen(input) == 0) {
            printf("ERROR\n");
            continue;
        }

        top = -1;
        i = 0;

        while (input[i] != '\0') {
            char ch = input[i];

            if (ch == ' ') {
                i++;
                continue;
            }

            if (ch == '(') {
                if (input[i + 1] == ')') {
                    if (top >= 0) {
                        childCount[top]++;
                        if (childCount[top] > 2) {
                            printf("FALSE\n");
                            break;
                        }
                    }
                    i += 2;
                    continue;
                }
                top++;
                childCount[top] = 0;
            }
            
            else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
                if (top >= 0) {
                    childCount[top]++;
                    if (childCount[top] > 2) {
                        printf("FALSE\n");
                        break;
                    }
                }
            }
           
            else if (ch == ')') {
                if (top < 0) { 
                    printf("ERROR\n");
                    break;
                }
                else {
                    top--;
                }
            }
            
            else { 
                printf("ERROR\n");
                break;
            }

            i++;
        }

        if (input[i] == '\0') {
            if (top != -1) { 
                printf("ERROR\n");
            }
            else {
                printf("TRUE\n");
            }
        }
    }

    printf("프로그램 종료.\n");
    return 0;
}
