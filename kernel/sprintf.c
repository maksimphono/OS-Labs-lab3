//
// formatted string output -- sprintf
//

#include <stdarg.h>

#include "types.h"
#include "param.h"

static char digits[] = "0123456789abcdef";

static char* 
sprintint(char* d, int bufLength, int xx, int base, int sign)
{
  char buf[16];
  int i;
  uint x;

  if(sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do {
    buf[i++] = digits[x % base];
  } while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  int count = 0;
  while (--i >= 0){
    *d++ = buf[i];
    if (++count >= bufLength) {
      return 0;
    }
  }
  return d;
}


static char* 
sprintptr(char* d, int bufLength, uint64 x)
{
  int i;
  int count = 0;
  *d++ =  '0';
  if (++count >= bufLength) {
    return 0;
  }
  *d++ = 'x';
  if (++count >= bufLength) {
    return 0;
  }
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4){
    *d++ = digits[x >> (sizeof(uint64) * 8 -4)];
    if (++count >= bufLength) {
      return 0;
    }
  }
  return d;
}

/*
 * Description: print to the string.
 * Return: number of characters written, -1 at overflow.
 */
int
sprintf(char *s, unsigned long bufLength, char *fmt, ...)
{
  va_list ap;
  int i, c, c0, c1, c2;
  char *source;
  char *start = s;
  char *current = s;
  char *end = &s[bufLength];

  va_start(ap, fmt);
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
    if(c != '%'){
      *current++ = c;
      if (current == end) {
        return -1;
      }
      continue;
    }
    i++;
    c0 = fmt[i+0] & 0xff;
    c1 = c2 = 0;
    if(c0) c1 = fmt[i+1] & 0xff;
    if(c1) c2 = fmt[i+2] & 0xff;
    if(c0 == 0){
      break;
    } else if(c0 == 'd'){
      current = sprintint(current, bufLength-(current-start), va_arg(ap, int), 10, 1);
      if (!current) { return -1; }
    } else if(c0 == 'l' && c1 == 'd'){
      current = sprintint(current, bufLength-(current-start), va_arg(ap, uint64), 10, 1);
      i++;
      if (!current) { return -1; }
    } else if(c0 == 'l' && c1 == 'l' && c2 == 'd'){
      current = sprintint(current, bufLength-(current-start), va_arg(ap, uint64), 10, 1);
      i+=2;
      if (!current) { return -1; }
    } else if(c0 == 'u'){
      current = sprintint(current, bufLength-(current-start), va_arg(ap, int), 10, 0);
      if (!current) { return -1; }
    } else if(c0 == 'l' && c1 == 'u'){
      current = sprintint(current, bufLength-(current-start), va_arg(ap, uint64), 10, 0);
      i++;
      if (!current) { return -1; }
    } else if(c0 == 'l' && c1 == 'l' && c2 == 'u'){
      current = sprintint(current, bufLength-(current-start), va_arg(ap, uint64), 10, 0);
      i+=2;
      if (!current) { return -1; }
    } else if(c0 == 'x'){
      current = sprintint(current, bufLength-(current-start), va_arg(ap, int), 16, 1);
      if (!current) { return -1; }
    } else if(c0 == 'l' && c1 == 'x'){
      current = sprintint(current, bufLength-(current-start), va_arg(ap, uint64), 16, 1);
      i++;
      if (!current) { return -1; }
    } else if(c0 == 'l' && c1 == 'l' && c2 == 'x'){
      current = sprintint(current, bufLength-(current-start), va_arg(ap, uint64), 16, 1);
      i+=2;
      if (!current) { return -1; }
    } else if(c0 == 'p'){
      current = sprintptr(current, bufLength-(current-start), va_arg(ap, uint64));
      if (!current) { return -1; }
    } else if(c0 == 's'){
      if((source = va_arg(ap, char*)) == 0)
        source = "(0)";
      for(; *source; source++) {
        *current++ = *source;
        if (current == end) {
          return -1; 
        }
      }
    } else if(c0 == '%'){
      *current++ = '%';
      if (current == end) {
        return -1;
      }
    } else {
      // Print unknown % sequence to draw attention.
      *current++ = '%';
      if (current == end) {
        return -1;
      }
      *current++ = c;
      if (current == end) {
        return -1;
      }
    }
  }
  *current++ = '\0';
  return current - start;
}
