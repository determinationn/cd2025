#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TYPE_TOKEN,      
    MAIN_TOKEN,       
    IF_TOKEN,         
    ELSE_TOKEN,    
    WHILE_TOKEN,    
    ID_TOKEN,       
    LITERAL_TOKEN,    
    ASSIGN_TOKEN,   
    EQUAL_TOKEN,   
    GREATEREQUAL_TOKEN, 
    LESSEQUAL_TOKEN, 
    GREATER_TOKEN,   
    LESS_TOKEN,       
    PLUS_TOKEN,      
    MINUS_TOKEN,     
    LEFTPAREN_TOKEN,  
    RIGHTPAREN_TOKEN, 
    LEFTBRACE_TOKEN,  
    RIGHTBRACE_TOKEN, 
    SEMICOLON_TOKEN,  
    UNKNOWN_TOKEN     
} TokenType;


typedef struct Node {
    char character;
    struct Node* next;
} Node;


Node* createNode(char character) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->character = character;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Node** head, char character) {
    Node* newNode = createNode(character);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}


void freeList(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}


Node* readFileIntoList(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file!\n");
        return NULL;
    }
    Node* head = NULL;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        appendNode(&head, ch);
    }
    fclose(file);
    return head;
}


char currentChar(Node* node) {
    if (node == NULL) return '\0';
    return node->character;
}


Node* advance(Node* node) {
    if (node == NULL) return NULL;
    return node->next;
}


int isLetter(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}


int isDigit(char c) {
    return (c >= '0' && c <= '9');
}


int isWhitespace(char c) {
    return (c == ' ' || c == '\n' || c == '\t');
}


const char* tokenTypeToString(TokenType t) {
    switch (t) {
        case TYPE_TOKEN:         return "TYPE_TOKEN";
        case MAIN_TOKEN:         return "MAIN_TOKEN";
        case IF_TOKEN:           return "IF_TOKEN";
        case ELSE_TOKEN:         return "ELSE_TOKEN";
        case WHILE_TOKEN:        return "WHILE_TOKEN";
        case ID_TOKEN:           return "ID_TOKEN";
        case LITERAL_TOKEN:      return "LITERAL_TOKEN";
        case ASSIGN_TOKEN:       return "ASSIGN_TOKEN";
        case EQUAL_TOKEN:        return "EQUAL_TOKEN";
        case GREATEREQUAL_TOKEN: return "GREATEREQUAL_TOKEN";
        case LESSEQUAL_TOKEN:    return "LESSEQUAL_TOKEN";
        case GREATER_TOKEN:      return "GREATER_TOKEN";
        case LESS_TOKEN:         return "LESS_TOKEN";
        case PLUS_TOKEN:         return "PLUS_TOKEN";
        case MINUS_TOKEN:        return "MINUS_TOKEN";
        case LEFTPAREN_TOKEN:    return "LEFTPAREN_TOKEN";
        case RIGHTPAREN_TOKEN:   return "RIGHTPAREN_TOKEN";
        case LEFTBRACE_TOKEN:    return "LEFTBRACE_TOKEN";
        case RIGHTBRACE_TOKEN:   return "RIGHTBRACE_TOKEN";
        case SEMICOLON_TOKEN:    return "SEMICOLON_TOKEN";
        default:                 return "UNKNOWN_TOKEN";
    }
}

void scanTokens(Node* head) {
    Node* current = head;

    while (current != NULL) {
        char c = currentChar(current);

        if (isWhitespace(c)) {
            current = advance(current);
            continue;
        }

        if (isLetter(c)) {
            char buffer[256];
            int idx = 0;
            while (current != NULL && (isLetter(currentChar(current)) || isDigit(currentChar(current)))) {
                buffer[idx++] = currentChar(current);
                current = advance(current);
            }
            buffer[idx] = '\0';

            if (strcmp(buffer, "int") == 0) {
                printf("%s: %s\n", buffer, tokenTypeToString(TYPE_TOKEN));
            } else if (strcmp(buffer, "main") == 0) {
                printf("%s: %s\n", buffer, tokenTypeToString(MAIN_TOKEN));
            } else if (strcmp(buffer, "if") == 0) {
                printf("%s: %s\n", buffer, tokenTypeToString(IF_TOKEN));
            } else if (strcmp(buffer, "else") == 0) {
                printf("%s: %s\n", buffer, tokenTypeToString(ELSE_TOKEN));
            } else if (strcmp(buffer, "while") == 0) {
                printf("%s: %s\n", buffer, tokenTypeToString(WHILE_TOKEN));
            } else {
                printf("%s: %s\n", buffer, tokenTypeToString(ID_TOKEN));
            }
            continue;
        }

        if (isDigit(c)) {
            char buffer[256];
            int idx = 0;
            while (current != NULL && isDigit(currentChar(current))) {
                buffer[idx++] = currentChar(current);
                current = advance(current);
            }
            buffer[idx] = '\0';
            printf("%s: %s\n", buffer, tokenTypeToString(LITERAL_TOKEN));
            continue;
        }

        switch (c) {
            case '=': {
                Node* nextNode = current->next;
                if (nextNode && currentChar(nextNode) == '=') {
                    printf("==: %s\n", tokenTypeToString(EQUAL_TOKEN));
                    current = advance(current);
                } else {
                    printf("=: %s\n", tokenTypeToString(ASSIGN_TOKEN));
                }
                current = advance(current);
                break;
            }
            case '>': {
                Node* nextNode = current->next;
                if (nextNode && currentChar(nextNode) == '=') {
                    printf(">=: %s\n", tokenTypeToString(GREATEREQUAL_TOKEN));
                    current = advance(current);
                } else {
                    printf(">: %s\n", tokenTypeToString(GREATER_TOKEN));
                }
                current = advance(current);
                break;
            }
            case '<': {
                Node* nextNode = current->next;
                if (nextNode && currentChar(nextNode) == '=') {
                    printf("<=: %s\n", tokenTypeToString(LESSEQUAL_TOKEN));
                    current = advance(current);
                } else {
                    printf("<: %s\n", tokenTypeToString(LESS_TOKEN));
                }
                current = advance(current);
                break;
            }
            case '+':
                printf("+: %s\n", tokenTypeToString(PLUS_TOKEN));
                current = advance(current);
                break;
            case '-':
                printf("-: %s\n", tokenTypeToString(MINUS_TOKEN));
                current = advance(current);
                break;
            case '(':
                printf("(: %s\n", tokenTypeToString(LEFTPAREN_TOKEN));
                current = advance(current);
                break;
            case ')':
                printf("): %s\n", tokenTypeToString(RIGHTPAREN_TOKEN));
                current = advance(current);
                break;
            case '{':
                printf("{: %s\n", tokenTypeToString(LEFTBRACE_TOKEN));
                current = advance(current);
                break;
            case '}':
                printf("}: %s\n", tokenTypeToString(RIGHTBRACE_TOKEN));
                current = advance(current);
                break;
            case ';':
                printf(";: %s\n", tokenTypeToString(SEMICOLON_TOKEN));
                current = advance(current);
                break;
            default:
                printf("%c: %s\n", c, tokenTypeToString(UNKNOWN_TOKEN));
                current = advance(current);
                break;
        }
    }
}

int main() {
    Node* head = readFileIntoList("main.c");
    if (!head) {
        return 1; 
    }

    scanTokens(head);

    freeList(head);

    return 0;
}
