#include "WordCollection.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// *******************************************************************************
// *******************************************************************************
// *******************************************************************************
// HELPER FUNCTIONS
// *******************************************************************************
bool IgnoreWord(unsigned char * theword , unsigned int wordsize )
{
  bool retres = true;
  if ( wordsize > 30 ) {} else
  if ( wordsize >= 3 ) retres = false;
  return retres;
}

void UpcaseIt(unsigned char * text,unsigned int textsize) //Metatrepei String se Upcase
{
  for (unsigned int i=0; i<textsize; i++) text[i]=toupper(text[i]);
}

void CutSpaces(unsigned char * text,unsigned short &textsize) //Kovei ta kena deksia k aristera apo to String
{
  for (unsigned short i=textsize-1; i>0; i--)
  {
     if ( (text[i]!='_')&&(text[i]!=' ') ) break; else
                        { text[i]=0; --textsize; }
  }

  unsigned short start_cut = 0;
  for (unsigned short i=0; i<textsize; i++)
  {
     if ( (text[i]!='_')&&(text[i]!=' ') ) break; else
                        { ++start_cut; }

  }
  if ( start_cut > 0 )
  {
   for (unsigned short i=start_cut; i<textsize; i++)
   {
     text[i-start_cut]=text[i];
   }
   textsize-=start_cut;
  }

}

inline unsigned long inline_sdbm(unsigned char *str)
{
  unsigned long hash = 0;
  int c;
  while (c = *str++) hash = c + (hash << 6) + (hash << 16) - hash;
  return hash;
}

inline void SanityProblem()
{
  fprintf(stderr,"Sanity byte problem -> memory overlapping -> buggy code :P :S \n ");
}
// *******************************************************************************
// HELPER FUNCTIONS
// *******************************************************************************
// *******************************************************************************
// *******************************************************************************



//-----------------------------------------------------------------------------------------------------------------------------------



/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   WORD COLLECTION KEEPING
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/

// STRING COMPARISON WITH A STRING STORED @ Word Collection - NH means NOT using HASH
inline bool NH_StringEqualToWordInCollection(word_collection * acol,unsigned int index,unsigned char * word,unsigned short length)
{
  if ( (acol->words[index].length==0) || ( acol->words[index].length!=length ) ) return false;

  for ( int i=0; i<length; i++)
    {
      if (acol->words[index].string[i]!=word[i]) return false;
    }
  return true;
}


// STRING COMPARISON WITH A STRING STORED @ Word Collection - H means using HASH
inline bool H_StringEqualToWordInCollection(word_collection * acol,unsigned int index,unsigned long word_hash)
{
  if ( acol->words[index].hash!=word_hash ) return false;
  return true;
}


inline bool CollectionOk(struct word_collection * acol)
{
  if ( acol != 0 )
    {
      if ( acol->sanity_byte != SANITY_BYTES_CHECK )
        {
          SanityProblem();
          return false;
        }
      return true;
    }
  fprintf(stderr,"WordCollection has memory problem! \n");
  return false;
}


bool DeleteWordCollection(struct word_collection * acol)
{
  //fprintf(stderr,"Deleting Word Collection!\n");
  if ( acol != 0 )
    {
      if ( acol->sanity_byte != SANITY_BYTES_CHECK )
        {
          SanityProblem();
          return false;
        }

      //fprintf(stderr,"Freeing all Words !\n");
      for ( unsigned int i=0; i<acol->words_mem_length; i++ )
        {
          if ( (acol->words[i].string!=0) ) free( acol->words[i].string );
          acol->words[i].length = 0;
        }


      //fprintf(stderr,"Freeing Words container !\n");
      if ( acol->words != 0 ) free(acol->words);

      //fprintf(stderr,"Freeing Words collection !\n");
      free(acol);
    }

  return true;
}

struct word_collection * NewWordCollection()
{
  struct word_collection * acol = 0;

  struct word_collection tmp;
  acol = ( struct word_collection * ) malloc ( sizeof( tmp ) );
  acol->sanity_byte = SANITY_BYTES_CHECK;
  acol->words_mem_length_cap = START_WORD_MEMORY_CAPACITY+1;
  acol->words_mem_length = 1;
  acol->total_words = 0;
  acol->memory_usage = 0;

  struct word tmpw;
  acol->words = ( struct word  * ) malloc ( sizeof( tmpw )* ( acol->words_mem_length_cap )  );
  acol->memory_usage += sizeof( tmpw )* ( acol->words_mem_length_cap );

  for ( int i=0; i < acol->words_mem_length_cap; i++)
  {
    acol->words[i].hash=0;
    acol->words[i].occurances=0;
    acol->words[i].length=0;
    acol->words[i].string=0;
  }

  if (! CollectionOk(acol) ) fprintf(stderr,"Error allocating memory.. ( Press c to continue ) \n");

  return acol;
}


unsigned int FindWordAtCollection(word_collection * acol, unsigned long hash_data , unsigned char * word,unsigned short length)
{
  if ( !CollectionOk(acol) ) return 0;

  fprintf(stderr,"Searching for `%s` between %u words! ... ",word,acol->words_mem_length);
  // SERIAL SEARCH ! , TO BE IMPROVED :P
  for ( unsigned int i = 0; i <= acol->words_mem_length; i++ )
    {
      if (  H_StringEqualToWordInCollection(acol,i,hash_data) )
      {
        if (  NH_StringEqualToWordInCollection(acol,i,word,length) )
        {
         printf(" found at %u , %u times \n",i,acol->words[i].occurances);
         return i;
        } else
        {
          printf(" bumped on hash duplicate (%s,%u with %s,%u) while searching! \n",acol->words[i].string,acol->words[i].length,word,length);
        }
      }
    }
  printf(" not found!\n");
  return 0;
}

bool AddNewWord2Collection(word_collection * acol,unsigned long hash_data ,unsigned char * word,unsigned short length)
{
  // WORD ASSUMED TO BE AN <<UPCASE>> WORD!
  unsigned int current_memspot = acol->words_mem_length;

  if (  acol->words_mem_length_cap <= current_memspot )
    {
      fprintf(stderr, "Reached Word Collection Peak .. !! :P , I should reallocate memory to accomodate more data but first things first :P \n");
      return false;
    }

  acol->words[current_memspot].string =(unsigned char *) malloc ( sizeof( unsigned char ) * ( length + 1 ) ); // +1 = final \0
  acol->memory_usage += sizeof( unsigned char ) * ( length + 1 );


  if ( acol->words[current_memspot].string == 0 ) { fprintf(stderr,"Could not allocate memory for string .. Adding failed! \n"); return false;}
  acol->words[current_memspot].hash = hash_data;
  acol->words[current_memspot].occurances = 1;
  acol->words[current_memspot].length = length;

  for ( unsigned int i=0; i<length; i++ )
    {
      acol->words[current_memspot].string[i] = word[i];
    }
  acol->words[current_memspot].string[length]=0;

  acol->words_mem_length+=1;
  acol->total_words+=1;
  return true;
}

bool AddWord2Collection( word_collection * acol,unsigned char * word,unsigned short length)
{
  if ( !CollectionOk(acol) ) return false;
  if ( length == 0 ) { return false; } // Safe guard

  // Check if word contains an ending \0 , we dont need it , remove it :P
  if (word[length-1]==0) {length -= 1;}


  // PREPARE WORD UPCASE DUPLICATE  // DONT FORGET TO FREE IT AFTERWARDS
  unsigned char * act_word; // <- The actual word , upcased :P
  act_word = (unsigned char * ) malloc (sizeof( unsigned char ) * (length + 1 ) );
  for ( unsigned short i=0; i<length; i++ ) act_word[i]=word[i];
  UpcaseIt(act_word,length);
  act_word[length]=0;
  // PREPARE WORD UPCASE DUPLICATE

  unsigned long  hash_data = inline_sdbm (act_word); // Calculate word hash!

  // HASH , FAST SEARCH!
  unsigned int memspot = FindWordAtCollection(acol,hash_data,act_word,length);
  unsigned char found=0;
  bool retres=true;
  if ( memspot == 0 )
    {
      // 2 possible states #1 we didn`t find a word ( we should add it as a new then )
      // #2 the word is actually positioned at 0!
      if ( NH_StringEqualToWordInCollection(acol,0,act_word,length) )
        {
          found = 1;
        }
    }
  else
    { // We found the word at position memspot!
      found = 1;
    }
  // HASH , FAST SEARCH!

  if ( found == 1 )
    {
      // We have found a matching hash..!
      acol->words[memspot].occurances+=1;
      acol->total_words+=1;
    }
     else
  if ( found == 0 )
    {
      // The word was not found , add the word as a new word! :)
      retres = AddNewWord2Collection(acol,hash_data,act_word,length);
    }


  free ( act_word );
  return retres;
}

bool DeleteWordFromCollection( word_collection * acol,unsigned char * word,unsigned short length)
{
  // TODO
  fprintf(stderr,"Function has no particular usage in the first project that uses TextAnalyzer so it will be implemented later :P , First Things first \n");
  fprintf(stderr,"in the future the whole structure will be a B-Tree so no need to write it now..\n");
  return true;
}

unsigned int GetWordOccurances(word_collection * acol,unsigned char * word,unsigned short length)
{
  if ( IgnoreWord(word,length) )
    {
      printf("Ignoring search for occurances ..\n");
      return 0;
    }

  // PREPARE WORD UPCASE DUPLICATE  // DONT FORGET TO FREE IT AFTERWARDS
  unsigned char * act_word; // <- The actual word , upcased :P
  act_word = (unsigned char * ) malloc (sizeof( unsigned char ) * (length + 1 ) );
  for ( unsigned short i=0; i<length; i++ )   act_word[i]=word[i];
  UpcaseIt(act_word,length);
  CutSpaces(act_word,length);
  act_word[length]=0;
  // PREPARE WORD UPCASE DUPLICATE

  unsigned long hash_data = inline_sdbm(act_word);

  unsigned int memspot = FindWordAtCollection(acol,hash_data,act_word,length);

  if ( memspot != 0 )
   {
     printf("Search found results @ word %u ",memspot);
     memspot = acol->words[memspot].occurances;
     printf("%u occurances\n ",memspot);
   }

  free (act_word);
  return memspot;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// WORD COLLECTION KEEPING
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
