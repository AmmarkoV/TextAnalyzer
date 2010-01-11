#include "TextAnalyzer.h"
#include "HashFunctions.h"

word_collection * TextAnalyzer_NewWordCollection()
{
 word_collection * ret;
 if ( NewWordCollection(ret) ) {  } else
                               { fprintf(stderr,"Could not commit memory for word collections\n");}
 return ret;
}

void TextAnalyzer_DeleteWordCollection(word_collection * acol)
{
 DeleteWordCollection(acol);
}

unsigned int TextAnalyzer_WordOccurances(word_collection * acol,unsigned char * theword , unsigned int wordsize)
{
 return GetWordOccurances(acol,theword,wordsize);
}

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



void ExtractWords(word_collection * acol,char * text,unsigned int &textsize)
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
                                 AddWord2Collection(acol,newword,act_ptr);
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

unsigned int ClearTextFromHTMLTags(word_collection * acol,char * text,unsigned int &textsize)
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
 ExtractWords(acol,text,textsize);
 return 0;
}
