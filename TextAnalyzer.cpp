#include "HashFunctions.h"
#include "TextAnalyzer.h"


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// WORD COLLECTION KEEPING
inline void SanityProblem()
{
     fprintf(stderr,"Sanity Byte problem :S \n ");
}

bool CollectionOk(struct word_collection * acol)
{
   if ( acol != 0 ) {  if ( acol->sanity_byte != 666 ) { SanityProblem(); return false;}
                       return true;
                    }
    return false;
}
bool DeleteWordCollection(struct word_collection * acol)
{
   if ( acol != 0 ) {  if ( acol->sanity_byte != 666 ) { SanityProblem(); return false;}
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
   acol->sanity_byte = 666;
   return true;
}

unsigned int FindWordAtCollection(word_collection * acol,unsigned char * word,unsigned int length)
{
 if ( !CollectionOk(acol) ) { return 0; }

 // SERIAL SEARCH ! , TO BE IMPROVED :P
 unsigned int hash_data = hash (word);
 for ( unsigned int i = 0; i < acol->words_number; i++ )
 {
   // TODO
 }

 return 0;
}

bool AddWord2Collection( word_collection * acol,unsigned char * word,unsigned int length)
{
   unsigned int hash_data = hash (word);
   // TODO
   return true;
}

bool DeleteWordFromCollection( word_collection * acol,unsigned char * word,unsigned int length)
{
   // TODO
   return true;
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
