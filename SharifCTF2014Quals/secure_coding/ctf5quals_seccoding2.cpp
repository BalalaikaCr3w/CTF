
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

	int size;
	printf("Line size: ");
	scanf("%d", &size);
	gets_s(tmp, sizeof(tmp));

	char *line = (char *)malloc(size + 1);

	_set_invalid_parameter_handler(continueHandler);

	restart:
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

		int offset;
		if (!(offset = atoi(tmp)) || offset - 1 > (int)strlen(line))
		{
			printf("ERROR\n");
			continue;
		}

		printf("Enter the text: ");
		gets_s(tmp, sizeof(tmp));

		if (_snprintf(line + (offset - 1), size - (offset - 1) + 1, tmp) < 0)
		{
			printf("ERROR\n");
			line[size] = 0;
			continue;
		}
	}

	printf("Restart (y/n): ");
	scanf("%s", tmp);
	if (tmp[0] == 'y')
	{
		gets_s(tmp, sizeof(tmp));
		goto restart;
	}
	
	return -14;
}
