#include "HashFunctions.h"
#include "TextAnalyzer.h"


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// WORD COLLECTION KEEPING
inline void SanityProblem()
{
     fprintf(stderr,"Sanity byte problem -> memory overlapping -> buggy code :P :S \n ");
}

// STRING COMPARISON WITH A STRING STORED @ Word Collection - NH means NOT using HASH
inline bool NH_StringEqualToWordInCollection(word_collection * acol,unsigned int index,unsigned char * word,unsigned short length)
{
   if ( acol->words[index].length!=length ) { return false; }
   for ( int i=0; i<length; i++)
    {
      if (acol->words[index].string[i]!=word[i]) { return false; }
    }

 return true;
}

// STRING COMPARISON WITH A STRING STORED @ Word Collection - H means using HASH
inline bool H_StringEqualToWordInCollection(word_collection * acol,unsigned int index,unsigned long word_hash)
{
   if ( acol->words[index].hash!=word_hash ) { return false; }
   return true;
}


bool CollectionOk(struct word_collection * acol)
{
   if ( acol != 0 ) {  if ( acol->sanity_byte != 66666 ) { SanityProblem(); return false;}
                       return true;
                    }
    return false;
}
bool DeleteWordCollection(struct word_collection * acol)
{
   if ( acol != 0 ) {  if ( acol->sanity_byte != 66666 ) { SanityProblem(); return false;}

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
   if ( acol != 0 ) {  if ( !DeleteWordCollection(acol) ) { return false; }
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
 if ( !CollectionOk(acol) ) { return 0; }

 // SERIAL SEARCH ! , TO BE IMPROVED :P
 for ( unsigned int i = 0; i < acol->words_mem_length; i++ )
 {
   if (  H_StringEqualToWordInCollection(acol,i,hash_data) ) { return i; }
 }

 return 0;
}

inline bool Phys_AddWord2Collection(word_collection * acol,unsigned long hash_data ,unsigned char * word,unsigned short length)
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
   if ( !CollectionOk(acol) ) { return false; }
   unsigned long  hash_data = sdbm (word);

   unsigned char found=0;

   // HASH , FAST SEARCH!
   unsigned int memspot = H_FindWordAtCollection(acol,hash_data,word,length);
   if ( memspot == 0 )
   {
     // 2 possible states, we didn`t find a word ( we should add it as a new then )
     // the word is positioned at 0!
     if ( H_StringEqualToWordInCollection(acol,0,hash_data) )
      {
         found = 1;
      }
   } else
   {
     // We found (?) the word at position memspot!
     found = 1;
   }
   // HASH , FAST SEARCH!

   if ( found == 1 )
   {
     // We have found a matching hash..!
     // We need to check if it is a real hit or a hash overlap!
     if  ( NH_StringEqualToWordInCollection(acol,memspot,word,length) )
     {
       acol->words[memspot].occurances+=1;
       acol->total_words+=1;
       return true;
     } else
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
     return Phys_AddWord2Collection(acol,hash_data,word,length);
   }


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
   fprintf(stderr,"Add search! \n");
   return 0;
}
// WORD COLLECTION KEEPING
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


unsigned int FindFirstInstanceOfChar(unsigned int ptr,char * text,char chartofind,unsigned int &textsize)
{
  unsigned int res=0;

  while ( ptr < textsize )
   {
     if ( text[ptr] == chartofind )
     {
         res = ptr;
         break;
     }
     ++ptr;
   }


  return res;
}


bool IgnoreWord(unsigned char * theword , unsigned int wordsize )
{
  bool retres = true;

  if ( wordsize > 30 ) {} else
  if ( wordsize >= 3 )
  {
    retres = false;
  }

  return retres;
}

void UpcaseIt(unsigned char * text,unsigned int textsize) //Metatrepei String se Upcase
{
    for (unsigned int i=0; i<textsize; i++)
    {
        text[i]=toupper(text[i]);
    }
}

void ExtractWords(char * text,unsigned int &textsize)
{
  printf("Extract Words called with input size %u \n",textsize);
  unsigned char found_word=0;

  unsigned char * newword;
  newword = new unsigned char [1024];
  unsigned int act_ptr=0;

  for ( unsigned int i=0; i<textsize; i++)
  {
    if ( ( text[i]==' ' ) || (text[i]=='|') )
    {

      if ( found_word == 1 ) { newword[act_ptr++]=0;
                               UpcaseIt(newword,act_ptr);
                               if ( IgnoreWord(newword , act_ptr ) == false )
                               {
                                printf("%s \n",newword);
                               }
                             }
      newword [ 0 ] = 0;
      act_ptr=0;
      found_word = 0;
    } else
    {
      newword[act_ptr++]=text[i] ,  found_word = 1;
    }

  }

  delete newword;
}


void CompressSpaces(char * text,unsigned int &textsize)
{
  printf("Compress Spaces called with input size %u \n",textsize);
  unsigned char found_space=1;
  unsigned int act_ptr=0;
  for ( unsigned int i=0; i<textsize; i++)
  {
    if ( text[i]==' ' )
    {
      if ( found_space == 1 ) { } else
      if ( found_space == 0 ) { text[act_ptr++]=' ' ,  found_space = 1;  }
    }
     else
    if ( text[i]=='|' )
    {
      text[act_ptr++]='|' ,  found_space = 1;
    } else
    if ( text[i]<' ' )
    {
      // FILTER OUT LOWER ASCII CODES!
    }
     else
    {
      text[act_ptr++]=text[i] , found_space = 0;
    }
  }
  textsize = act_ptr;
}

unsigned int ClearTextFromHTMLTags(char * text,unsigned int &textsize)
{
 unsigned int start_text_size = textsize;

 unsigned int ptr = 0;
 unsigned int token = 666; // <- GIA NA MPEI STO PRWTO WHILE LOOP!
 unsigned int token2 = 0;


 printf("ClearTextFromHTMLTags \n");

 if ( text[0]=='<' )
    {
      printf("Bug with unsigned ints :P , sloppy fix \n");
      text[0]=' ',text[1]='<';
    }

 while ( token > 0  )
 {
   token = FindFirstInstanceOfChar(ptr,text,'<',textsize);

   if ( token != 0 ) { ptr = token; }


   if (  token > 0  )
   {
    token2 = FindFirstInstanceOfChar(ptr,text,'>',textsize);

    if ( token2 > 0 )
    {
        for ( unsigned int i=token; i<=token2; i++ )
        {
             text[i]=' ';
        }
        text[token]='|'; // SIGNAL DELIMITER
      if ( token2 != 0 ) { ptr = token2; }

    }
   }


 }

 CompressSpaces(text,textsize);
 ExtractWords(text,textsize);
}
