// ------------------------------------------------------------------------------------------
// Name: Eric Bambach
// Class: C Programming 1 (151-001)
// Abstract: Homework 8
// Using the the Addresses1.txt file as input, parse the addresses to udtAddressType 
// audtAddresses[] and print the data after parsing. The data in the txt file is always in the 
// format of “RecordID, Full Name, Street, City, State, ZipCode” and the full name may or may 
// not include a middle name.
// ------------------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS
// ------------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>

// ------------------------------------------------------------------------------------------
// Constants
// ------------------------------------------------------------------------------------------



// ------------------------------------------------------------------------------------------
// Structs
// ------------------------------------------------------------------------------------------
typedef struct
{
	long lngRecordID;
	char strFirstName[50];
	char strMiddleName[50];
	char strLastName[50];
	char strStreet[100];
	char strCity[50];
	char strState[50];
	char strZipCode[50];
} udtAddressType;



// ------------------------------------------------------------------------------------------
// Prototypes, where procedures are defined so the compiler knows what to look for.
// ------------------------------------------------------------------------------------------
int OpenInputFile(char strFileName[], FILE** ppfilInput);
bool FindIfMiddleNamePresent(char strSource[]);
void ParseAddresses(char strSource[], udtAddressType audtAddresses[], int intIndex);
void OutputAddresses(udtAddressType audtAddresses[], int intFilledStructSize);



// ------------------------------------------------------------------------------------------
// Name: main
// Abstract: This is where the program starts, this particular practice is for procedures.
// ------------------------------------------------------------------------------------------
int main()
{
	// Declare a file pointer
	FILE* pfilInput = 0;
	int intResultFlag = 0;
	char strBuffer[100] = "";
	char strDestination[100] = "";
	char chrCharacter = 0;
	udtAddressType audtAddresses[100];
	int intIndex = 0;
	int intFilledStructSize = 0;

	// Try to open the file for reading (notice you have to double up the backslashes)
	intResultFlag = OpenInputFile("..\\Addresses\\Addresses1.txt", &pfilInput);

	// Was the file opened?
	if (intResultFlag == 1)
	{
		// Yes, read in records until end of file( EOF )
		while (feof(pfilInput) == 0)
		{
			// Read next line from file
			fgets(strBuffer, sizeof(strBuffer), pfilInput);
			
			// Load all the addresses from the file into the array of structures
			ParseAddresses(strBuffer, audtAddresses, intIndex);
			intIndex++;
		}
		// Clean up
		fclose(pfilInput);
		
		intFilledStructSize = intIndex;
		//Print out all the addresses
		OutputAddresses(audtAddresses, intFilledStructSize);
	}	

	system("pause");

	return 0;
}



// ------------------------------------------------------------------------------------------
// Name: OpenInputFile
// Abstract: Open the file for reading.  Return true if successful.
// ------------------------------------------------------------------------------------------
int OpenInputFile(char strFileName[], FILE** ppfilInput)
{

	int intResultFlag = 0;

	// Open the file for reading
	*ppfilInput = fopen(strFileName, "rb");

	// Success?
	if (*ppfilInput != 0)
	{
		// Yes
		intResultFlag = 1;
	}
	else
	{
		// No
		printf("Error opening %s for reading!!!\n", strFileName);
	}

	return intResultFlag;

}



// ------------------------------------------------------------------------------------------
// Name: FindIfMiddleNamePresent
// Abstract: Given a string that includes "RecordID, Full Name, Street, City, State, ZipCode,"
// find if the Full Name includes a middle name
// ------------------------------------------------------------------------------------------
bool FindIfMiddleNamePresent(char strSource[])
{
	int intIndex = 0;
	int intCommaCount = 0;
	int intSpaceCount = 0;

	char chrCurrentCharacter = ' ';
	char chrPreviousCharacter = ' ';

	bool blnMiddleNamePresent = false;

	while (strSource[intIndex] != '\0' && intCommaCount < 2)
	{
		if (strSource[intIndex] == ',')
		{
			intCommaCount++;
		}
		// Once past the RecordID, check the number of spaces between words.
		// If there are 3 nonconsecutive spaces after the first comma, there
		// is a middle name
		if (intCommaCount > 0)
		{
			chrCurrentCharacter = strSource[intIndex];
			chrPreviousCharacter = strSource[intIndex - 1];
			if (chrCurrentCharacter == ' ' && chrPreviousCharacter != ' ')
			{
				intSpaceCount++;
			}
			if (intSpaceCount == 3)
			{
				blnMiddleNamePresent = true;
			}
		}
		intIndex++;
	}
	return blnMiddleNamePresent;
}



// ------------------------------------------------------------------------------------------
// Name: ParseAddresses
// Abstract: Load each address from the file into audtAddresses. main() passes the current line
// with an address, the audtAddresses[], and the index for which of the audtAddresses[] to
// add the data to
// ------------------------------------------------------------------------------------------
void ParseAddresses(char strSource[], udtAddressType audtAddresses[], int intIndex)
{
	bool blnHasMiddleName = false;
	int intRet = 0;
	long lngRecordID = 0;

	blnHasMiddleName = FindIfMiddleNamePresent(strSource);

	if (blnHasMiddleName == true)
	{
		// %[^,], lets us avoid putting commas into the relevant field
		intRet = sscanf(strSource, "%ld, %s %s %[^,], %[^,], %[^,], %[^,], %s",
			&lngRecordID,
			audtAddresses[intIndex].strFirstName,
			audtAddresses[intIndex].strMiddleName,
			audtAddresses[intIndex].strLastName,
			audtAddresses[intIndex].strStreet,
			audtAddresses[intIndex].strCity,
			audtAddresses[intIndex].strState,
			audtAddresses[intIndex].strZipCode);
	}
	else
	{
		intRet = sscanf(strSource, "%ld, %s %[^,], %[^,], %[^,], %[^,], %s",
			&lngRecordID,
			audtAddresses[intIndex].strFirstName,
			audtAddresses[intIndex].strLastName,
			audtAddresses[intIndex].strStreet,
			audtAddresses[intIndex].strCity,
			audtAddresses[intIndex].strState,
			audtAddresses[intIndex].strZipCode);

		strcpy(audtAddresses[intIndex].strMiddleName, " ");
	}

	audtAddresses[intIndex].lngRecordID = lngRecordID;
}



// ------------------------------------------------------------------------------------------
// Name: OutputAddresses
// Abstract: Output the profile information, stopping when there are no more audtAddresses with
// data in them, as determined by intFilledStructSize.
// ------------------------------------------------------------------------------------------
void OutputAddresses(udtAddressType audtAddresses[], int intFilledStructSize)
{
	int intIndex = 0;
	bool blnHasMiddleName = false;
	while (intIndex < intFilledStructSize)
	{
		printf("------------------------------------------------------------\n");
		printf("Customer #%ld\n", audtAddresses[intIndex].lngRecordID);
		printf("First Name: %s\n", audtAddresses[intIndex].strFirstName);
		// Check if there is a middle name before attempting to print one
		if (audtAddresses[intIndex].strMiddleName[0] > 64 && audtAddresses[intIndex].strMiddleName[0] < 91)
		{
			printf("Middle Name: %s\n", audtAddresses[intIndex].strMiddleName);
		}
		printf("Last Name: %s\n", audtAddresses[intIndex].strLastName);
		printf("Address: %s\n", audtAddresses[intIndex].strStreet);
		printf("City: %s\n", audtAddresses[intIndex].strCity);
		printf("State: %s\n", audtAddresses[intIndex].strState);
		printf("Zip Code: %s\n", audtAddresses[intIndex].strZipCode);
		printf("\n");
		intIndex++;
	}	
}