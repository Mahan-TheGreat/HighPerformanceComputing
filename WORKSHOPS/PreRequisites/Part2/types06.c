 #include <stdlib.h>
 #include <stdio.h>

int main() 
{
   printf("Strings are just arrays of chars\n");
   
   char *message1 = "hello";
   char *message2 = "kevan";

   printf("%s %s\n", message1, message2); 
   printf("Look in /usr/include/string.h for functions\n");
   printf("that can be applied. Each has a man page.\n");
   return EXIT_SUCCESS;
 }
 