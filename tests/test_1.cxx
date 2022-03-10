#include <stdio.h>
//int testExtraStuff3();
int test_extra_stuff();
//int testExtraStuff2();

int test_1(int ac, char* av[])
{
  //if (!testExtraStuff2()) {
  //  return -1;
  //}
  if (!test_extra_stuff()) {
    return -1;
  }
  //if (!testExtraStuff3()) {
  //  return -1;
  //}

  printf("test1\n");
  for (int i = 0; i < ac; i++)
    printf("arg %d is %s\n", ac, av[i]);
  return 0;
}
