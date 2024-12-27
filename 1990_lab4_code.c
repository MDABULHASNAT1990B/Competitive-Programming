#include <stdio.h>
#include <string.h>

int matchPattern(const char *pattern, const char *str) {
    if (*pattern == '\0') {
        return 1;  // Empty pattern matches any string
    }

    if (*pattern == '*' && *(pattern + 1) == '\0') {
        return 1;  // '*' matches empty string
    }

    if (*pattern == '\0' || *str == '\0') {
        return 0;  // Pattern or string ended
    }

    if (*pattern == *str || *pattern == '.') {
        return matchPattern(pattern + 1, str + 1);
    }

    if (*pattern == '*') {
        return matchPattern(pattern + 1, str) || matchPattern(pattern, str + 1);
    }

    if (*pattern == '|') {
        return matchPattern(pattern + 1, str) || matchPattern(pattern + 3, str);
    }

    if (*pattern == '(') {
        const char *endBracket = strchr(pattern, ')');
        if (endBracket != NULL) {
            return (matchPattern(pattern + 1, str) || matchPattern(endBracket + 1, str)) && matchPattern(endBracket + 1, str);
        }
    }

    return 0;
}

int main() {
    char pattern[100], str[100];

    printf("Enter a pattern: ");
    scanf("%s", pattern);

    printf("Enter a string: ");
    scanf("%s", str);

    if (matchPattern(pattern, str)) {
        printf("Matched\n");
    } else {
        printf("Not Matched\n");
    }

    return 0;
}
