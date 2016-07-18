#ifndef TEXTANALYZER_H_INCLUDED
#define TEXTANALYZER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "WordCollection.h"


#ifdef __cplusplus
extern "C" {
#endif


struct word_collection * TextAnalyzer_NewWordCollection();
unsigned int TextAnalyzer_WordOccurances(struct word_collection * acol,unsigned char * theword , unsigned int wordsize);
void TextAnalyzer_DeleteWordCollection(struct word_collection * acol);

unsigned int ClearTextFromHTMLTags(struct word_collection * acol,char * text,unsigned int *textsize);

#ifdef __cplusplus
}
#endif

#endif // TEXTANALYZER_H_INCLUDED
