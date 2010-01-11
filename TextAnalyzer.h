#ifndef TEXTANALYZER_H_INCLUDED
#define TEXTANALYZER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


struct word
{
    unsigned long hash;
    unsigned int occurances;
    unsigned short length;
    unsigned char * string;
};

struct word_collection
{
    unsigned int total_words;
    unsigned int words_mem_length_cap;
    unsigned int words_mem_length;
    struct word * words;
    unsigned int sanity_byte;

};

//bool DeleteWordCollection(struct word_collection * acol);
//bool NewWordCollection(struct word_collection * acol);
//bool CollectionOk(struct world_collection * acol);

unsigned int ClearTextFromHTMLTags(char * text,unsigned int &textsize);

#endif // TEXTANALYZER_H_INCLUDED
