#ifndef TEXTANALYZER_H_INCLUDED
#define TEXTANALYZER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "WordCollection.h"


#ifdef __cplusplus
extern "C" {
#endif


word_collection * TextAnalyzer_NewWordCollection();
unsigned int TextAnalyzer_WordOccurances(word_collection * acol,unsigned char * theword , unsigned int wordsize);
void TextAnalyzer_DeleteWordCollection(word_collection * acol);

unsigned int ClearTextFromHTMLTags(word_collection * acol,char * text,unsigned int &textsize);

#ifdef __cplusplus
}
#endif

#endif // TEXTANALYZER_H_INCLUDED
