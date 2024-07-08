#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

long int lengthOfFile;
long int numOfNewLines = 0;
const int LENGTHOFENCODINGS = 8;

char* getDataFromFile(char* path, int limit);

int* getIntegerEncodedValuesFromData(char* data);

char* convertIntegerToBinaryString(int num);

char** getBinaryValuesFromIntgerEncodedData(int* dataEncodings);

void writeBinaryDataToFile(char** data, char* path, int limit);

void writeHexDataToFile(int* data, char* path, int limit);

void writeTextDataToFile(char* data, char* path, int limit);

char* getDataFromIntegerEncodedValues(int* dataEncodings);

void handleFunctionCallsBasedOnArgs(char** args, int argc);

void freeBinaryPointers(char** binaryPointers);

int main(int argc, char** argv){
    if (strcmp(argv[1], "-h") == 0){
        printf("Modes:\n -tb -> text to binary \n -th -> text to hexadecimal \n -bh -> binary to hexadecimal \n -hb -> hexadecimal to binary \n -bt -> binary to text \n -ht -> hexadecimal to text \n");
        printf("Correct usage: xxd <inFile> -<mode> <outFile> -<limit in bytes (optional)>");
        return 0;
    }
    if (strcmp(argv[1], "-v") == 0){
        printf("xxdForWindows Version: 1.0.1");
        return 0;
    }
    if (argc < 4){
        printf("Correct usage: xxd <inFile> -<mode> <outFile> -<limit in bytes (optional)>");
        return 0;
    }
    handleFunctionCallsBasedOnArgs(argv, argc);
    return 0;
}

char* getDataFromFile(char* path, int limit){
    FILE* filePtr;
    filePtr = fopen(path, "r");
    if (filePtr == NULL){
        printf("Could not open file to read data.");
        exit(1); // Exit program if file opening fails
    }
    fseek(filePtr, 0, SEEK_END);
    long int length =  ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);
    lengthOfFile = (limit) ? limit : length;
    char* dataPtr = (char*)malloc(lengthOfFile);
    if (dataPtr == NULL){
        printf("Memory allocation failed.");
        fclose(filePtr);
        exit(1); // Exit program if memory allocation fails
    }
    fread(dataPtr, lengthOfFile, 1, filePtr);
    for (int i = 0; i < lengthOfFile; i++){
        if (dataPtr[i] == '\n'){
            numOfNewLines++;
        }
    }
    fclose(filePtr);
    return dataPtr;
}

int* getIntegerEncodedValuesFromData(char* data){
    int* encodedValuesPtr = (int*)malloc(lengthOfFile * sizeof(int));
    if (encodedValuesPtr == NULL){
        printf("Memory allocation failed.");
        exit(1); // Exit program if memory allocation fails
    }
    for (int i = 0; i < lengthOfFile; i++){
        encodedValuesPtr[i] = (int)data[i];
    }
    return encodedValuesPtr;
}

char* convertIntegerToBinaryString(int num){
    int length = LENGTHOFENCODINGS + 1; 
    char* binaryValuePtr = (char*)malloc(length * sizeof(char));
    if (binaryValuePtr == NULL){
        printf("Memory allocation failed.");
        exit(1); // Exit program if memory allocation fails
    }
    for (int i = 0; i < length - 1; i++){
        binaryValuePtr[i] = '0';
    }
    binaryValuePtr[length - 1] = '\0';
    int counter = length - 2;
    while (num > 0 && counter > 0){
        char bitValue = '0';
        if (num % 2 == 1){
            bitValue = '1';
        }
        binaryValuePtr[counter] = bitValue;
        num = floor(num / 2);
        counter --;
    }
    return binaryValuePtr; 
}

char** getBinaryValuesFromIntgerEncodedData(int* dataEncodings){
    int length = lengthOfFile;
    char** binaryPtr = (char**)malloc(lengthOfFile * sizeof(char*));
    if (binaryPtr == NULL){
        printf("Memory allocation failed.");
        exit(1); // Exit program if memory allocation fails
    }
    for (int i = 0; i < length; i++){
        binaryPtr[i] = convertIntegerToBinaryString(dataEncodings[i]);
    }
    return binaryPtr;
}

void writeBinaryDataToFile(char** data, char* path, int limit){
    FILE* filePtr;
    filePtr = fopen(path, "w");
    if (filePtr == NULL){
        printf("Could not open file to write data.");
        exit(1); // Exit program if file opening fails
    }
    int length = (limit) ? limit : lengthOfFile;
    for (int i = 0; i < length; i++){
        fprintf(filePtr, "%s ", data[i]);
    }
    fclose(filePtr);
    return;
}

void writeHexDataToFile(int* data, char* path, int limit){
    FILE* filePtr;
    filePtr = fopen(path, "w");
    if (filePtr == NULL){
        printf("Could not open file to write data.");
        exit(1); // Exit program if file opening fails
    }
    int length = (limit) ? limit : lengthOfFile;
    for (int i = 0; i < length; i++){
        fprintf(filePtr, "%02X ", data[i]);
    }
    fclose(filePtr);
    return;
}

void writeTextDataToFile(char* data, char* path, int limit){
    FILE* filePtr;
    filePtr = fopen(path, "w");
    if (filePtr == NULL){
        printf("Could not open file to write data.");
        exit(1); // Exit program if file opening fails
    }
    int length = (limit) ? limit : lengthOfFile;
    for (int i = 0; i < length; i++){
        fprintf(filePtr, "%c", data[i]);
    }
    fclose(filePtr);
    return;
}

char* getDataFromIntegerEncodedValues(int* dataEncodings){
    char* data = (char*)malloc(lengthOfFile * sizeof(char));
    if (data == NULL){
        printf("Memory allocation failed.");
        exit(1); // Exit program if memory allocation fails
    }
    for (int i = 0; i < lengthOfFile; i++){
        data[i] = (char) dataEncodings[i];
    }
    return data;
}

void freeBinaryPointers(char** binaryPointers){
    for (int i = 0; i < lengthOfFile; i++){
        free(binaryPointers[i]);
    }
    free(binaryPointers);
}

void handleFunctionCallsBasedOnArgs(char** args, int argc){
    char* inFilePath = args[1];
    char* mode = args[2];
    char* outFilePath = args[3];
    int limit = 0;

    if (argc > 4 && strcmp(args[4], "-l") == 0){
        limit = atoi(args[5]);
    }

    if (strcmp(mode, "-tb") == 0){
        char* dataPtr = getDataFromFile(inFilePath, limit);
        int* encodedDataPtr = getIntegerEncodedValuesFromData(dataPtr);
        char** binaryDataPtr = getBinaryValuesFromIntgerEncodedData(encodedDataPtr);
        writeBinaryDataToFile(binaryDataPtr, outFilePath, 0);
        free(dataPtr);
        free(encodedDataPtr);
        freeBinaryPointers(binaryDataPtr);
        return;
    }
    else if (strcmp(mode, "-th") == 0){
        char* dataPtr = getDataFromFile(inFilePath, limit);
        int* encodedDataPtr = getIntegerEncodedValuesFromData(dataPtr);
        writeHexDataToFile(encodedDataPtr, outFilePath, 0);
        free(dataPtr);
        free(encodedDataPtr);
        return;
    }
    else if (strcmp(mode, "-bh") == 0){
        char* dataPtr = getDataFromFile(inFilePath, limit);
        char* delimiter = " ";
        int* binaryValues = (int*)malloc(strlen(dataPtr) / LENGTHOFENCODINGS * sizeof(int));
        if (binaryValues == NULL){
            printf("Memory allocation failed.");
            exit(1); // Exit program if memory allocation fails
        }
        char* binaryValue = strtok(dataPtr, delimiter);
        int index = 0;
        while (binaryValue != NULL){
            binaryValues[index] = strtol(binaryValue, NULL, 2);
            index++;
            binaryValue = strtok(NULL, delimiter);
        }
        writeHexDataToFile(binaryValues, outFilePath, index - numOfNewLines);
        free(dataPtr);
        free(binaryValue);
        free(binaryValues);
        return;
    }
    else if (strcmp(mode, "-hb") == 0){
        char* dataPtr = getDataFromFile(inFilePath, limit);
        char* delimiter = " ";
        char* hexValue = strtok(dataPtr, delimiter);
        int* hexValues = (int*)malloc(strlen(dataPtr) / LENGTHOFENCODINGS * sizeof(int));
        if (hexValues == NULL){
            printf("Memory allocation failed.");
            exit(1); // Exit program if memory allocation fails
        }
        int index = 0;
        while (hexValue != NULL){
            hexValues[index] = strtol(hexValue, NULL, 16);
            index++;
            hexValue = strtok(NULL, delimiter);
        }
        char** binaryValues = getBinaryValuesFromIntgerEncodedData(hexValues);
        writeBinaryDataToFile(binaryValues, outFilePath, index - numOfNewLines);
        free(dataPtr);
        free(hexValue);
        free(hexValues);
        freeBinaryPointers(binaryValues);
        return;
    }
    else if (strcmp(mode, "-bt") == 0){
        char* dataPtr = getDataFromFile(inFilePath, limit);
        char* delimiter = " ";
        char* binValue = strtok(dataPtr, delimiter);
        int* intValues = (int*)malloc(strlen(dataPtr) / LENGTHOFENCODINGS * sizeof(int));
        if (intValues == NULL){
            printf("Memory allocation failed.");
            exit(1); // Exit program if memory allocation fails
        }
        int index = 0;
        while (binValue != NULL){
            intValues[index] = strtol(binValue, NULL, 2);
            index++;
            binValue = strtok(NULL, delimiter);
        }
        char* data = getDataFromIntegerEncodedValues(intValues);
        writeTextDataToFile(data, outFilePath, index - numOfNewLines);
        free(dataPtr);
        free(binValue);
        free(intValues);
        free(data);
        return;
    }
    else if (strcmp(mode, "-ht") == 0){
        char* dataPtr = getDataFromFile(inFilePath, limit);
        char* delimiter = " ";
        char* hexValue = strtok(dataPtr, delimiter);
        int* intValues = (int*)malloc(strlen(dataPtr) / LENGTHOFENCODINGS * sizeof(int));
        if (intValues == NULL){
            printf("Memory allocation failed.");
            exit(1); // Exit program if memory allocation fails
        }
        int index = 0;
        while (hexValue != NULL){
            intValues[index] = strtol(hexValue, NULL, 16);
            index++;
            hexValue = strtok(NULL, delimiter);
        }
        char* data = getDataFromIntegerEncodedValues(intValues);
        writeTextDataToFile(data, outFilePath, index - numOfNewLines);
        free(dataPtr);
        free(hexValue);
        free(intValues);
        free(data);
        return;
    }
    else {
        printf("Invalid mode.");
        return;
    }
}