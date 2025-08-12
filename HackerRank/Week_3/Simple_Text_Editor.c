#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct UndoOp {
    char op[10];  // "append" or "delete"
    char* param;  // string for append or deleted substring
    struct UndoOp* next;
} UndoOp;

UndoOp* undo_stack = NULL;

void push_undo(const char* op, const char* param) {
    UndoOp* new_node = malloc(sizeof(UndoOp));
    strcpy(new_node->op, op);
    new_node->param = strdup(param);
    new_node->next = undo_stack;
    undo_stack = new_node;
}

UndoOp* pop_undo() {
    if (!undo_stack) return NULL;
    UndoOp* top = undo_stack;
    undo_stack = undo_stack->next;
    return top;
}

void free_undo(UndoOp* op) {
    if (!op) return;
    free(op->param);
    free(op);
}

void perform(char** s_ptr, const char* op, const char* param, int in_undo);

int main() {
    int n_q;
    scanf("%d\n", &n_q);

    char* s = calloc(1, sizeof(char));  // dynamic string, initially empty
    size_t s_len = 0;
    size_t s_cap = 1;

    for (int i = 0; i < n_q; i++) {
        char line[1024];
        if (!fgets(line, sizeof(line), stdin)) break;

        int code;
        char param[1024] = {0};

        int count = sscanf(line, "%d %s", &code, param);
        // code: 1=append,2=delete,3=print,4=undo

        if (code == 1) {  // append
            perform(&s, "append", param, 0);
        } else if (code == 2) {  // delete
            perform(&s, "delete", param, 0);
        } else if (code == 3) {  // print
            int pos = atoi(param);
            if (pos > 0 && pos <= (int)strlen(s))
                printf("%c\n", s[pos-1]);
        } else if (code == 4) {  // undo
            UndoOp* undo = pop_undo();
            if (undo) {
                perform(&s, undo->op, undo->param, 1);
                free_undo(undo);
            }
        }
    }

    free(s);

    // free any remaining undo stack nodes
    while (undo_stack) {
        UndoOp* tmp = pop_undo();
        free_undo(tmp);
    }

    return 0;
}

void perform(char** s_ptr, const char* op, const char* param, int in_undo) {
    char* s = *s_ptr;
    size_t s_len = strlen(s);

    if (strcmp(op, "append") == 0) {
        size_t param_len = strlen(param);
        if (!in_undo) {
            // save for undo: delete last param_len chars
            char num_str[20];
            sprintf(num_str, "%zu", param_len);
            push_undo("delete", num_str);
        }
        // append param
        s = realloc(s, s_len + param_len + 1);
        memcpy(s + s_len, param, param_len + 1);
        *s_ptr = s;

    } else if (strcmp(op, "delete") == 0) {
        int del_len = atoi(param);
        if (del_len > (int)s_len) del_len = (int)s_len;

        if (!in_undo) {
            // save for undo: append deleted substring
            char* deleted_sub = malloc(del_len + 1);
            memcpy(deleted_sub, s + s_len - del_len, del_len);
            deleted_sub[del_len] = '\0';
            push_undo("append", deleted_sub);
            free(deleted_sub);
        }
        // delete last del_len chars
        s[s_len - del_len] = '\0';
        s = realloc(s, s_len - del_len + 1);
        *s_ptr = s;
    }
}
