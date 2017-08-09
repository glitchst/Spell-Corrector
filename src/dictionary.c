/**
 * dictionary.c
 * 
 * This library provides functions to build a dictionary in memory using
 * a hash table, check if a word is in the dictionary, build an array of
 * possible correct words based off an incorrect one, and finally unload
 * the dictionary from memory.
 */
#include <ctype.h>
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "edits.h"
#include "dictionary.h"

HashNode *hashtable[HASH_SIZE];

/* The hash function used to place words into the hash table */
int hash_function(const wchar_t *word) {
  int i, sum = 0;
  int wordLength = wcslen(word);
  
  for (i = 0; i < wordLength; i++) {
    if (word[i] >= 0)
      sum += word[i];
    else
      /* Since we're working with wchar_t instead of normal chars, I
       * think there's some that have negative values, to avoid any
       * segfaults, I just multiplied by -1 */
      sum += ((-1) * word[i]);
  }
  
  return sum % HASH_SIZE; /* Bucket */
}

/* Takes a dictionary file location and tries to load it into memory */
int load_dictionary(const char *location) {
  setlocale(LC_ALL, "");
  
  int i, wordsLoaded = 0;
  
  /* Preset all the buckets to NULL */
  for (i = 0; i < HASH_SIZE; i++) {
    hashtable[i] = NULL;
  }

  /* Open file in read-only mode */
  FILE *dictionary = fopen(location, "r");

  /* Check if the file was opened correctly */
  if (!dictionary) {
    wprintf(L"File \"%s\" wasn't found or couldn't be loaded.", 
            location);
    
    return false;
  }
  
  /* Create a buffer to store the words */
  wchar_t buffer[MAX_WORD_SIZE + 1];

  /* Iterate through the words in the dictionary and add them to the
   * hash table one by one. NOTE: it's probably best if the dictionary
   * has a comprehensible format, like all words separated by spaces or
   * by new lines. */
  while (fscanf(dictionary, "%ls", buffer) > 0) {
    HashNode *newNode = malloc(sizeof(HashNode));
    newNode->next = NULL;

    wcscpy(newNode->word, buffer);

    int bucket = hash_function(newNode->word);
    
    if (!hashtable[bucket]) {
      hashtable[bucket] = newNode;
    } else {
      newNode->next = hashtable[bucket];
      hashtable[bucket] = newNode;
    }
    
    wordsLoaded++;
  }

  wprintf(L"The dictionary was loaded succesfully with %d words.\n\n",
         wordsLoaded);

  fclose(dictionary);

  return true;
}

/* Checks whether a word is in the dictionary */
int is_in_dictionary(const wchar_t *word) {
  int i, wordLength = wcslen(word);
  wchar_t lowercaseWord[wordLength + 1];
  
  /* We need a lowercase word to check against the dictionary. Usually,
   * if the person who built the dictionary was a good samaritan all 
   * words should be lowercase */
  for (i = 0; i < wordLength; i++) {
    if (!iswlower(word[i]))
      lowercaseWord[i] = towlower(word[i]);
    else
      lowercaseWord[i] = word[i];
  }

  /* Null-terminate the string */
  lowercaseWord[wordLength] = '\0';

  int bucket = hash_function(lowercaseWord);
  HashNode *cursor = hashtable[bucket];
  
  /* Iterate through the linked list to check if the word is there */
  while (cursor) {
    if (wcscmp(lowercaseWord, cursor->word) == 0)
      return true;
      
    cursor = cursor->next;
  }

  return false;
}

/* Checks for duplicates in the array of strings */
static int array_is_duplicate(wchar_t **array, int items,
                              wchar_t *word) {
  int i;
  
  for (i = 0; i < items; i++) {
    if (!wcscmp(array[i], word))
      return true;
  }
  
  return false;
}

/* Fills an array of strings with suggestions for an incorrect word,
 * based on the edits performed by edits.h */
wchar_t **find_suggestions(wchar_t *word, wchar_t *alphabet,
                           int *resultsAmount) {
  int i;
  unsigned int next, editsSpace, suggestionsSpace, resultSize = 0,
               wordLength = wcslen(word);
  int resMax = 50;
  wchar_t **suggestions = NULL;
  wchar_t lowercaseWord[wordLength + 1];
  
  /* Convert word to lowercase */
  for (i = 0; i < wordLength; i++) {
    if (!iswlower(word[i]))
      lowercaseWord[i] = towlower(word[i]);
    else
      lowercaseWord[i] = word[i];
  }

  /* Null-terminate the string */
  lowercaseWord[wordLength] = L'\0';
  
  /* Calculate the space necesary to store the edits of the word */
  editsSpace = calculate_edits_space(lowercaseWord, alphabet);
  wchar_t *edits[editsSpace];
  
  /* Use the edits.h functions to operate over the word */
  next  = edit_deletions(lowercaseWord, edits, 0);
  next += edit_transpositions(lowercaseWord, edits, next);
  next += edit_alterations(lowercaseWord, edits, next, alphabet);
  next += edit_insertions(lowercaseWord, edits, next, alphabet);
  
  /* Iterate through the edits array and if we find an edit in the
   * dictionary, add it to the suggestions */
  for (i = 0; i < editsSpace; i++) {
    if (is_in_dictionary(edits[i])) {
      if (!array_is_duplicate(suggestions, resultSize, edits[i])) {
        if (resultSize >= resMax) {
          resMax *= 2;
          suggestions = realloc(suggestions, sizeof(wchar_t *) * resMax);
        }
        
        suggestions[resultSize++] = edits[i];
      }
    }
  }

  *resultsAmount = resultSize;
  
  return suggestions;
}

/* Unloads the dictionary from memory */
int dictionary_unload(void) {
  for (int i = 0; i < HASH_SIZE; i++) {
    HashNode *cursor = hashtable[i];
    
    while (cursor) {
      HashNode *temp = cursor;
      cursor = cursor->next;
      free(temp);
    }
  }
  
  return true;
}
