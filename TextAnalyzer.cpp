#include "TextAnalyzer.h"
#include <stdio.h>

struct terminal_list
{
    unsigned char * terminals;
    unsigned int terminal_number;

};

unsigned int FindFirstInstanceOfChar(unsigned int ptr,char * text,char chartofind,unsigned int &textsize)
{
  unsigned int res=ptr;

  while ( ptr < textsize )
   {
     if ( text[ptr] == chartofind )
     {
         res = ptr;
         break;
     } else
     { printf ("%c is not %c ",text[ptr],chartofind);  }
     ++ptr;
   }


  return res;
}

unsigned int ClearTextFromHTMLTags(char * text,unsigned int &textsize)
{
 unsigned int start_text_size = textsize;

 unsigned int ptr = 0;
 unsigned int token = 666; // <- GIA NA MPEI STO PRWTO WHILE LOOP!
 unsigned int token2 = 0;

 terminal_list terms;
 terms.terminals = new unsigned char [2];
 terms.terminal_number = 2;

 terms.terminals[0] = '<';
 terms.terminals[1] = '>';

 printf("ClearTextFromHTMLTags \n");

 if ( text[0]=='<' )
    {
      printf("Bug with unsigned ints :P , sloppy fix \n");
      text[0]=' ',text[1]='<';
    }

 while ( ( token > 0 ) || ( ptr < textsize ) )
 {
   token = FindFirstInstanceOfChar(ptr,text,'<',textsize);
   printf("Found < at %u \n",token);
   ptr = token;


   if ( ( token > 0 ) || ( text[token]=='<' ) )
   {
    token2 = FindFirstInstanceOfChar(ptr,text,'>',textsize);
    printf("Found > at %u \n",token2);
    if ( ( token2 > 0 ) || ( text[token]=='>' ) )
    {
        printf("Erasing %u to %u \n",token,token2);
        for ( unsigned int i=token; i<=token2; i++ )
        {
             text[i]=' ';
        }
      ptr = token2;
    }
   }


 }


}
