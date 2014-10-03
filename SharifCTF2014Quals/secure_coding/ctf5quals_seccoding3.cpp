
#include <stdio.h>
#include <string.h>
#include <Windows.h>

#include <cstdarg>
#include <vector>
#include <iostream>
int main()
{
	printf("  WI  \n");
	printf("------\n");
	printf("\n");

	std::vector<WCHAR> commandBuffer(1000 + 1, L'\0');
	std::vector<WCHAR> trashBuffer(1000 + 1, L'\0');
	std::vector<WCHAR> textBuffer(1024 + 1, L'\0');

	int lineNumber = 1;

	size_t linePositionInText = 0;

	for (;;)
	{
		printf("%d: ", lineNumber);

		std::fill(commandBuffer.begin(), commandBuffer.end(), L'\0');
		wscanf(L"%1000[a-zA-Z0-9?,.!()%:- ]", commandBuffer.data());

		std::fill(trashBuffer.begin(), trashBuffer.end(), L'\0');
		fgetws(trashBuffer.data(), trashBuffer.size(), stdin); //NORMAL MAY BE

		if (wcscmp(commandBuffer.data(), L":done") == 0)
			break;

		if (wcsncmp(commandBuffer.data(), L":line ", 6) == 0)
		{
			swscanf(commandBuffer.data() + 6, L"%d", &lineNumber);

			if (lineNumber < 0 || lineNumber > 1000) return -1;

			if (lineNumber == 0)
			{
				lineNumber = 1;
			}

			linePositionInText = 0;

			for (int i = 0; i < lineNumber && linePositionInText < textBuffer.size(); linePositionInText++)
			{
				if (textBuffer[linePositionInText] == L'\n')
				{
					i++;
				}
			}

			continue;
		}

		// replace arguments
		int cnt = 0;

		for (WCHAR* ptr2 = commandBuffer.data();;)
		{
			ptr2 = wcschr(ptr2, L'%');
			if (ptr2 == NULL)
				break;

			if (ptr2[1] == L'%')  //%%???
			{
				if (ptr2[2] == L'x' || ptr2[2] == L's' || ptr2[2] == L'i' || ptr2[2] == L'd' || ptr2[2] == L'u' || ptr2[2] == L'l') break;
				for (int i = 0; i < wcslen(ptr2); i++)
				{
					ptr2[i] = ptr2[i + 1];  //1????
				}

				ptr2--;

				if (ptr2 < commandBuffer.data()) break;  //!!!!
			}
			else if (ptr2[1] == L'l')
			{
				cnt++;

				ptr2[1] = L'd';
			}
			else
			{
				for (int i = wcslen(ptr2) - 1; i >= 0; i--)
				{
					ptr2[i + 1] = ptr2[i];
				}
			}

			if (ptr2 >= commandBuffer.data() + commandBuffer.size() - 1) break;	//!!
			ptr2 += 2;
		}

		std::vector<int> arg_list(cnt, lineNumber);

		int space = _vsnwprintf(NULL, 0, commandBuffer.data(), reinterpret_cast<va_list>(arg_list.data()));

		textBuffer.insert(textBuffer.begin() + linePositionInText, space + 1, L'\0');

		_vsnwprintf(&textBuffer[linePositionInText], space, commandBuffer.data(), reinterpret_cast<va_list>(arg_list.data()));

		textBuffer[linePositionInText + space] = L'\n';
		
		linePositionInText += space + 1;

		lineNumber++;
	}

	wprintf(L"\n\n");
	wprintf(L"THE FINAL TEXT\n");
	wprintf(L"==============\n");

	wprintf(L"%s\n\n", textBuffer.data());

	return -14;
}
