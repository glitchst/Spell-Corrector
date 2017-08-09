#ifndef __CORRECTOR_H__
#define __CORRECTOR_H__

#include <stddef.h>

/* Main spell corrector functionality */
int spell_corrector(char *dictionaryFile, char *inputFile,
                    char *outputFile, char *alphabetFile);

#endif /* __CORRECTOR_H__ */
