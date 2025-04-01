#include <stdio.h>
#include <stdbool.h>

bool checkValidation(const char *str)
{
  int i = 0;

  if (str == NULL)
    return false;

  while (str[i] == 'a')
  {
    i++;
  }

  if (str[i] == 'b' && str[i + 1] == 'b' && str[i + 2] == '\0')
  {
    return true;
  }

  return false;
}

int main()
{
  char str[100];

  printf("Enter a string: ");
  scanf("%99s", str);

  bool ans = checkValidation(str);

  printf("Validation result: %s\n", ans ? "Valid" : "Invalid");

  return 0;
}
