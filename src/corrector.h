#ifndef __CORRECTOR_H__
#define __CORRECTOR_H__

#include <stddef.h>

typedef enum {
  CORR_SUCCESS,
  FAIL_DICTFILE,
  FAIL_INPUTFILE,
  FAIL_OUTPUTFILE,
  FAIL_ALPHABETFILE
} CorrectorStatus;

/* Main spell corrector functionality */
CorrectorStatus spell_corrector(char *dictionaryFile, char *inputFile,
                    char *outputFile, char *alphabetFile);

#endif /* __CORRECTOR_H__ */
