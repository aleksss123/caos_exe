#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <windows.h>

int
main(argc, argv)
        int argc;
        char** argv;
{
        HANDLE hFile = CreateFileA(
		argv[1],
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		goto fail;
	}
	LPDWORD pointer;
	LARGE_INTEGER fake_pointer;
	fake_pointer.QuadPart = pointer; 
	do
	{
		BOOL seek = SetFilePointerEx(
			hFile,
			fake_pointer,
			NULL,
			FILE_BEGIN
		);
		if (!seek)
		{
			goto fail;
		}
		int element = 0;
		LPDWORD bytes_read = 0;
		BOOL read = ReadFile(
			hFile,
			&element,
			sizeof(element),
			bytes_read,
			NULL
		);
		if (!read || !bytes_read)
		{
			goto fail;
		}
		printf("%d\n", element);
		BOOL pointer_read = ReadFile(
			hFile,
			&pointer,
			sizeof(pointer),
			bytes_read,
			NULL
		);
		if (!pointer_read)
		{
			goto fail;
		}
	}
	while (pointer != 0);
	CloseHandle(hFile);
	return 0;
 fail:
	CloseHandle(hFile);
	return 1;
}
