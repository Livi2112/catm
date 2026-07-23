#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
 

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
usageError()
{
	die("[ERROR]: Usage \"catm <FILENAME> -<FLAGS>\"\nTry \"catm -h\" or \"catm --help\" for usage and flags specification\n");
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
printFileInfo(const char* filename)
{
	struct stat fileInfo;

	if(stat(filename, &fileInfo) != 0)
	{
		usageError();
	}

	printf("=================================\n");
	printf(" FILE INFO: %s\n", filename);
	printf("=================================\n");

	printf("Type: ");
	if(S_ISREG(fileInfo.st_mode)) printf("Regular\n");
	else if(S_ISDIR(fileInfo.st_mode)) printf("Dir\n");
	else if(S_ISLNK(fileInfo.st_mode)) printf("Link\n");
	else printf("Unable to identify\n");

	double sizeKB = fileInfo.st_size / 1024.0;
	printf("Size: %ld bytes (%.2f KB)\n", fileInfo.st_size, sizeKB);

	printf("Permissions: 0%o\n", fileInfo.st_mode & 0777);
	
	struct passwd *pw = getpwuid(fileInfo.st_uid);
	struct group *gr = getgrgid(fileInfo.st_gid);
	printf("Owner: %s\n", pw ? pw->pw_name : "Unknown");
	printf("Group: %s\n", gr ? gr->gr_name : "Unknown");

	char date[100];
	struct tm *timeInfo = localtime(&fileInfo.st_mtime);
	strftime(date, sizeof(date), "%d/%m/%Y %H:%M:%S", timeInfo);
	printf("Last modified in: %s\n", date);
}


void
printFileContent(const char* filename)
{
	FILE* f = fopen(filename, "r");
	if(f == NULL)
	{
		die("[ERROR] File not loaded correctly, consider checking filename and usage");
	}

	char data[1024];
	
	while(fgets(data, 50, f) != NULL)
	{
		printf("%s", data);
	}

	fclose(f);
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
		printFileInfo(filename);
	}

	if(!(flags & FLAG_LESS))
	{
		printFileContent(filename);
	}
	
	return EXIT_SUCCESS;
}
