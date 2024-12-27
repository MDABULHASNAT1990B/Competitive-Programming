#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_KEYWORDS 32
#define MAX_KEYWORD_LENGTH 20

char keywords[MAX_KEYWORDS][MAX_KEYWORD_LENGTH] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

int is_keyword(const char *word) {
    int i;
    for (i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    FILE *input_file, *output_file;
    char input_file_name[100], output_file_name[100];
    char ch;
    char word[MAX_KEYWORD_LENGTH];
    int keywordCount[MAX_KEYWORDS] = {0};
    int uniqueKeywordCount = 0;
    int keywordSeen[MAX_KEYWORDS] = {0};
    int inString = 0;
    int inComment = 0;
    int i, j;

    // Prompt user for input file name
    printf("Enter the name of the input file: ");
    scanf("%s", input_file_name);

    input_file = fopen(input_file_name, "r");

    if (input_file == NULL) {
        printf("Error: Unable to open input file.\n");
        return 1;
    }

    // Create output file name based on input file name
    strcpy(output_file_name, "output_");
    strcat(output_file_name, input_file_name);

    output_file = fopen(output_file_name, "w");

    if (output_file == NULL) {
        printf("Error: Unable to create output file.\n");
        fclose(input_file);
        return 1;
    }

    while ((ch = fgetc(input_file)) != EOF) {
        // Check for start of comment
        if (!inComment && !inString && ch == '/') {
            char nextChar = fgetc(input_file);
            if (nextChar == '/') {
                inComment = 1;
            } else {
                ungetc(nextChar, input_file);
            }
        }

        // Toggle inString flag when encountering double quotes
        if (ch == '"' && !inComment) {
            inString = !inString;
        }

        if (inComment) {
            if (ch == '\n') {
                inComment = 0;
            }
            fputc(ch, output_file);
        } else {
            if (isalnum(ch) || ch == '_') {
                j = 0;
                do {
                    word[j++] = ch;
                    ch = fgetc(input_file);
                } while ((isalnum(ch) || ch == '_') && ch != EOF);
                word[j] = '\0';

                // Convert word to lowercase for case-insensitive comparison
                for (i = 0; word[i]; i++) {
                    word[i] = tolower(word[i]);
                }

                if (!inString && is_keyword(word)) {
                    for (i = 0; i < MAX_KEYWORDS; i++) {
                        if (strcmp(word, keywords[i]) == 0) {
                            keywordCount[i]++;
                            if (!keywordSeen[i]) {
                                uniqueKeywordCount++;
                                keywordSeen[i] = 1;
                            }
                            break;
                        }
                    }
                } else {
                    fprintf(output_file, "%s", word);
                }

                if (ch != EOF) {
                    fprintf(output_file, "%c", ch);
                }
            } else {
                fputc(ch, output_file);
            }
        }
    }

    fclose(input_file);
    fclose(output_file);

    // Output counts for non-zero occurrences
    printf("Keyword Counts:\n");
    for (i = 0; i < MAX_KEYWORDS; i++) {
        if (keywordCount[i] > 0) {
            printf("%s : %d\n", keywords[i], keywordCount[i]);
        }
    }
    printf("Count of unique keywords = %d\n", uniqueKeywordCount);

    printf("Keywords detected successfully.\n");

    return 0;
}
