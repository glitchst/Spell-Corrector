/**
 * corrector.c
 * 
 * This library contains the main processing of the required files for
 * the spelling corrector.
 */
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#include "dictionary.h"
#include "corrector.h"

int spell_corrector(char *dictionaryFileLoc, char *inputFileLoc,
                    char *outputFileLoc, char *alphabetFileLoc) {
  int i, sugAmount, isDictLoaded, isDictUnloaded, count = 0, index = 0, 
      misspellings = 0, totalWords = 0;

  wchar_t *alphabet = NULL;
  wchar_t buffer[MAX_WORD_SIZE + 1];
  
  /* Load the dictionary into memory */
  isDictLoaded = load_dictionary(dictionaryFileLoc);
  
  /* Return an error code if there was a problem */
  if (!isDictLoaded)
    return -1;
    
  /* Open the input file, containing the words to correct. */
  FILE *inputFile = fopen(inputFileLoc, "r");
  
  /* Return an error code if there was a problem loading the file */
  if (!inputFile)
    return -2;
  
  /* We open the output file in w+ mode so we can write into it */
  FILE *outputFile = fopen(outputFileLoc, "w+");
  
  /* Return an error code if there was a problem loading the file */
  if (!outputFile)
    return -3;
    
  /* Write a simple header for the file */
  fwprintf(outputFile, L"Misspelled words in %s:\n============\n",
           inputFileLoc);
  
  /* Open the alphabet file, default is handled by main.c */
  FILE *alphabetFile = fopen(alphabetFileLoc, "r");
  
  if (alphabetFile) {
    /* If the alphabet file was loaded successfully, then we can
     * build the alphabet string we're going to use for the word 
     * operations */
    for (wint_t c = fgetwc(alphabetFile); c != WEOF;
         c = fgetwc(alphabetFile)) {
      alphabet = realloc(alphabet, (count + 1) * sizeof(wchar_t));
      alphabet[count++] = c;
    }
  } else {
    return -4;
  }
  
  /* Start the corrector process */
  for (wint_t c = fgetwc(inputFile); c != WEOF; c = fgetwc(inputFile)) {
    /* Only allow alphabetical characters */
    if (iswalpha(c)) {
      buffer[index++] = c;
      
      /* Ignore character chains that are too long to be words */
      if (index > MAX_WORD_SIZE) {
        /* Iterate through rest of the characters */
        while ((c = fgetwc(inputFile)) != WEOF && iswalpha(c));
        
        /* Prepare for a new word */
        index = 0;
      }
    }
    /* Ignore words or chains that have digits in them */
    else if (iswdigit(c)) {
        /* Iterate through rest of the characters */
        while ((c = fgetwc(inputFile)) != WEOF && iswalpha(c));
        
        /* Prepare for a new word */
        index = 0;
    }
    /* If we're looking at a non-alphabetical character and our index
     * is greater than 0, then we can assume we're at the end of a word.
     * Note that this character can be a space, a new line, whatever. */
    else if (index > 0) {
      /* Null-terminate the word in the buffer */
      buffer[index] = L'\0';
      
      /* Add one to the word counter */
      totalWords++;
      
      /* If the word is not in the dictionary, it's misspelled (or not,
       * depending on the dictionary. I've had instances where a 
       * dictionary didn't include stuff like plural nouns, or some
       * gendered nouns, etc). */
      if (!is_in_dictionary(buffer)) {
        /* We use the function to find possible suggestions for the word
         * in question */
        wchar_t **suggestions = find_suggestions(buffer, alphabet,
                                                 &sugAmount);
        
        /* Add one to the misspellings counter */
        misspellings++;
        
        /* If there is suggestions, we add them to the file */
        if (suggestions) {
          /* Print the incorrect word to the file and add a new line for
           * some pretty nice formatting ;) */
          fwprintf(outputFile, L"%ls >>\n", buffer);
          
          /* We print the suggestions separated by tabs, also for some
           * pretty nice formatting and legibility. */
          for (i = 0; i < sugAmount; i++)
            fwprintf(outputFile, L"\t%ls", suggestions[i]);

          /* Add a separator for the next word */
          fwprintf(outputFile, L"\n\n");
          
          /* @TODO: free free memory used by suggestions */
          
        } else {
          /* If there's no suggestions, we warn the user */
          fwprintf(outputFile,
                   L"%ls >> no suggestions were found.\n\n",
                   buffer);
        }
      }

      /* Prepare for a new word */
      index = 0;
    }
  }

  /* Alphabet string is no longer in use, so we free it */
  free(alphabet);
  
  /* Since we're done checking the file, we can now unload the
   * dictionary */
  isDictUnloaded = dictionary_unload();

  /* Give the user a little insight about the words that were scanned
   * and the total incorrectly spelled words. */
  wprintf(L"Words scanned:     %d\n", totalWords);
  wprintf(L"Words misspelled:  %d\n\n", misspellings);

  /* Close the files that we opened. NOTE: the dictionary file gets
   * closed in the build_dictionary function after it gets loaded
   * into memory */
  fclose(alphabetFile);
  fclose(inputFile);
  fclose(outputFile);
  
  /* We return 0 as a success code */
  return 0;
}
