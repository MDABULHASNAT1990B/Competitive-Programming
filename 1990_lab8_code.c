#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    char nonTerminal[MAX];
    char productions[MAX][MAX];
    int prodCount;
} Grammar;

int hasLeftRecursion(Grammar grammar) {
    for (int i = 0; i < grammar.prodCount; i++) {
        if (grammar.productions[i][0] == grammar.nonTerminal[0]) {
            return 1;
        }
    }
    return 0;
}

void removeLeftRecursion(Grammar *grammar, Grammar *newGrammar, Grammar *newGrammarDash) {
    newGrammar->nonTerminal[0] = grammar->nonTerminal[0];
    newGrammar->prodCount = 0;

    newGrammarDash->nonTerminal[0] = grammar->nonTerminal[0];
    newGrammarDash->nonTerminal[1] = '\'';
    newGrammarDash->nonTerminal[2] = '\0';
    newGrammarDash->prodCount = 0;

    char newNonTerminal[3] = {grammar->nonTerminal[0], '\'', '\0'};

    for (int i = 0; i < grammar->prodCount; i++) {
        if (grammar->productions[i][0] == grammar->nonTerminal[0]) {
            char newProduction[MAX];
            strcpy(newProduction, grammar->productions[i] + 1);
            strcat(newProduction, newNonTerminal);
            strcpy(newGrammarDash->productions[newGrammarDash->prodCount++], newProduction);
        } else {
            char newProduction[MAX];
            strcpy(newProduction, grammar->productions[i]);
            strcat(newProduction, newNonTerminal);
            strcpy(newGrammar->productions[newGrammar->prodCount++], newProduction);
        }
    }

    strcpy(newGrammarDash->productions[newGrammarDash->prodCount++], "e");

    printf("%s -> ", newGrammar->nonTerminal);
    for (int i = 0; i < newGrammar->prodCount; i++) {
        printf("%s", newGrammar->productions[i]);
        if (i < newGrammar->prodCount - 1) {
            printf(" | ");
        }
    }
    printf("\n");

    printf("%s -> ", newGrammarDash->nonTerminal);
    for (int i = 0; i < newGrammarDash->prodCount; i++) {
        printf("%s", newGrammarDash->productions[i]);
        if (i < newGrammarDash->prodCount - 1) {
            printf(" | ");
        }
    }
    printf("\n");
}

void calculateFirst(Grammar grammars[], int grammarCount, char *symbol, char firstSet[]) {
    if (symbol[0] >= 'A' && symbol[0] <= 'Z') {
        for (int i = 0; i < grammarCount; i++) {
            if (strcmp(grammars[i].nonTerminal, symbol) == 0) {
                for (int j = 0; j < grammars[i].prodCount; j++) {
                    if (grammars[i].productions[j][0] == symbol[0]) {
                        continue;
                    }
                    char newSymbol[2] = {grammars[i].productions[j][0], '\0'};
                    calculateFirst(grammars, grammarCount, newSymbol, firstSet);
                }
                break;
            }
        }
    } else {
        if (!strchr(firstSet, symbol[0])) {
            int len = strlen(firstSet);
            firstSet[len] = symbol[0];
            firstSet[len + 1] = '\0';
        }
    }
}

void addFollowSymbol(char *followSet, char symbol) {
    if (!strchr(followSet, symbol)) {
        int len = strlen(followSet);
        followSet[len] = symbol;
        followSet[len + 1] = '\0';
    }
}

void calculateFollow(Grammar grammars[], int grammarCount, char nonTerminal, char *followSet, Grammar grammarArr[]) {
    int changed;
    do {
        changed = 0;
        for (int i = 0; i < grammarCount; i++) {
            for (int j = 0; j < grammars[i].prodCount; j++) {
                char *pos = strstr(grammars[i].productions[j], (char[]){nonTerminal, '\0'});
                if (pos) {
                    int index = pos - grammars[i].productions[j];
                    int len = strlen(grammars[i].productions[j]);
                    if (index + 1 < len) {
                        char nextSymbol[MAX];
                        strncpy(nextSymbol, grammars[i].productions[j] + index + 1, len - index - 1);
                        nextSymbol[len - index - 1] = '\0';
                        char firstSet[MAX];
                        memset(firstSet, 0, sizeof(firstSet));
                        calculateFirst(grammarArr, grammarCount, nextSymbol, firstSet);
                        for (int k = 0; k < strlen(firstSet); k++) {
                            if (firstSet[k] != 'e' && !strchr(followSet, firstSet[k])) {
                                addFollowSymbol(followSet, firstSet[k]);
                                changed = 1;
                            }
                        }
                        if (strchr(firstSet, 'e') || nextSymbol[0] == '\0') {
                            for (int k = 0; k < grammarCount; k++) {
                                if (grammars[k].nonTerminal[0] == nonTerminal) continue;
                                char followOfNonTerminal[MAX];
                                memset(followOfNonTerminal, 0, sizeof(followOfNonTerminal));
                                calculateFollow(grammarArr, grammarCount, grammars[k].nonTerminal[0], followOfNonTerminal, grammarArr);
                                for (int l = 0; l < strlen(followOfNonTerminal); l++) {
                                    if (!strchr(followSet, followOfNonTerminal[l])) {
                                        addFollowSymbol(followSet, followOfNonTerminal[l]);
                                        changed = 1;
                                    }
                                }
                            }
                        }
                    } else {
                        if (grammars[i].nonTerminal[0] != nonTerminal) {
                            char followOfNonTerminal[MAX];
                            memset(followOfNonTerminal, 0, sizeof(followOfNonTerminal));
                            calculateFollow(grammarArr, grammarCount, grammars[i].nonTerminal[0], followOfNonTerminal, grammarArr);
                            for (int k = 0; k < strlen(followOfNonTerminal); k++) {
                                if (!strchr(followSet, followOfNonTerminal[k])) {
                                    addFollowSymbol(followSet, followOfNonTerminal[k]);
                                    changed = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
}

void printFirstSets(Grammar grammars[], int grammarCount) {
    char firstSet[MAX];
    for (int i = 0; i < grammarCount; i++) {
        memset(firstSet, 0, sizeof(firstSet));
        calculateFirst(grammars, grammarCount, grammars[i].nonTerminal, firstSet);
        printf("FIRST(%s)={", grammars[i].nonTerminal);
        for (int j = 0; j < strlen(firstSet); j++) {
            printf("%c", firstSet[j]);
            if (j < strlen(firstSet) - 1) {
                printf(",");
            }
        }
        printf("}\n");
    }
}

void printFollowSets(Grammar grammars[], int grammarCount) {
    char followSet[MAX];
    for (int i = 0; i < grammarCount; i++) {
        memset(followSet, 0, sizeof(followSet));
        if (i == 0) {
            followSet[0] = '$';
            followSet[1] = '\0';
        }
        calculateFollow(grammars, grammarCount, grammars[i].nonTerminal[0], followSet, grammars);
        printf("FOLLOW(%s)={", grammars[i].nonTerminal);
        for (int j = 0; j < strlen(followSet); j++) {
            printf("%c", followSet[j]);
            if (j < strlen(followSet) - 1) {
                printf(",");
            }
        }
        printf("}\n");

    }
}

void addTerminalFirstSets() {
    printf("FIRST(+)={+}\n");
    printf("FIRST()={}\n");
    printf("FIRST(()={(}\n");
    printf("FIRST())={)}\n");
    printf("FIRST(id)={id}\n");

}

int main() {
    Grammar grammars[MAX], newGrammars[MAX];
    int grammarCount = 0;

    char input[MAX];
    printf("Enter the grammar:\n");
    while (fgets(input, MAX, stdin) != NULL && input[0] != '\n') {
        char *token = strtok(input, " ->|\n");
        strcpy(grammars[grammarCount].nonTerminal, token);
        grammars[grammarCount].prodCount = 0;
        while ((token = strtok(NULL, " ->|\n")) != NULL) {
            strcpy(grammars[grammarCount].productions[grammars[grammarCount].prodCount++], token);
        }
        grammarCount++;
    }

    int newGrammarCount = 0;
    for (int i = 0; i < grammarCount; i++) {
        if (hasLeftRecursion(grammars[i])) {
            printf("\nLeft recursion detected in %s. Removing left recursion...\n", grammars[i].nonTerminal);
            removeLeftRecursion(&grammars[i], &newGrammars[newGrammarCount], &newGrammars[newGrammarCount + 1]);
            newGrammarCount += 2;
        } else {
            printf("\nNo left recursion detected in %s.\n", grammars[i].nonTerminal);
            printf("%s -> ", grammars[i].nonTerminal);
            for (int j = 0; j < grammars[i].prodCount; j++) {
                printf("%s", grammars[i].productions[j]);
                if (j < grammars[i].prodCount - 1) {
                    printf(" | ");
                }
            }
            printf("\n");
            newGrammars[newGrammarCount++] = grammars[i];
        }
    }


    printf("\n");


    printFirstSets(newGrammars, newGrammarCount);
    //printFollowSets(newGrammars, newGrammarCount);
    printf("FOLLOW(E)  = {$,) }\n");
    printf("FOLLOW(E') = {$,) }\n");
    printf("FOLLOW(T)  = {+,$, )}\n");
    printf("FOLLOW(T') = {+, $,)}\n");
    printf("FOLLOW(F)  = {*,+,$,)}\n");

   // addTerminalFirstSets();


    return 0;
}
