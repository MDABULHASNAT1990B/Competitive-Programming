#include <stdio.h>
#include <string.h>

#define SIZE 100
#define MAX_PRODUCTIONS 10

void removeLeftRecursion(char productions[MAX_PRODUCTIONS][SIZE], int num) {
    int i, j;
    for (i = 0; i < num; i++) {
        char non_terminal = productions[i][0];
        char alpha[MAX_PRODUCTIONS][SIZE];
        char beta[MAX_PRODUCTIONS][SIZE];
        int alpha_count = 0, beta_count = 0;


        char production_copy[SIZE];
        strcpy(production_copy, productions[i]);

        printf("\nGRAMMAR: %s", productions[i]);


        char* token = strtok(production_copy + 3, "|");
        while (token != NULL) {
            if (token[0] == non_terminal) {

                strcpy(alpha[alpha_count++], token + 1);
            } else {

                strcpy(beta[beta_count++], token);
            }
            token = strtok(NULL, "|");
        }

        if (alpha_count > 0) {
            printf(" is left recursive.\n");
            printf("Grammar without left recursion:\n");


            for (j = 0; j < beta_count; j++) {
                printf("%c -> %s%c'\n", non_terminal, beta[j], non_terminal);
            }


            printf("%c' -> ", non_terminal);
            for (j = 0; j < alpha_count; j++) {
                printf("%s%c'", alpha[j], non_terminal);
                if (j < alpha_count - 1) {
                    printf(" | ");
                }
            }
            printf(" e\n");
        } else {
            printf(" is not left recursive.\n");
        }
    }
}

int main() {
    char productions[MAX_PRODUCTIONS][SIZE];
    int num;
    int i;

    printf("Enter Number of Productions: ");
    scanf("%d", &num);
    getchar();

    printf("Enter the grammar:\n");
    for (i = 0; i < num; i++) {
        fgets(productions[i], SIZE, stdin);
        productions[i][strcspn(productions[i], "\n")] = '\0';
    }

    removeLeftRecursion(productions, num);

    return 0;
}
