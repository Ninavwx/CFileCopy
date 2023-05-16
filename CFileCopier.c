/*

Program to copy files

The program receives parameters to the main:
first parameter: textCopy/textBinary - a string that represents the type of file text or binary
second parameter: path of the source file
third parameter: path of the copied file

ex: program.exe textCopy source_file  destination_file

-> If there is any error with the files or if there is a missing file, tell the use what is the error and exit program:
Invalid execution.
Usage: copyFile (textCopy|binaryCopy) <sourceFilename_filename> <destFilenameination_filename>

-> if the destination exists the user chooses if to overwrite it or to exit
Do you want to overwrite? 0 (exit) / 1 (success)

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define TRUE 1
#define FALSE !TRUE

#define TEXT_FILE_STR "textCopy"
#define BINARY_FILE_STR "binaryCopy"

#define TEXT_FILE_INT 0
#define BINARY_FILE_INT 1
#define ERROR_FILE_INT -1

#define STRCMP_EQUAL 0

#define READ_TEXT_FILE_MODE "r"
#define WRITE_TEXT_FILE_MODE "w"
#define READ_BINARY_FILE_MODE "rb"
#define WRITE_BINARY_FILE_MODE "wb"

#define BUFFER_SIZE 8192

char* stringFromArrStrings(char** strArr, int strIndex);
int copyFiles(char* sourceFileName, char* destinationFileName, int typeFileInt);
int fileExists(char* fileName);
int getTypeOfFile(char** arrStr, int strIndex);
void copyTextFile(FILE* sourceFile, FILE* destinationFile);

void copyBinaryFile(FILE* sourceFile, FILE* destinationFile);


int main(int argc, char** argv)
{
	// parameters constants
	const int NUM_PARAMETERS = 4; // 1.programName 2.(textFile/binaryFile) 3. sourceFile 4. destFile
	const int INDEX_TYPE_FILE = 1;
	const int INDEX_SOURCE_FILE = 2;
	const int INDEX_DESTINATION_FILE = 3;

	const char PARAMETERS_ERROR[] = "Invalid execution.\nUsage: copyFile(textCopy | binaryCopy) < sourceFilename_filename > < destFilenameination_filename>";
	const char COPY_COMPLETED_TEXT[] = "Copy completded.";


	int typeFile = 0;
	char* sourceFileName = NULL;
	char* destinationFileName = NULL;

	int hasParameterError = FALSE;


	// checking if the numbers of parameters are valid
	if (argc == NUM_PARAMETERS)
	{

		// get the type of file 
		typeFile = getTypeOfFile(argv, INDEX_TYPE_FILE);

		// if there is an error in the type of file
		if (typeFile == ERROR_FILE_INT)
		{
			hasParameterError = TRUE;	
		}
		else
		{

			// get the name of the source file
			sourceFileName = stringFromArrStrings(argv, INDEX_SOURCE_FILE);
			
			// get the name of the destination file
			destinationFileName = stringFromArrStrings(argv, INDEX_DESTINATION_FILE);
			
			// copy from one file to the other
			if (copyFiles(sourceFileName, destinationFileName, typeFile)) // if the copy was successful
			{
				printf("%s", COPY_COMPLETED_TEXT);
			}
			
		}

	}
	
	// check if it broke because has a parameter error
	if(hasParameterError)
	{
		printf(PARAMETERS_ERROR);
	}



	getchar();
	return 0;
}

/*
Function to copy from one file to another, text file or binary file
the function checks if the destination file exists and if so asks if the user wants to overwrite it
input: path of the source file, path of the destinaiton file, and type of file (text/binary)
output: if the copy was successful or not
*/
int copyFiles(char* sourceFileName, char* destinationFileName, int typeFileInt)
{
	// REMEMBER PROBLEM WITH wb and rb
	int isSuccessful = FALSE;
	int overwriteFile = TRUE;
	FILE* sourceFile = NULL;
	FILE* destinationFile = NULL;

	if (fileExists(destinationFileName)) // if the destination already exists, ask if wants to overwrite
	{
		// Ask if user wants to overwrite the file
		printf("\nDo you want to overwrite? 0 (no) / 1 (yes)\n");
		scanf("%d", &overwriteFile);
	}

	if (overwriteFile)
	{
		if (typeFileInt == TEXT_FILE_INT) // if the files are text files
		{
			sourceFile = fopen(sourceFileName, READ_TEXT_FILE_MODE);

			// check if the source file exists
			if (sourceFile == NULL)
			{
				printf("\n%s File does not exist.", sourceFileName);
				isSuccessful = FALSE;
			}
			else
			{
				destinationFile = fopen(destinationFileName, WRITE_TEXT_FILE_MODE);

				// check if the  creation of destination file was succesful
				if (destinationFile == NULL)
				{
					isSuccessful = FALSE;
					printf("\nCreation of destination file not succesful ");
				}
				else // if so, copy the text from the source file to the destination file
				{
					// COPY TEXT FILE
					copyTextFile(sourceFile, destinationFile);
					isSuccessful = TRUE;
					
					// free the allocated memory and close files
					fclose(sourceFile);
					fclose(destinationFile);
				
				}
			}
		}
		else if (typeFileInt == BINARY_FILE_INT) // if the files are binary file
		{
			sourceFile = fopen(sourceFileName, READ_BINARY_FILE_MODE);

			if (sourceFile == NULL)
			{
				printf("\n%s File does not exist.", sourceFileName);
				isSuccessful = FALSE;
			}
			else
			{
				destinationFile = fopen(destinationFileName, WRITE_BINARY_FILE_MODE);

				// check if the  creation f destination file was succesful
				if (destinationFile == NULL)
				{
					isSuccessful = FALSE;
					printf("\nCreation of destination file not succesful ");
				}
				else // if so, copy the text from the source file to the destination file
				{

					copyBinaryFile(sourceFile, destinationFile);
					// free the allocated memory and close files
					fclose(sourceFile);
					fclose(destinationFile);

				}
			}
		}

	}

	return isSuccessful;


}

/*
Function to copy from a binary file to another
input: source file and destination file
output: none
*/
void copyBinaryFile(FILE* sourceFile, FILE* destinationFile)
{
	int fileSize = 0;
	char* fileData = NULL;
	char* bufferStr = NULL;

	// get source file size
	fseek(sourceFile, 0, SEEK_END);
	fileSize = ftell(sourceFile);

	// set the file to the start again
	fseek(sourceFile, 0, SEEK_SET);

	// allocate memory to the buffer
	bufferStr = (char*)malloc(sizeof(char) * fileSize);

	fread(bufferStr, sizeof(char), fileSize, sourceFile);
	fwrite(bufferStr, sizeof(char), fileSize, destinationFile);

	free(bufferStr);

}
	

/*
function to copy from a text file to another
input: the source file and the destination file
output: none
*/
void copyTextFile(FILE* sourceFile, FILE* destinationFile)
{
	char currentChar = ' ';
	while ( (currentChar = (char)fgetc(sourceFile)) != EOF) // EOF is end of line
	{
		fputc(currentChar, destinationFile);
	}

}


/*
Function to check if a file exists
input: char* of the file name
output: int (0/1) if the file exists or not
*/
int fileExists(char* fileName)
{
	int fileExists = FALSE;
	FILE* file;
	
	if ((file = fopen(fileName, READ_BINARY_FILE_MODE))) // just to check opening in binary mode a text file also works
	{
		fclose(file);
		fileExists = TRUE;
		printf("file exists");
	}
	else
	{
		printf("file not exists");
	}

	return fileExists;
}

/*
Function to get the type of file from the main parameter
input: the string array from the main parameters, the index of the file type
output: the file type 
*/
int getTypeOfFile(char** arrStr, int strIndex)
{

	char* typeFileStr = NULL;
	int typeFileInt = 0;

	// get the string of the file type
	typeFileStr = stringFromArrStrings(arrStr, strIndex);

	// check the type of file
	if (strcmp(typeFileStr, TEXT_FILE_STR) == STRCMP_EQUAL) // if the file is a text file
	{
		typeFileInt = TEXT_FILE_INT;
	}
	else if (strcmp(typeFileStr, BINARY_FILE_STR) == STRCMP_EQUAL) // if the file is a binary file
	{
		typeFileInt = BINARY_FILE_INT;
	}
	else // if it is an invalid type of file
	{
		typeFileInt = ERROR_FILE_INT;
	}

	// if the string is not null, free the memory
	if (typeFileStr != NULL)
	{
		free(typeFileStr);
	}

	return typeFileInt;

}

/*
Function to dynamically create a string (*char) from an array of strings (**char)
input: the array of strings and the index to get the string from
output: the dynamically allocated string

OBSERVATION:
1. Make sure the index exists in the array
2. Remember to free the memory in the end
*/
char* stringFromArrStrings(char** strArr, int strIndex)
{
	
	char* finalString = NULL;
	int lenString = strlen(strArr[strIndex]);

	
	finalString = (char*)malloc(sizeof(char) * (lenString+1) );
	
	strcpy(finalString, strArr[strIndex]);
	return finalString;

}
