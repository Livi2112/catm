#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 

// Flag bitmask
#define FLAG_HELP (1 << 0)
#define FLAG_INFO (1 << 1)
#define FLAG_LESS (1 << 2)
#define FLAG_HEX  (1 << 3)


void
die(const char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}


void
printHelp()
{
	printf("catm stands for cat and more\n");
	printf("Usage: \"catm <FILENAME> -<FLAG>\"\n");
	printf("Flags:\n");
	printf("  -h, --help    Print this message and exit\n");
	printf("  -l, --less    Does not print file content\n");
	printf("  -i, --info    Print file information\n");
	printf("  -x, --hex     Print file in hexdump format\n");
}	


void
printFileContent(const char* filename)
{
	FILE* f = fopen(filename, "r");
	if(f == NULL)
	{
		die("[ERROR] File not loaded correctly, consider checking filename and usage");
	}

}


void
usageError()
{
	die("[ERROR]: Usage \"catm <FILENAME> -<FLAGS>\"\nTry \"catm -h\" or \"catm --help\" for usage and flags specification\n");
}


int
main(int argc, char* argv[])
{	

	unsigned int flags = 0;
	char *filename = NULL;

	// Wrong arguments/usage handling
	if(argc < 2)
	{
	       usageError();	
	}

	// Process flags and add to bitmask
	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
		{
			flags |= FLAG_HELP;
			continue;			
		}

		if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--info") == 0)
		{	
			flags |= FLAG_INFO;
			continue;
		}
	
		if(strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--less") == 0)
		{	
			flags |= FLAG_LESS;
			continue;
		}

		if(strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--hex") == 0)
		{
			flags |= FLAG_HEX;
			continue;
		}
		
		if(filename != NULL)
		{
			usageError();
		}

		filename = argv[i];
	}	

	// Handle help flag
	if(flags & FLAG_HELP)
	{
		printHelp();
		return EXIT_SUCCESS;
	}

	// Handle info flag
	if(flags & FLAG_INFO)
	{
		printf("info");
	}

	if(!(flags & FLAG_LESS))
	{
		printFileContent(argv[1]);
	}
	
	return EXIT_SUCCESS;
}
