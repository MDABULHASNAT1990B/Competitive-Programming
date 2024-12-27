#include <stdio.h>
#include <string.h>

#define SIZE 100
#define MAX_PRODUCTIONS 10

void leftFactor(char productions[MAX_PRODUCTIONS][SIZE], int num) {
    int i;
    for (i = 0; i < num; i++) {
        char non_terminal = productions[i][0];
        char* options[MAX_PRODUCTIONS];
        int option_count = 0;
        int j, prefix_length;

        printf("\n\nGRAMMAR: %s", productions[i]);

        char* token = strtok(productions[i] + 3, "|");
        while (token != NULL) {
            options[option_count++] = token;
            token = strtok(NULL, "|");
        }

        if (option_count > 1) {
            prefix_length = 0;
            while (1) {
                char current_char = options[0][prefix_length];
                int all_match = 1;

                for (j = 1; j < option_count; j++) {
                    if (options[j][prefix_length] != current_char) {
                        all_match = 0;
                        break;
                    }
                }

                if (all_match && current_char != '\0') {
                    prefix_length++;
                } else {
                    break;
                }
            }

            if (prefix_length > 0) {
                char common_prefix[SIZE];
                char new_non_terminal[3];
                strncpy(common_prefix, options[0], prefix_length);
                common_prefix[prefix_length] = '\0';
                new_non_terminal[0] = non_terminal;
                new_non_terminal[1] = '\'';
                new_non_terminal[2] = '\0';

                printf("\n\nGrammar after left factoring:\n");
                printf("%c -> %s%s\n", non_terminal, common_prefix, new_non_terminal);
                printf("%s -> ", new_non_terminal);

                for (j = 0; j < option_count; j++) {
                    if (strlen(options[j] + prefix_length) > 0) {
                        printf("%s", options[j] + prefix_length);
                    } else {
                        printf("e"); // Replace empty string with 'e'
                    }
                    if (j < option_count - 1) {
                        printf(" | ");
                    }
                }
                printf("\n");
                continue;
            }
        }
        printf("\n\n%s has no common prefix.\n", productions[i]);
    }
}

int main() {
    char productions[MAX_PRODUCTIONS][SIZE];
    int num, i;

    printf("Enter Number of Productions: ");
    scanf("%d", &num);
    getchar();

    printf("Enter the grammar:\n");
    for (i = 0; i < num; i++) {
        fgets(productions[i], SIZE, stdin);
        productions[i][strcspn(productions[i], "\n")] = '\0';
    }

    leftFactor(productions, num);

    return 0;
}
