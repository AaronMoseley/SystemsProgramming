#include <stdio.h>

int streqnocase(const char *a, const char *b)
{
	if(a[0] == '\0' && b[0] == '\0')
	{
		return 1;
	} else if(a[0] == '\0' || b[0] == '\0')
	{
		return 0;
	}

	int index = 0;

	while(a[index] != '\0' && b[index] != '\0')
	{
		char newAChar = (a[index] >= 65 && a[index] <= 90) ? a[index] + 32 : a[index];
		char newBChar = (b[index] >= 65 && b[index] <= 90) ? b[index] + 32 : b[index];

		if(newAChar != newBChar)
		{
			return 0;
		} else {
			index++;
		}
	}

	if(a[index] != '\0' || b[index] != '\0')
	{
		return 0;
	}

	return 1;
}
