#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "corrector.h"

int main(int argc, char *argv[]) {
  if (argc > 5 || argc < 3) {
    wprintf(L"ARGUMENT ERROR: The correct use of the program is: %s ",
            argv[0]);
    wprintf(L"inputFile outputFile dictionary [alphabet]\n");
    wprintf(L"NOTE: Alphabet file is not required, since there's a ");
    wprintf(L"default one, but if you're working with other languages");
    wprintf(L" it's recommended you provide your own.\n\n");
    return 1;
  }
  
  CorrectorStatus status;
  char *dictionaryLoc, *inputFileLoc, *outputFileLoc, *alphabetFileLoc;
  
  /* Obligatory arguments */
  inputFileLoc      = argv[1];
  outputFileLoc     = argv[2];
  dictionaryLoc     = argv[3];
  
  /* Check for the alphabet file */
  if (argc == 5)
    alphabetFileLoc = argv[4];
  else
    alphabetFileLoc = "defaultDictionary.txt";
  
  status = spell_corrector(dictionaryLoc, inputFileLoc,
                                    outputFileLoc, alphabetFileLoc);
                                    
  switch (status) {
    case CORR_SUCCESS:
      wprintf(L"Program was executed correctly.\n");
      wprintf(L"Incorrect words with their suggestions can be found");
      wprintf(L" in [%s]\n\n", outputFileLoc);
      break;
    
    case FAIL_DICTFILE:
      wprintf(L"ERROR: Dictionary could not be loaded/found.\n");
      exit(FAIL_DICTFILE);
      
    case FAIL_INPUTFILE:
      wprintf(L"ERROR: Input file could not be loaded/found.\n");
      exit(FAIL_INPUTFILE);
      
    case FAIL_OUTPUTFILE:
      wprintf(L"ERROR: Output file could not be loaded/found.\n");
      exit(FAIL_OUTPUTFILE);
      
    case FAIL_ALPHABETFILE:
      wprintf(L"ERROR: Alphabet file could not be loaded/found.\n");
      exit(FAIL_ALPHABETFILE);
  }

  return 0;
}
