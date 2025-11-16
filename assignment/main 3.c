#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct Node {
    char data;
    int left_child;
    int right_child;
} Node;

Node* tree = NULL;
int node_count = 1;
int tree_capacity = 0;

void build_tree(const char* s) {
    int* stack = (int*)malloc(sizeof(int) * strlen(s));
    int top = -1;

    for (int i = 0; s[i] != '\0'; ++i) {
        if (isalpha(s[i])) {
            if (node_count >= tree_capacity) {
                tree_capacity = (tree_capacity == 0) ? 16 : tree_capacity * 2;
                tree = (Node*)realloc(tree, sizeof(Node) * tree_capacity);
                if (tree == NULL) {
                    printf("메모리 재할당 실패\n");
                    free(stack);
                    return;
                }
            }

            tree[node_count].data = s[i];
            tree[node_count].left_child = -1;
            tree[node_count].right_child = -1;

            if (top != -1) {
                int parent_index = stack[top];
                if (tree[parent_index].left_child == -1) {
                    tree[parent_index].left_child = node_count;
                }
                else {
                    tree[parent_index].right_child = node_count;
                }
            }

            int j = i + 1;
            while (s[j] == ' ') j++;

            if (s[j] == '(') {
                stack[++top] = node_count;
            }
            node_count++;
        }
        else if (s[i] == ')') {
            if (top != -1) {
                top--;
            }
        }
    }
    free(stack);
}

void iterative_preorder(int root_index) {
    if (root_index == -1 || node_count <= 1) return;

    int* stack = (int*)malloc(sizeof(int) * node_count);
    int top = -1;

    stack[++top] = root_index;
    printf("pre-order: ");
    while (top != -1) {
        int current_index = stack[top--];
        Node current_node = tree[current_index];
        printf("%c ", current_node.data);
        if (current_node.right_child != -1) stack[++top] = current_node.right_child;
        if (current_node.left_child != -1) stack[++top] = current_node.left_child;
    }
    printf("\n");
    free(stack);
}

void iterative_inorder(int root_index) {
    if (root_index == -1 || node_count <= 1) return;

    int* stack = (int*)malloc(sizeof(int) * node_count);
    int top = -1;
    int current_index = root_index;

    printf("in-order: ");
    while (current_index != -1 || top != -1) {
        while (current_index != -1) {
            stack[++top] = current_index;
            current_index = tree[current_index].left_child;
        }
        current_index = stack[top--];
        printf("%c ", tree[current_index].data);
        current_index = tree[current_index].right_child;
    }
    printf("\n");
    free(stack);
}

void iterative_postorder(int root_index) {
    if (root_index == -1 || node_count <= 1) return;

    int* s1 = (int*)malloc(sizeof(int) * node_count);
    int* s2 = (int*)malloc(sizeof(int) * node_count);
    int top1 = -1, top2 = -1;

    s1[++top1] = root_index;
    while (top1 != -1) {
        int current_index = s1[top1--];
        s2[++top2] = current_index;
        Node current_node = tree[current_index];
        if (current_node.left_child != -1) s1[++top1] = current_node.left_child;
        if (current_node.right_child != -1) s1[++top1] = current_node.right_child;
    }

    printf("post-order: ");
    while (top2 != -1) {
        int index = s2[top2--];
        printf("%c ", tree[index].data);
    }
    printf("\n");
    free(s1);
    free(s2);
}

int main() {
    char input_string[256];
    int root_node_index = 1;

    scanf_s("%[^\n]s", input_string, (unsigned)sizeof(input_string));

    build_tree(input_string);

    iterative_preorder(root_node_index);
    iterative_inorder(root_node_index);
    iterative_postorder(root_node_index);

    free(tree);

    return 0;
}