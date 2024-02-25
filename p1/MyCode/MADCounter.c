#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define UsageErrorMessage "USAGE:\n\t./MADCounter -f <input file> -o <output file> -c -w -l -Lw -Ll\n\t\tOR\n\t./MADCounter -B <batch file>\n"

typedef struct word {
    char *contents;
    int numChars;
    int frequency;
    int initialPosition;
    int height;
    struct word *left, *right;
} WORD;

// Function declarations
int processSingleCommand(int argc, char *argv[]);
int processBatchCommand(int argc, char *argv[]);
int trackCharacters(FILE *inputFile, FILE *outputFile);
int trackWords(FILE *inputFile, FILE *outputFile);
int trackLines(FILE *inputFile, FILE *outputFile);
int trackLongestWord(FILE *inputFile, FILE *outputFile);
int trackLongestLine(FILE *inputFile, FILE *outputFile);
// Function declarations for AVL tree operations
int height(WORD *N);
int max(int a, int b);
WORD* newNode(char *word, int position);
WORD* rightRotate(WORD *y);
WORD* leftRotate(WORD *x);
int getBalance(WORD *N);
WORD* insert(WORD *node, char *word, int position, int *uniqueWords);
void inOrder(WORD *root, FILE *outputFile);
void inOrderLines(WORD *root, FILE *outputFile);
// Function declarations for sorting
int compareStrings(const void *a, const void *b);


int main(int argc, char *argv[]) {

    // Usage Error - Print when less than 3 arguments are provided.
    if (argc < 3) {
        printf( UsageErrorMessage );
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-B") == 0) {
        // Batch Command Mode
        return processBatchCommand(argc, argv);
    }
    else {
        return processSingleCommand(argc, argv);
    }
    return 0;

}

int processSingleCommand(int argc, char *argv[]) {

    // Usage Error - Print when less than 3 arguments are provided.
    if (argc < 3) {
        printf( UsageErrorMessage );
        return EXIT_FAILURE;
    }

    // Check for invalid flags
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0)
            continue;
        else if (strcmp(argv[i], "-o") == 0)
            continue;
        else if (strcmp(argv[i], "-c") == 0)
            continue;
        else if (strcmp(argv[i], "-w") == 0)
            continue;
        else if (strcmp(argv[i], "-l") == 0)
            continue;
        else if (strcmp(argv[i], "-Lw") == 0)
            continue;
        else if (strcmp(argv[i], "-Ll") == 0)
            continue;
        // argv[i] is a filename
        else if (strcmp(argv[i-1], "-f") == 0)
            continue;
        else if (strcmp(argv[i-1], "-o") == 0)
            continue;
        else {
            // Invalid Flag - All flags should begin with a -, and only flags
            // should begin with this. If there is a flag that we did specify
            // elsewhere in this doc then print this error.
            printf("ERROR: Invalid Flag Types\n");
            return EXIT_FAILURE;
        }
    }

    // Input and output file flags are not guaranteed to appear in 
    // any particular order. So we need to search for them first.

    // Check for input file
    char *inputFileName = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
            inputFileName = argv[i + 1];
    }
    if (inputFileName == NULL || inputFileName[0] == '-') {
        // NO Input File Provided - If the -f flag wasn't specified
        // or it the -f flag is immediately followed by another flag.
        printf("ERROR: No Input File Provided\n");
        return EXIT_FAILURE;
    }
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        // Can't Open Input File - If the is an error in opening the input file.
        printf("ERROR: Can't open input file\n");
        return EXIT_FAILURE;
    }

    // Check for output file
    char *outputFileName = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            // No Output File - If the -o flag was specified
            // but is immediately followed by another flag.
            if (i + 1 >= argc) {
                printf("ERROR: No Output File Provided\n");
                return EXIT_FAILURE;
            }
            outputFileName = argv[i + 1];
            if (outputFileName[0] == '-') {
                printf("ERROR: No Output File Provided\n");
                return EXIT_FAILURE;
            }
        }
    }

    // Input File Empty - If there nothing in the input file.
    int c = fgetc(inputFile);
    if (c == EOF) {
        printf("ERROR: Input File Empty\n");
        return EXIT_FAILURE;
    }
    fclose(inputFile);

    // Open output file
    FILE *outputFile = NULL;
    if (outputFileName != NULL)
        outputFile = fopen(outputFileName, "w");

    // Now we can start processing the remaining flags.
    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-c") == 0) {
            // Add a new line if previous flag was also statistics
            if (argv[i-1][0] == '-') {
                if (outputFile == NULL)
                    printf("\n");
                else
                    fprintf(outputFile, "\n");
            }
            // Track Characters
            FILE *inputFile = fopen(inputFileName, "r");
            trackCharacters(inputFile, outputFile);
            fclose(inputFile);
        }

        else if (strcmp(argv[i], "-w") == 0) {
            // Add a new line if previous flag was also statistics
            if (argv[i-1][0] == '-') {
                if (outputFile == NULL)
                    printf("\n");
                else
                    fprintf(outputFile, "\n");
            }
            // Track Words
            FILE *inputFile = fopen(inputFileName, "r");
            trackWords(inputFile, outputFile);
            fclose(inputFile);
        }

        else if (strcmp(argv[i], "-l") == 0) {
            // Add a new line if previous flag was also statistics
            if (argv[i-1][0] == '-') {
                if (outputFile == NULL)
                    printf("\n");
                else
                    fprintf(outputFile, "\n");
            }
            // Track Lines
            FILE *inputFile = fopen(inputFileName, "r");
            trackLines(inputFile, outputFile);
            fclose(inputFile);
        }

        else if (strcmp(argv[i], "-Lw") == 0) {
            // Add a new line if previous flag was also statistics
            if (argv[i-1][0] == '-') {
                if (outputFile == NULL)
                    printf("\n");
                else
                    fprintf(outputFile, "\n");
            }
            // Track Longest Word
            FILE *inputFile = fopen(inputFileName, "r");
            trackLongestWord(inputFile, outputFile);
            fclose(inputFile);
        }

        else if (strcmp(argv[i], "-Ll") == 0) {
            // Add a new line if previous flag was also statistics
            if (argv[i-1][0] == '-') {
                if (outputFile == NULL)
                    printf("\n");
                else
                    fprintf(outputFile, "\n");
            }
            // Track Longest Line
            FILE *inputFile = fopen(inputFileName, "r");
            trackLongestLine(inputFile, outputFile);
            fclose(inputFile);
        }

    }

    if (outputFile != NULL)
        fclose(outputFile);
    
    return EXIT_SUCCESS;

}

int processBatchCommand(int argc, char *argv[]) {

    char *batchFileName = argv[2];
    FILE *batchFile = fopen(batchFileName, "r");

    // Can't Open Batch File - If the is an error in opening the batch file.
    if (batchFile == NULL) {
        printf("ERROR: Can't open batch file\n");
        return EXIT_FAILURE;
    }

    // Batch File Empty - If there's nothing in the batch file.
    int c = fgetc(batchFile);
    if (c == EOF) {
        printf("ERROR: Batch File Empty\n");
        return EXIT_FAILURE;
    }
    ungetc(c, batchFile);

    char *line = malloc(MAX_LINE_LENGTH * sizeof(char));
    
    // Process each line in the batch file.
    while (fgets(line, MAX_LINE_LENGTH, batchFile) != NULL) {

        // Tokenize the line into arguments
        char *batchArgv[MAX_LINE_LENGTH];

        // First argument is the program name
        batchArgv[0] = strdup("MADCounter");

        int batchArgc = 1;
        char *token = strtok(line, " \t\n");
        while (token != NULL) {
            batchArgv[batchArgc++] = strdup(token);
            token = strtok(NULL, " \t\n");
        }

        // Process the command
        processSingleCommand(batchArgc, batchArgv);
    }

    fclose(batchFile);
    free(line);
    return EXIT_SUCCESS;

}

/* 
 * -c : This flag means you should track each Ascii character 0-127.
 * You should track how many times each character is used and the initial
 * zero based position the character occurred in. You should also track
 * the total number of chars and number of unique chars. When you print
 * the output it should be in the following format, with a line for each
 * character, and should only include characters you actually encountered
 * in the document.
 * 
 * Example: Ascii Value: 33, Char: !, Count: 1, Initial Position: 16
 * 
 */
int trackCharacters(FILE *inputFile, FILE *outputFile) {

    int charCount = 0;
    int uniqueCharCount = 0;
    int c;
    int charFrequency[128] = {0};
    int initialPosition[128] = {0};

    while ((c = fgetc(inputFile)) != EOF) {
        if (c < 0 || c > 127) {
            printf("ERROR: Detecting Ascii Character %c at position %d\n", c, charCount);
            // Ignore non-ascii characters
            continue;
        }
        if (charFrequency[c] == 0) {
            // First time seeing this character
            uniqueCharCount++;
            initialPosition[c] = charCount;
        }
        charFrequency[c]++;
        charCount++;  // The positions are zero based
    }

    if (outputFile == NULL) {
        printf("Total Number of Chars = %d\n", charCount);
        printf("Total Unique Chars = %d\n\n", uniqueCharCount);
    }
    else {
        fprintf(outputFile, "Total Number of Chars = %d\n", charCount);
        fprintf(outputFile, "Total Unique Chars = %d\n\n", uniqueCharCount);
    }

    for (int i = 0; i < 128; i++) {
        // Skip characters that were never encountered
        if (charFrequency[i] == 0)
            continue;
        if (outputFile == NULL)
            printf("Ascii Value: %d, Char: %c, Count: %d, Initial Position: %d\n",
              i, i, charFrequency[i], initialPosition[i]);
        else
            fprintf(outputFile, "Ascii Value: %d, Char: %c, Count: %d, Initial Position: %d\n",
              i, i, charFrequency[i], initialPosition[i]);
    }

    return EXIT_SUCCESS;

}

// Utility functions
int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(WORD *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

WORD* newNode(char *word, int position) {
    WORD* node = (WORD*) malloc(sizeof(WORD));
    node->contents = strdup(word);
    node->numChars = strlen(word);
    node->frequency = 1;
    node->initialPosition = position;
    node->height = 1; // new node is initially added at leaf
    node->left = NULL;
    node->right = NULL;
    return(node);
}

// Right rotate subtree rooted with y
WORD* rightRotate(WORD *y) {
    WORD *x = y->left;
    WORD *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

// Left rotate subtree rooted with x
WORD* leftRotate(WORD *x) {
    WORD *y = x->right;
    WORD *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int getBalance(WORD *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Recursive function to insert a word in the subtree rooted
// with node and returns the new root of the subtree.
WORD* insert(WORD *node, char *word, int position, int *uniqueWords) {
    // 1. Perform the normal BST insertion
    if (node == NULL) {
        (*uniqueWords)++;
        return(newNode(word, position));
    }

    if (strcmp(word, node->contents) < 0)
        node->left = insert(node->left, word, position, uniqueWords);
    else if (strcmp(word, node->contents) > 0)
        node->right = insert(node->right, word, position, uniqueWords);
    else {
        // Same word; increment frequency
        node->frequency++;
        return node;
    }

    // 2. Update height of this ancestor node
    node->height = 1 + max(height(node->left), height(node->right));

    // 3. Get the balance factor of this ancestor node to check whether
    // this node became unbalanced
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && strcmp(word, node->left->contents) < 0)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && strcmp(word, node->right->contents) > 0)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && strcmp(word, node->left->contents) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && strcmp(word, node->right->contents) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the (unchanged) node pointer
    return node;
}

// A utility function to print inorder traversal of the tree.
void inOrder(WORD *root, FILE *outputFile) {
    if (root != NULL) {
        inOrder(root->left, outputFile);
        if (outputFile == NULL)
            printf("Word: %s, Freq: %d, Initial Position: %d\n",
              root->contents, root->frequency, root->initialPosition);
        else
            fprintf(outputFile, "Word: %s, Freq: %d, Initial Position: %d\n",
              root->contents, root->frequency, root->initialPosition);
        inOrder(root->right, outputFile);
    }
}

// For lines
void inOrderLines(WORD *root, FILE *outputFile) {
    if (root != NULL) {
        inOrderLines(root->left, outputFile);
        if (outputFile == NULL)
            printf("Line: %s, Freq: %d, Initial Position: %d\n",
              root->contents, root->frequency, root->initialPosition);
        else
            fprintf(outputFile, "Line: %s, Freq: %d, Initial Position: %d\n",
              root->contents, root->frequency, root->initialPosition);
        inOrderLines(root->right, outputFile);
    }
}

/*
 * -w : This flag means you should track each whitespace separated string.
 * You should track how many times each string is used and the initial 
 * zero-based position the string occurred in. This will be the string 
 * position, not the character position. You should also track the total
 * number of words and number of unique words. When you print the output
 * it should be in the following format for each string.
 * 
 * Example:
 * Total Number of Words: <number of words>
 * Total Unique Words: <number unique words>
 *
 * Word: <string>, Freq: <freq int>, Initial Position: <position int>
 * 
 */
int trackWords(FILE *inputFile, FILE *outputFile) {

    WORD *root = NULL;
    char buffer[MAX_LINE_LENGTH];
    int position = 0;  // Total words = position
    int uniqueWords = 0;
    
    while (fscanf(inputFile, "%s", buffer) != EOF) {
        root = insert(root, buffer, position, &uniqueWords);
        position++;
    }

    if (outputFile == NULL) {
        printf("Total Number of Words: %d\n", position);
        printf("Total Unique Words: %d\n\n", uniqueWords);
    }
    else {
        fprintf(outputFile, "Total Number of Words: %d\n", position);
        fprintf(outputFile, "Total Unique Words: %d\n\n", uniqueWords);
    }

    inOrder(root, outputFile);

    return EXIT_SUCCESS;

}

/*
 * -l : This flag means you should track each newline separated line.
 * You should track how many times each line is used and the initial
 * zero-based position the line occurred in. This will be the line number
 * of the first occurrence of the line, not the character position.
 * You should also track the total number of lines and number of unique lines.
 * When you print the output it should be in the following format, with a
 * line for each string. When you print the lines, you should remove any
 * trailing newline characters.
 */
int trackLines(FILE *inputFile, FILE *outputFile) {

    WORD *root = NULL;
    char buffer[MAX_LINE_LENGTH];
    int position = 0;  // Total lines = position
    int uniqueLines = 0;
    
    while (fgets(buffer, MAX_LINE_LENGTH, inputFile) != NULL) {

        // Remove newline character, if present
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        root = insert(root, buffer, position, &uniqueLines);
        position++;

    }

    if (outputFile == NULL) {
        printf("Total Number of Lines: %d\n", position);
        printf("Total Unique Lines: %d\n\n", uniqueLines);
    }
    else {
        fprintf(outputFile, "Total Number of Lines: %d\n", position);
        fprintf(outputFile, "Total Unique Lines: %d\n\n", uniqueLines);
    }

    inOrderLines(root, outputFile);

    return EXIT_SUCCESS;

}

int compareStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

/*
 * -Lw : This flag means you should keep track of the longest word(s) and
 * how long the string(s) is. You will print the output in the following
 * format. If there is more than one word with the same length than print
 * them in an Ascii alphabetically sorted list.
 */
int trackLongestWord(FILE *inputFile, FILE *outputFile) {

    char buffer[MAX_LINE_LENGTH];
    int maxWordLength = 0;
    char **longestWords = NULL;
    int longestWordsCount = 0;
    
    // First pass: find maximum word length
    while (fscanf(inputFile, "%s", buffer) != EOF) {
        int len = strlen(buffer);
        if (len > maxWordLength)
            maxWordLength = len;
    }

    if (outputFile == NULL)
        printf("Longest Word is %d characters long:\n", maxWordLength);
    else
        fprintf(outputFile, "Longest Word is %d characters long:\n", maxWordLength);

    // Second pass: find number of words of maximum length
    rewind(inputFile);
    while (fscanf(inputFile, "%s", buffer) != EOF) {
        int len = strlen(buffer);
        if (len == maxWordLength)
            longestWordsCount++;
    }

    // Allocate memory for storing longest words
    longestWords = malloc(sizeof(char*) * longestWordsCount);
    if (longestWords == NULL) {
        printf("Memory allocation failed for longestWords\n");
        return EXIT_FAILURE;
    }

    // Third pass: find all words of maximum length
    rewind(inputFile);
    int i = 0;
    while (fscanf(inputFile, "%s", buffer) != EOF) {
        int len = strlen(buffer);
        if (len == maxWordLength)
            longestWords[i++] = strdup(buffer);
    }

    // Sort the words
    qsort(longestWords, longestWordsCount, sizeof(char*), compareStrings);

    // Print the words
    for (int i = 0; i < longestWordsCount; i++) {
        if ( i > 0 && strcmp(longestWords[i], longestWords[i-1]) == 0)
            continue;
        if (outputFile == NULL)
            printf("\t%s\n", longestWords[i]);
        else
            fprintf(outputFile, "\t%s\n", longestWords[i]);
    }

    for (int i = 0; i < longestWordsCount; i++)
        free(longestWords[i]);

    free(longestWords);

    return EXIT_SUCCESS;

}

int trackLongestLine(FILE *inputFile, FILE *outputFile) {

    char buffer[MAX_LINE_LENGTH];
    int maxLineLength = 0;
    char **longestLines = NULL;
    int longestLinesCount = 0;
    
    // First pass: find maximum line length
    while (fgets(buffer, MAX_LINE_LENGTH, inputFile) != NULL) {
        
        // Remove newline character, if present
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        if (len > maxLineLength)
            maxLineLength = len;
    }

    if (outputFile == NULL)
        printf("Longest Line is %d characters long:\n", maxLineLength);
    else
        fprintf(outputFile, "Longest Line is %d characters long:\n", maxLineLength);

    // Second pass: find number of lines of maximum length
    rewind(inputFile);
    while (fgets(buffer, MAX_LINE_LENGTH, inputFile) != NULL) {
        
        // Remove newline character, if present
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        if (len == maxLineLength)
            longestLinesCount++;
    }

    // Allocate memory for storing longest lines
    longestLines = malloc(sizeof(char*) * longestLinesCount);
    if (longestLines == NULL) {
        printf("Memory allocation failed for longestLines\n");
        return EXIT_FAILURE;
    }
    
    // Third pass: find all lines of maximum length
    rewind(inputFile);
    int i = 0;
    while (fgets(buffer, MAX_LINE_LENGTH, inputFile) != NULL) {
        
        // Remove newline character, if present
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        if (len == maxLineLength)
            longestLines[i++] = strdup(buffer);
    }

    // Sort the lines
    qsort(longestLines, longestLinesCount, sizeof(char*), compareStrings);

    // Print the lines
    for (int i = 0; i < longestLinesCount; i++) {
        if ( i > 0 && strcmp(longestLines[i], longestLines[i-1]) == 0)
            continue;
        if (outputFile == NULL)
            printf("\t%s\n", longestLines[i]);
        else
            fprintf(outputFile, "\t%s\n", longestLines[i]);
    }

    for (int i = 0; i < longestLinesCount; i++)
        free(longestLines[i]);

    free(longestLines);

    return EXIT_SUCCESS;

}