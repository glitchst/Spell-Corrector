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
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

/* Substring function */
static wchar_t *substr(const wchar_t *str, size_t offset, size_t limit) {
  wchar_t *newStr;
  size_t strSize = wcslen(str);
  
  if ((limit > strSize) || ((offset + limit) > strSize) || (strSize < 1) ||
      (limit == 0))
    return NULL;
    
  newStr = malloc((limit + 1) * sizeof(wchar_t));
  
  if (!newStr)
    return NULL;
    
  wcsncpy(newStr, str + offset, limit);
  *(newStr + limit) = L'\0';
  
  return newStr;
}

/* Another take on wcscat/strcat */
static wchar_t *concat(wchar_t *str1, wchar_t *str2) {
  if (!str1) {
    str1 = malloc(sizeof(wchar_t));
    *str1 = L'\0';
  }
  
  if (!str2) {
    str2 = malloc(sizeof(wchar_t));
    *str2 = L'\0';
  }
  
  wchar_t *newStr = malloc(wcslen(str1) + wcslen(str2) + 1);
  newStr = wcscat(str1, str2);

  return newStr;
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
  unsigned int i;
  int wordLength = wcslen(word);
  
  for (i = 0; i < wordLength; i++) {
    array[i + start] = concat(substr(word, 0, i),
                              substr(word, i + 1, wordLength - (i + 1)));
  }
  
  return i;
}

/* Performs transpositions */
unsigned int edit_transpositions(wchar_t *word, wchar_t **array, int start) {
  unsigned int i;
  int wordLength = wcslen(word);
  
  for (i = 0; i < wordLength - 1; i++) {
    array[i + start] = (
      concat(concat(substr(word, 0, i), substr(word, i + 1, 1)),
             concat(substr(word, i, 1), substr(word, i + 2, 
                                               wordLength - (i + 2)))));
  }
  
  return i;
}

/* Performs alterations */
unsigned int edit_alterations(wchar_t *word, wchar_t **array, int start,
                              wchar_t *alphabet) {
  int i, j, wordLength = wcslen(word), alphabetSize = wcslen(alphabet);
  unsigned int k;
  wchar_t c[2] = {0, 0};
 
  for (i = 0, k = 0; i < wordLength; i++) {
    for (j = 0; j < alphabetSize; j++, k++) {
      c[0] = alphabet[j];
      array[start + k] = concat(concat(substr(word, 0, i), (wchar_t *)&c),
                                substr(word, i + 1, wordLength - (i + 1)));
    }
  }
  
  return k;
}

/* Performs insertions */
unsigned int edit_insertions(wchar_t *word, wchar_t **array, int start,
                             wchar_t *alphabet) {
  int i, j, wordLength = wcslen(word), alphabetSize = wcslen(alphabet);
  unsigned int k;
  wchar_t c[2] = {0, 0};

  for (i = 0, k = 0; i <= wordLength; i++) {
    for (j = 0; j < alphabetSize; j++, k++) {
      c[0] = alphabet[j];
      array[start + k] = concat(concat(substr(word, 0, i), (wchar_t *)&c),
                                substr(word, i, wordLength - i));
    }
  }
  
  return k;
}
