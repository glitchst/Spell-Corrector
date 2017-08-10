#ifndef __EDITS_H__
#define __EDITS_H__

#include <stddef.h>

/* Calculates the space necessary for storing all the operations */
unsigned int calculate_edits_space(wchar_t *word, wchar_t *alphabet);

/* Performs deletions */
unsigned int edit_deletions(wchar_t *word, wchar_t **array, int start);

/* Performs transpositions */
unsigned int edit_transpositions(const wchar_t *word, wchar_t **array,
                                 int start);

/* Performs alterations */
unsigned int edit_alterations(wchar_t *word, wchar_t **array, int start,
                              wchar_t *alphabet);

/* Performs insertions */
unsigned int edit_insertions(wchar_t *word, wchar_t **array, int start,
                             wchar_t *alphabet);

#endif /* __EDITS_H__ */
