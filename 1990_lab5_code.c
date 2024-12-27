#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_KEYWORDS 32
#define MAX_IDENTIFIERS 1000

const char *keywords[NUM_KEYWORDS] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

const char *standardLibraries[] = {
    "stdio.h", "stdlib.h", "string.h", "math.h", "time.h", "ctype.h",
    "limits.h", "stdbool.h", "stdint.h", "assert.h", "locale.h", "errno.h"
};

typedef struct {
    char name[100];
    int count;
} Identifier;

int isComment(const char *line) {
    while (*line == ' ' || *line == '\t')
        line++;

    if (strncmp(line, "//", 2) == 0)
        return 1;

    if (strncmp(line, "/*", 2) == 0)
        return 1;

    if (strstr(line, "*/") != NULL)
        return 1;

    return 0;
}

int isKeyword(const char *word) {
    int i;
    for (i = 0; i < NUM_KEYWORDS; ++i) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isIdentifier(const char *word) {
    if (isKeyword(word) || !isalpha(word[0])) {
        return 0;
    }
    const char *p;
    for (p = word; *p; ++p) {
        if (!isalnum(*p) && *p != '_') {
            return 0;
        }
    }
    return 1;
}

int findIdentifierIndex(const char *word, Identifier identifiers[MAX_IDENTIFIERS], int identifierCount) {
    int i;
    for (i = 0; i < identifierCount; ++i) {
        if (strcmp(word, identifiers[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

void removeComments(char *line) {
    char *commentStart = strstr(line, "//");
    if (commentStart != NULL) {
        *commentStart = '\0';
    }
    commentStart = strstr(line, "/*");
    if (commentStart != NULL) {
        char *commentEnd = strstr(commentStart + 2, "*/");
        if (commentEnd != NULL) {
            strcpy(commentStart, commentEnd + 2);
        } else {
            *commentStart = '\0';
        }
    }
}

void processLine(char *line, Identifier identifiers[MAX_IDENTIFIERS], int *identifierCount, int keywordCounts[NUM_KEYWORDS]) {
    int inString = 0;
    char *token = strtok(line, " \t\n;(){}[]<>+-=*/%,&|^!~");
    while (token != NULL) {
        if (token[0] == '\"') {
            inString = !inString;
            token = strtok(NULL, " \t\n;(){}[]<>+-=*/%,&|^!~");
            continue;
        }
        if (!inString) {
            int i;
            for (i = 0; i < sizeof(standardLibraries) / sizeof(standardLibraries[0]); ++i) {
                if (strcmp(token, standardLibraries[i]) == 0) {
                    token[strlen(token) - 2] = '\0';
                    break;
                }
            }
            if (isKeyword(token)) {
                for (i = 0; i < NUM_KEYWORDS; ++i) {
                    if (strcmp(token, keywords[i]) == 0) {
                        keywordCounts[i]++;
                        break;
                    }
                }
            } else if (isIdentifier(token)) {
                int index = findIdentifierIndex(token, identifiers, *identifierCount);
                if (index != -1) {
                    identifiers[index].count++;
                } else {
                    strcpy(identifiers[*identifierCount].name, token);
                    identifiers[*identifierCount].count = 1;
                    (*identifierCount)++;
                }
            }
        }
        token = strtok(NULL, " \t\n;(){}[]<>+-=*/%,&|^!~");
    }
}

int main() {
    FILE *sourceFile, *destinationFile;
    char sourceFileName[100], destinationFileName[100] = "output.txt", line[1000];
    int keywordCounts[NUM_KEYWORDS] = {0};
    Identifier identifiers[MAX_IDENTIFIERS];
    int identifierCount = 0;
    int i;

    printf("Source file: ");
    scanf("%s", sourceFileName);

    sourceFile = fopen(sourceFileName, "r");
    if (sourceFile == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    destinationFile = fopen(destinationFileName, "w");
    if (destinationFile == NULL) {
        fprintf(stderr, "Error creating output file.\n");
        fclose(sourceFile);
        return 1;
    }

    while (fgets(line, sizeof(line), sourceFile) != NULL) {
        if (!isComment(line)) {
            removeComments(line);
            fputs(line, destinationFile);
            processLine(line, identifiers, &identifierCount, keywordCounts);
        }
    }

    fclose(sourceFile);
    fclose(destinationFile);

    printf("Total Unique Identifiers: %d\n", identifierCount);

    printf("Unique variable counts:\n\n");
    for (i = 0; i < identifierCount; ++i) {
        printf("%s: %d\n", identifiers[i].name, identifiers[i].count);
    }

    return 0;
}
