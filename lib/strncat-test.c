
#include <stddef.h>
char *strncat(char *dest, const char *src, size_t n){
    char * result = dest;
    while (*dest != 0)
        dest++;
    while (n > 0){
    	*dest++ = *src++;
    	n--;
    	if(*src == 0){
    		return result;
    	}
    }
    *dest = 0;
    return result;
}

int main(){
  char str1[20];
  char str2[20];
  strcpy (str1,"To be ");
  strcpy (str2,"or not to be");
  strncat (str1, str2, 6);
  puts (str1);
  return 0;
}