#include <stdlib.h>
#include <new>
#include <duma.h>
#include <dumapp.h>

int main()
{
  int *pi;
  int i;
#ifdef DUMA_EXPLICIT_INIT
  duma_init();
#endif
  pi = (int*)malloc(10*sizeof(int));
  for(i=0; i<10; ++i)
    pi[i] = i;
  delete pi;
  return 0;
}
