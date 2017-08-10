/**
 * edits.c
 * 
 * This library consists of functions that operate over a given word and
 * add the resulting strings to a provided array.
 * 
 * Operation descriptions:
 *  - deletion: deletes only one letter of the word, starting from the
 * first one and ending in the last.
 * Example: "dog" >> { "og", "dg", "do" }
 * 
 *  - transposition: swaps adjacent pairs of letters.
 * Example: "dog" >> { "odg", "god", "dgo" }
 * 
 *  - alteration: replaces each letter of the word with a letter of the
 * alphabet, starting from the first letter, ending in the last one.
 * Example: assume alphabet is "adgoz"
 *          "dog" >> { "aog", "dog", "gog", "oog", "zog", "dag", ... }
 * 
 *  - insertion: inserts a letter of the alphabet between each adjacent
 * pair of letters in the word.
 * Example: assume alphabet is "adgoz"
 *          "dog" >> { "adog", "ddog", "gdog", "odog", "zdog", "daog",
 *                    ... }
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/* This function appends a set of characters from src to the correct
 * place in dst */
static void append(wchar_t *dst, int *dstLen, const wchar_t *src,
                   int srcBegin, int len) {
  if (len > 0) {
    wmemcpy(&dst[*dstLen], &src[srcBegin], len);
    *dstLen += len;
  }
  
  dst[*dstLen] = L'\0';
}

/* Calculates the space necessary for storing all the operations */
unsigned int calculate_edits_space(wchar_t *word, wchar_t *alphabet) {
  int wordLength = wcslen(word), alphabetSize = wcslen(alphabet);
  register unsigned int totalSize = 0;
  
  totalSize += wordLength;                            /* deletion */
  //printf("DEBUG > calculate_edits_space delet:  %u\n", totalSize);

  totalSize += (wordLength - 1);                      /* transps */
  //printf("DEBUG > calculate_edits_space transp: %u\n", totalSize);

  totalSize += (alphabetSize * wordLength);           /* alteration */
  //printf("DEBUG > calculate_edits_space alt:    %u\n", totalSize);
  
  totalSize += (alphabetSize * (wordLength + 1));     /* insertion */
  //printf("DEBUG > calculate_edits_space insert: %u\n", totalSize);
  
  return totalSize;
}

/* Performs deletions */
unsigned int edit_deletions(wchar_t *word, wchar_t **array, int start) {
  unsigned int i = 0;
  int wordLength = wcslen(word);
  
  for (; i < wordLength; i++) {
    int pos = 0;
    array[i + start] = malloc(sizeof(wchar_t) * wordLength);
    append(array[i + start], &pos, word, 0 , i);
    append(array[i + start], &pos, word, i + 1, wordLength - (i + 1));
  }
  
  return i;
}

/* Performs transpositions */
unsigned int edit_transpositions(const wchar_t *word, wchar_t **array,
                                 int start) {
  unsigned int i = 0;
  int wordLength = wcslen(word);
  
  for (; i < wordLength; i++) {
    int pos = 0;
    array[i + start] = malloc(sizeof(wchar_t) * (wordLength + 2));
    append(array[i + start], &pos, word, 0, i);
    append(array[i + start], &pos, word, i + 1, 1);
    append(array[i + start], &pos, word, i, 1);
    append(array[i + start], &pos, word, i + 2, wordLength - (i + 2));
  }
  
  return i;
}

/* Performs alterations */
unsigned int edit_alterations(wchar_t *word, wchar_t **array, int start,
                              wchar_t *alphabet) {
  int i, j, wordLength = wcslen(word), alphabetSize = wcslen(alphabet);
  unsigned int k = 0;
  wchar_t c[2] = {};
 
  for (i = 0; i < wordLength; i++) {
    for (j = 0; j < alphabetSize; j++, k++) {
      int pos = 0;
      c[0] = alphabet[j];
      array[k + start] = malloc(sizeof(wchar_t) * (wordLength + 2));
      append(array[k + start], &pos, word, 0, i);
      append(array[k + start], &pos, c, 0, 1);
      append(array[k + start], &pos, word, i + 1, wordLength - (i + 1));
    }
  }

  return k;
}

/* Performs insertions */
unsigned int edit_insertions(wchar_t *word, wchar_t **array, int start,
                             wchar_t *alphabet) {
  int i, j, wordLength = wcslen(word), alphabetSize = wcslen(alphabet);
  unsigned int k = 0;
  wchar_t c[2] = {0, 0};

  for (i = 0; i <= wordLength; i++) {
    for (j = 0; j < alphabetSize; j++, k++) {
      int pos = 0;
      c[0] = alphabet[j];
      array[k + start] = malloc(sizeof(wchar_t) * (wordLength + 2));
      append(array[k + start], &pos, word, 0, i);
      append(array[k + start], &pos, c, 0, 1);
      append(array[k + start], &pos, word, i, wordLength - i);
    }
  }
  
  return k;
}
