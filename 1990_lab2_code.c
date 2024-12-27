// Step 1: Determine the goal of the algorithm.

#include <stdio.h>
#include <string.h>

// Function to check if a line is a comment
int isComment(const char *line) {
    while (*line == ' ' || *line == '\t')  // Skip leading whitespace
        line++;

    if (strncmp(line, "//", 2) == 0)  // Single line comment
        return 1;

    if (strncmp(line, "/*", 2) == 0)  // Multi-line comment start
        return 1;

    if (strstr(line, "*/") != NULL)  // Multi-line comment end
        return 1;

    return 0;  // Not a comment
}

int main() {
    // Step 2: Access historic and current data.
    FILE *sourceFile, *destinationFile;
    char sourceFileName[100], destinationFileName[100], line[1000];

    // Step 3: Choose the right models.
    printf("Source file: ");
    scanf("%s", sourceFileName);

    sourceFile = fopen(sourceFileName, "r");

    if (sourceFile == NULL) {
        perror("Error opening source file");
        return 1;
    }

    printf("Destination file: ");
    scanf("%s", destinationFileName);

    destinationFile = fopen(destinationFileName, "w");

    if (destinationFile == NULL) {
        perror("Error opening destination file");
        fclose(sourceFile);
        return 1;
    }

    // Step 4: Fine tuning.
    while (fgets(line, sizeof(line), sourceFile) != NULL) {
        if (!isComment(line)) {
            fputs(line, destinationFile);
        }
    }

    fclose(sourceFile);
    fclose(destinationFile);

    // Step 5: Visualize your results.
    printf("Comments removed successfully.\n");

    // Step 6: Running your algorithm continuously.
    return 0;
}
