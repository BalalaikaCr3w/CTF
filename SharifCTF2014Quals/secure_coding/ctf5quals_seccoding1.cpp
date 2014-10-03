
#include <stdio.h>
#include <string.h>

int main()
{
	char str[1000];
	
	printf("SPLITTER\n");
	printf("--------\n");
	printf("\n");

	str[0] = 0;
	while (1)
	{
		char temp[50];
		scanf("%50s", temp);

		if (strcmp(temp, ".") == 0)
			break;

		strncat(str, temp, sizeof(str));
		strncat(str, "\n", sizeof(str));
	}

	printf("\n%s\n", str);

	return -14;
}
