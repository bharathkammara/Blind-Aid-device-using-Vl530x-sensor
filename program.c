
#include<stdio.h>
#include<string.h>

int main()
{
    char str1[20]="abc";
    char str2[10]="12345";
    char str3[] = "abc-12345-def";
    
    printf("%s\n","strncat function");
    strncat(str1,str2,3);                   //dest: str1 ;src: str2 ; bytes:3
    printf("%s\n",str1);
    
    char temp[10];
    strncpy(temp,str2,3);                   //dest: temp ;src: str2 ; bytes:3
    printf("%s\n","strncpy function");
    printf("%s\n",temp);
    
    printf("%s\n","strtok function");
    char* token = strtok(str3,"-");        //src: str3 ; delimiters: -
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, "-");
    }
return 0;
}

\\\\\\\\\\\\\\\\\\\\\\\\\\
/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

// Function to implement strncat() function in C
char* my_strncat(char* destination, const char* source, size_t num)
{
    char* p = destination + strlen(destination); // make p point to the end of destination string
    while (*source != '\0' && num--)            // Appends characters of source to the destination string
        *(p++) = *(source++);
    *p = '\0';                                // null terminate destination string after appending
    return destination;                         // destination string is returned by standard strncat()
}


// Function to implement strncpy() function
char* my_strncpy(char* destination, const char* source, size_t num)
{
    
    if (destination == NULL)                    // return if no memory is allocated to the destination
        return NULL;

    char* ptr = destination;                    // take a pointer pointing to the beginning of destination string
    while (*source && num--)                    // copy first num characters of C-string pointed by source into the array pointed by destination
    {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';                        // null terminate destination string after copying
    return ptr;                                 // destination is returned by standard strncpy()
}

char* sp = NULL; // the starting position of the string
char* my_strtok(char* str, const char* delimiters)
{
    int i = 0;
    int len = strlen(delimiters);
 
    if(len == 0)                // check if delimiters is not empty
        printf("delimiters are empty\n");
 
    if(!str && !sp)             //if the string is empty,return NULL
        return NULL;
    else if(str && !sp)         // initialize the sp during the first call
        sp = str;
 
                                // find the start of the substring, skip delimiters
    char* p_start = sp;
    while(true)
    {
        for(i = 0; i < len; i ++)
        {
            if(*p_start == delimiters[i]) //
            {
                p_start ++;
                break;
            }
        }
        if(i == len)
        {
            sp = p_start;
            break;
        }
    }
 
    // return NULL if nothing left
    if(*sp == '\0')
    {
        sp = NULL;
        return sp;
    }
 
    //find the end of the substring, and replace the delimiter with null
    while(*sp != '\0')
    {
        for(i = 0; i < len; i ++)
        {
            if(*sp == delimiters[i])
            {
                *sp = '\0';
                break;
            }
        }
        sp ++;
        if (i < len)
            break;
    }
    return p_start;
}


int main()
{
    char str1[20]="abcd";
    char str2[10]="12345";
    char str3[] = "abc-12345-def";
    
    printf("%s\n","strncat function");
    my_strncat(str1,str2,3);                   //dest: str1 ;src: str2 ; bytes:3
    printf("%s\n",str1);
    
    char temp[10]="abcdef";
    my_strncpy(temp,str2,3);                   //dest: temp ;src: str2 ; bytes:3
    printf("%s\n","strncpy function");
    printf("%s\n",temp);
    
    printf("%s\n","strtok function");
    char* token = my_strtok(str3,"-");        //src: str3 ; delimiters: -
    while (token != NULL) {
        printf("%s\n", token);
        token = my_strtok(NULL, "-");
    }
return 0;
}
