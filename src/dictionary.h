#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <stddef.h>
#include <wchar.h>

#define ALPHABET_SIZE   26
#define MAX_WORD_SIZE   45
#define HASH_SIZE       20000

#define true  1
#define false 0

typedef struct _node {
  wchar_t word[MAX_WORD_SIZE + 1];
  struct _node *next;
} HashNode;

/* Takes a dictionary file location and tries to load it into memory */
int load_dictionary(const char *location);

/* Checks whether a word is in the dictionary */
int is_in_dictionary(const wchar_t *word);

/* Fills an array of strings with suggestions for an incorrect word,
 * based on the edits performed by edits.h */
wchar_t **find_suggestions(wchar_t *word, wchar_t *alphabet,
                           int *resultsAmount);
                           
/* Unloads the dictionary from memory */
int dictionary_unload(void);

#endif /* __DICTIONARY_H__ */
