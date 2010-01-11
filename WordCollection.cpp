#include "WordCollection.h"
#include "HashFunctions.h"
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

  if ( wordsize > 30 ) {}
  else
  if ( wordsize >= 3 ) retres = false;

  return retres;
}

void UpcaseIt(unsigned char * text,unsigned int textsize) //Metatrepei String se Upcase
{
  for (unsigned int i=0; i<textsize; i++) text[i]=toupper(text[i]);
}


inline unsigned long inline_sdbm(unsigned char *str)
{
  unsigned long hash = 0;
  int c;
  while (c = *str++) hash = c + (hash << 6) + (hash << 16) - hash;
  return hash;
}

void SanityProblem()
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
bool NH_StringEqualToWordInCollection(word_collection * acol,unsigned int index,unsigned char * word,unsigned short length)
{
  if ( acol->words[index].length!=length ) return false;

  for ( int i=0; i<length; i++)
    {
      if (acol->words[index].string[i]!=word[i]) return false;
    }

  return true;
}


bool CollectionOk(struct word_collection * acol)
{
  if ( acol != 0 )
    {
      if ( acol->sanity_byte != 66666 )
        {
          SanityProblem();
          return false;
        }
      return true;
    }
  fprintf(stderr,"WordCollection has memory problem! \n");
  return false;
}

// STRING COMPARISON WITH A STRING STORED @ Word Collection - H means using HASH
bool H_StringEqualToWordInCollection(word_collection * acol,unsigned int index,unsigned long word_hash)
{
  if ( acol->words[index].hash!=word_hash ) return false;
  return true;
}


bool DeleteWordCollection(struct word_collection * acol)
{
  if ( acol != 0 )
    {
      if ( acol->sanity_byte != 66666 )
        {
          SanityProblem();
          return false;
        }

      for ( unsigned int i=0; i<acol->words_mem_length; i++ )
        {
          free ( acol->words[0].string ) ;
        }

      free(acol->words);
      free(acol);
      acol = 0;
    }
  return true;
}

bool NewWordCollection(struct word_collection * acol)
{
  if ( acol != 0 )
    {
      if ( !DeleteWordCollection(acol) )
        {
          return false;
        }
    }

  struct word_collection tmp;
  acol = ( struct word_collection * ) malloc ( sizeof( tmp ) );
  acol->sanity_byte = 66666;

  acol->words_mem_length_cap = 3000;
  struct word tmpw;
  acol->words = ( struct word  * ) malloc ( sizeof( tmpw ) );


  acol->words_mem_length = 0;
  acol->total_words = 0;
  acol->words = 0;
  return true;
}


unsigned int H_FindWordAtCollection(word_collection * acol, unsigned long hash_data , unsigned char * word,unsigned short length)
{
  if ( !CollectionOk(acol) ) return 0;

  fprintf(stderr,"Searching for `%s` between %u words! \n",word,acol->words_mem_length);
// SERIAL SEARCH ! , TO BE IMPROVED :P
  for ( unsigned int i = 0; i < acol->words_mem_length; i++ )
    {
      if (  H_StringEqualToWordInCollection(acol,i,hash_data) )  return i;
    }

  return 0;
}

bool Phys_AddWord2Collection(word_collection * acol,unsigned long hash_data ,unsigned char * word,unsigned short length)
{
  unsigned int current_memspot = acol->words_mem_length;

  if (  acol->words_mem_length_cap <= current_memspot )
    {
      fprintf(stderr, "Reached Word Collection Peak .. !! :P , I should reallocate memory to accomodate more data but first things first :P \n");
      return false;
    }

  acol->words[current_memspot].hash = hash_data;
  acol->words[current_memspot].occurances = 1;
  acol->words[current_memspot].length = length;
  acol->words[current_memspot].string =(unsigned char *) malloc ( sizeof( unsigned char ) * ( length + 1 ) ); // +1 = final \0

  for ( unsigned int i=0; i<length; i++ )
    {
      acol->words[current_memspot].string[i] = word[i];
    }

  acol->words_mem_length+=1;
  acol->total_words+=1;
  return true;
}

bool AddWord2Collection( word_collection * acol,unsigned char * word,unsigned short length)
{
  if ( !CollectionOk(acol) )
    {
      return false;
    }

  // PREPARE WORD UPCASE DUPLICATE  // DONT FORGET TO FREE IT AFTERWARDS
  unsigned char * act_word; // <- The actual word , upcased :P
  act_word = (unsigned char * ) malloc (sizeof( unsigned char ) * (length + 1 ) );
  for ( unsigned short i=0; i<length; i++ )
    {
      act_word[i]=word[i];
    }
  UpcaseIt(act_word,length);
  // PREPARE WORD UPCASE DUPLICATE

  unsigned long  hash_data = inline_sdbm (act_word);

  unsigned char found=0;

  // HASH , FAST SEARCH!
  unsigned int memspot = H_FindWordAtCollection(acol,hash_data,act_word,length);
  if ( memspot == 0 )
    {
      // 2 possible states, we didn`t find a word ( we should add it as a new then )
      // the word is positioned at 0!
      if ( H_StringEqualToWordInCollection(acol,0,hash_data) )
        {
          found = 1;
        }
    }
  else
    {
      // We found (?) the word at position memspot!
      found = 1;
    }
  // HASH , FAST SEARCH!

  if ( found == 1 )
    {
      // We have found a matching hash..!
      // We need to check if it is a real hit or a hash overlap!
      if  ( NH_StringEqualToWordInCollection(acol,memspot,act_word,length) )
        {
          acol->words[memspot].occurances+=1;
          acol->total_words+=1;
          free ( act_word );
          return true;
        }
      else
        {
          // HASH FUNCTION OVERLAPPED :P
          found = 0;
          fprintf(stderr,"Hash Function Overlapped :P \n");

          //ToDO add code that checks words without using hashes!
          fprintf(stderr,"ToDO add code that checks words without using hashes!\n");
        }

    }

  if ( found == 0 )
    {
      // The word was not found , add the word as a new word! :)
      free ( act_word );
      return Phys_AddWord2Collection(acol,hash_data,act_word,length);
    }


  free ( act_word );
  return true;
}

bool DeleteWordFromCollection( word_collection * acol,unsigned char * word,unsigned short length)
{
  // TODO
  fprintf(stderr,"Function has no particular usage in the first project that uses TextAnalyzer so it will be implemented later :P , First Things first \n");
  return true;
}

unsigned int GetWordOccurances(word_collection * acol,unsigned char * word,unsigned short length)
{
  // TODO
  if ( IgnoreWord(word,length) )
    {
      return 0;
    }

  // PREPARE WORD UPCASE DUPLICATE  // DONT FORGET TO FREE IT AFTERWARDS
  unsigned char * act_word; // <- The actual word , upcased :P
  act_word = (unsigned char * ) malloc (sizeof( unsigned char ) * (length + 1 ) );
  for ( unsigned short i=0; i<length; i++ )
    {
      act_word[i]=word[i];
    }
  UpcaseIt(act_word,length);
  // PREPARE WORD UPCASE DUPLICATE

  unsigned long hash_data = inline_sdbm(word);
  fprintf(stderr,"TODO : Add search! \n");

  unsigned int memspot = H_FindWordAtCollection(acol,hash_data,act_word,length);


  free (act_word);

  return memspot; // <- Moufa!
  return 0;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// WORD COLLECTION KEEPING
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
