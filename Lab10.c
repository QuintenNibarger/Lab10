#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALPHABET_SIZE 26

// Trie structure
struct Trie {
    int count;
    struct Trie *children[ALPHABET_SIZE];
};

// Function to create a new Trie node
struct Trie *createTrie() {
    struct Trie *node = (struct Trie *) malloc(sizeof(struct Trie));
    if (node) {
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts the word into the Trie
void insert(struct Trie *pTrie, const char *word) {
    struct Trie *curr = pTrie;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL) {
            curr->children[index] = createTrie();
        }
        curr = curr->children[index];
    }
    curr->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, const char *word) {
    struct Trie *curr = pTrie;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL) {
            return 0;
        }
        curr = curr->children[index];
    }
    return curr->count;
}

// Deallocates the Trie structure
void deallocateTrie(struct Trie *pTrie) {
    if (pTrie == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (pTrie->children[i] != NULL) {
            deallocateTrie(pTrie->children[i]);
        }
    }
    free(pTrie);
}

// Reads words from a file into an array of strings
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }

    char buffer[1024];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file) && count < 256) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        pInWords[count] = strdup(buffer);
        count++;
    }

    fclose(file);
    return count;
}

int main(void) {
    char *inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);

    // Free dynamically allocated dictionary words
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}
