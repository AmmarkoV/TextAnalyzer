#include "TextAnalyzer.h"
#include <stdio.h>

struct terminal_list
{
    unsigned char * terminals;
    unsigned int terminal_number;

};

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

void CompressSpaces(char * text,unsigned int &textsize)
{
  unsigned char found_space=1;
  unsigned int act_ptr=0;
  for ( int i=0; i<textsize; i++)
  {
    if ( text[i]==' ' )
    {
      if ( found_space == 1 ) {} else
      if ( found_space == 0 ) { text[act_ptr++]=' ' ,  found_space = 1;  }
    }
     else
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

 while ( token > 0  )
 {
   token = FindFirstInstanceOfChar(ptr,text,'<',textsize);
   printf("Found < at %u \n",token);
   if ( token != 0 ) { ptr = token; }


   if (  token > 0  )
   {
    token2 = FindFirstInstanceOfChar(ptr,text,'>',textsize);
    printf("Found > at %u \n",token2);
    if ( token2 > 0 )
    {
        printf("Erasing %u to %u \n",token,token2);
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
}
