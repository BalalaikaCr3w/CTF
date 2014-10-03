
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <string>

using namespace std;

void continueHandler(const wchar_t * expression,
	const wchar_t * function, const wchar_t * file, 
	unsigned int line, uintptr_t pReserved)
{
	// just continue
}

void safefree(char **ptr)
{
	if (*ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

int main()
{
	char tmp[50];

	printf("Line Editor\n");
	printf("-----------\n\n");

restart:
	int size;
	printf("Line size: ");
	scanf("%d", &size);
	if (size <= 0) return -1; 
	gets_s(tmp, sizeof(tmp));

	char *line = (char *)calloc(1, size + 1);
	if (NULL == line) return -1;

	_set_invalid_parameter_handler(continueHandler);

	line[0] = 0;

	while (1)
	{
		printf("Enter the offset (or \"quit\"): ");
		gets_s(tmp, sizeof(tmp));

		if (!strcmp(tmp, "quit"))
		{
			printf("OUTPUT LINE: %s\n\n", line);
			safefree(&line);
			break;
		}

		int offset = atoi(tmp);
		if (offset <= 0 || offset - 1 >= size ||  offset - 1 > (int)strlen(line))
		{
			printf("ERROR\n");
			continue;
		}

		printf("Enter the text: ");
		gets_s(tmp, sizeof(tmp));

		if (strlen(tmp) > size - (offset - 1))
		{
			printf("ERROR\n");
			line[size] = 0;
			continue;
		}
		else {
			strncpy(line + (offset - 1), tmp, size - (offset - 1));
		}
	}

	printf("Restart (y/n): ");
	scanf("%1s", tmp);
	if (tmp[0] == 'y')
	{
		gets_s(tmp, sizeof(tmp));
		goto restart;
	}

	return -14;
}
