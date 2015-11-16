/* example.i */
 %module example
 %{
   /* Put header files here or function declarations like below */
   // #include "example.c"
   extern void imprime();
   /*
   extern double My_variable;
   extern  int fact(int n);
   extern int my_mod(int x, int y);
   extern char *get_time();
   */
   %}
    extern void imprime();
//%include "example.c"
//extern void imprime(void);
/* 
extern double My_variable;
extern int fact(int n);
extern int my_mod(int x, int y);
extern char *get_time();
*/
