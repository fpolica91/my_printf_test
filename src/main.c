#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TRUE  1
#define FALSE 0

typedef char bool;

int my_putchar(char c) {
  return write(1, &c, 1);
}

char* itoa(int value, char* result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) { *result = '\0'; return result; }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );

  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

int write_ptr(void *p) {
    uintptr_t x = (uintptr_t)p;
    char buf[2 + sizeof(x) * 2];
    size_t i;
    buf[0] = '0';
    buf[1] = 'x';
    for (i = 0; i < sizeof(x) * 2; i++) {
        buf[i + 2] = "0123456789abcdef"[(x >> ((sizeof(x) * 2 - 1 - i) * 4)) & 0xf];
    }
    return write(1, buf, sizeof(buf));
}

int print_int(int c) {
  char tmpbuf[30];
  itoa(c, tmpbuf, 10);
  return write(1, tmpbuf, strlen(tmpbuf));
}

int my_putstr(char *str) {
  int index = 0;
  
  while (str[index] != '\0') {
    my_putchar(str[index]);
    index += 1;
  }
  return index;
}

void proceed_flag(va_list *arg, char flag) {
  if (flag == 's') {
    char * str = va_arg(*arg, char *);
    if (str) {
      my_putstr(str);
    }
  }
  else if (flag == 'c') {
    char c = va_arg(*arg, int);
    my_putchar(c);    
  }
  else if (flag == 'd') {
    int digit = va_arg(*arg, int);
    print_int(digit);   
  }
}
// ello

int my_printf(char* restrict format, ...) {
  va_list arg;
  int index = 0;
  bool flag_spotted = FALSE;

  va_start(arg, format); 
  while (format[index] != 0)
  {
    if (flag_spotted) {
      if(format[index] == 'p'){
         write_ptr(&arg);
      }else{
        proceed_flag(&arg, format[index]);
        flag_spotted = FALSE;
      }
    }
    else if (format[index] == '%') {
      flag_spotted = TRUE;
    }
    else {
      my_putchar(format[index]);
    }
    index += 1;
  }
  va_end(arg);
  return 0;
}

int main(){


  // printf("%p \n", ptr);
  char myStr[5] = "hello";
  char * ptStr = myStr;           // store 16-bit address, and then pretend that memory is a 

  
  // write_ptr(&ptStr);
  // printf("OMG NEXT \n");
  my_printf("Custom Printf Result => %p \n", myStr);
  printf("Original Prinft => %p\n", &myStr);
    // my_printf("Hello world!\n");
    // my_printf("%c!\n", 'H');
    // my_printf("the char is %c \n", 'a');
    // my_printf("the string is is %s Count: %d \n", "hello world", 6);
    // my_printf("%c%c%c%c%c!\n", 'H', 'e', 'l', 'l', 'o');
}
