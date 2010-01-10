#ifndef TEXTANALYZER_H_INCLUDED
#define TEXTANALYZER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


struct word
{
    unsigned int hash;
    unsigned int occurances;
    unsigned int length;
    unsigned char * string;
};

struct word_collection
{
    unsigned int words_number;
    unsigned int total_words;
    struct word * words;
    unsigned int sanity_byte;

};

//bool DeleteWordCollection(struct word_collection * acol);
//bool NewWordCollection(struct word_collection * acol);
//bool CollectionOk(struct world_collection * acol);

unsigned int ClearTextFromHTMLTags(char * text,unsigned int &textsize);

#endif // TEXTANALYZER_H_INCLUDED
