#ifndef WORDCOLLECTION_H_INCLUDED
#define WORDCOLLECTION_H_INCLUDED

const int SANITY_BYTES_CHECK = 6661230;
const int START_WORD_MEMORY_CAPACITY = 3000;

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

// HELPER FUNCTIONS
bool IgnoreWord(unsigned char * theword , unsigned int wordsize );
void UpcaseIt(unsigned char * text,unsigned int textsize); //Metatrepei String se Upcase
// HELPER FUNCTIONS


// CONSTRUCTORS / DESTRUCTORS
struct word_collection * NewWordCollection();
bool DeleteWordCollection(struct word_collection * acol);

// FUNCTIONS!
bool AddWord2Collection( word_collection * acol,unsigned char * word,unsigned short length);
unsigned int GetWordOccurances(word_collection * acol,unsigned char * word,unsigned short length);



#endif // WORDCOLLECTION_H_INCLUDED
