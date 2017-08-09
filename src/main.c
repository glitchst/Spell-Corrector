#include <stdio.h>
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
  
  int correctorStatus;
  char *dictionaryLoc, *inputFileLoc, *outputFileLoc, *alphabetFileLoc;
  
  /* Obligatory arguments */
  inputFileLoc      = argv[1];
  outputFileLoc     = argv[2];
  dictionaryLoc     = argv[3];
  
  /* Checkeamos si hay archivo de alfabeto */
  alphabetFileLoc   = (argv[4]) ? argv[4] : "defaultAlphabet.txt";
  
  correctorStatus = spell_corrector(dictionaryLoc, inputFileLoc,
                                    outputFileLoc, alphabetFileLoc);
                                    
  switch (correctorStatus) {
    case 0:
      wprintf(L"Program was executed correctly.\n");
      wprintf(L"Incorrect words with their suggestions can be found");
      wprintf(L" in [%s]\n\n", outputFileLoc);
      break;
    
    case -1:
      wprintf(L"ERROR: Dictionary could not be loaded/found.\n");
      return -1;
      
    case -2:
      wprintf(L"ERROR: Input file could not be loaded/found.\n");
      return -2;
      
    case -3:
      wprintf(L"ERROR: Output file could not be loaded/found.\n");
      return -3;
      
    case -4:
      wprintf(L"ERROR: Alphabet file could not be loaded/found.\n");
      return -3;
  }

  return 0;
}
