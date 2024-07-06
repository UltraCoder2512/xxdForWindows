//* Make sure to update the version!

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

long int lengthOfFile;
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

int main(int argc, char** argv){
    if (strcmp(argv[1], "-h") == 0){
        printf("Modes:\n -tb -> text to binary \n -th -> text to hexadecimal \n -bh -> binary to hexadecimal \n -hb -> hexadecimal to binary \n -bt -> binary to text \n -ht -> hexadecimal to text \n");
        printf("Correct usage: xxd <inFile> -<mode> <outFile> -<limit in bytes (optional)>");
        return 0;
    }
    if (strcmp(argv[1], "-v") == 0){
        printf("xxdForWindows Version: 1.0.0");
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
    }
    fseek(filePtr, 0, SEEK_END);
    long int length =  ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);
    lengthOfFile = (limit) ? limit : length;
    char* dataPtr = (char*)malloc(lengthOfFile);
    fread(dataPtr, lengthOfFile, 1, filePtr);
    return dataPtr;
}

int* getIntegerEncodedValuesFromData(char* data){
    int* encodedValuesPtr = (int*)malloc(lengthOfFile);
    for (int i = 0; i < lengthOfFile; i++){
        encodedValuesPtr[i] = (int)data[i];
    }
    return encodedValuesPtr;
}

char* convertIntegerToBinaryString(int num){
    int length = LENGTHOFENCODINGS + 1; 
    char* binaryValuePtr = (char*)malloc(length);
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
    char** binaryPtr = (char**)malloc(lengthOfFile * LENGTHOFENCODINGS);
    for (int i = 0; i < length; i++){
        binaryPtr[i] = convertIntegerToBinaryString(dataEncodings[i]);
    }
    return binaryPtr;
}

void writeBinaryDataToFile(char** data, char* path, int limit){
    FILE* filePtr;
    filePtr = fopen(path, "w");
    int length = (limit) ? limit : lengthOfFile;
    for (int i = 0; i < length; i++){
        fprintf(filePtr, "%s ", data[i]);
    }
    fclose(filePtr);
}

void writeHexDataToFile(int* data, char* path, int limit){
    FILE* filePtr;
    filePtr = fopen(path, "w");
    int length = (limit) ? limit : lengthOfFile;
    for (int i = 0; i < length; i++){
        fprintf(filePtr, "%02X ", data[i]);
    }
    fclose(filePtr);
}

void writeTextDataToFile(char* data, char* path, int limit){
    FILE* filePtr;
    filePtr = fopen(path, "w");
    int length = (limit) ? limit : lengthOfFile;
    for (int i = 0; i < length; i++){
        fprintf(filePtr, "%c", data[i]);
    }
    fclose(filePtr);
}

char* getDataFromIntegerEncodedValues(int* dataEncodings){
    char* data = (char*)malloc(lengthOfFile * sizeof(char));
    for (int i = 0; i < lengthOfFile; i++){
        data[i] = (char) dataEncodings[i];
    }
    return data;
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
        free(binaryDataPtr);
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
        int* binaryValues = (int*)malloc(strlen(dataPtr) / LENGTHOFENCODINGS);
        char* binaryValue = strtok(dataPtr, delimiter);
        int index = 0;
        while (binaryValue != NULL){
            binaryValues[index] = strtol(binaryValue, NULL, 2);
            index++;
            binaryValue = strtok(NULL, delimiter);
        }
        writeHexDataToFile(binaryValues, outFilePath, index - 4);
        free(dataPtr);
        free(binaryValue);
        free(binaryValues);
        return;
    }
    else if (strcmp(mode, "-hb") == 0){
        char* dataPtr = getDataFromFile(inFilePath, limit);
        char* delimiter = " ";
        char* hexValue = strtok(dataPtr, delimiter);
        int* hexValues = (int*)malloc(strlen(dataPtr) / LENGTHOFENCODINGS);
        int index = 0;
        while (hexValue != NULL){
            hexValues[index] = strtol(hexValue, NULL, 16);
            index++;
            hexValue = strtok(NULL, delimiter);
        }
        char** binaryValues = getBinaryValuesFromIntgerEncodedData(hexValues);
        writeBinaryDataToFile(binaryValues, outFilePath, index - 4);
        free(dataPtr);
        free(hexValue);
        free(hexValue);
        free(binaryValues);
        return;
    }
    else if (strcmp(mode, "-bt") == 0){
        char* dataPtr = getDataFromFile(inFilePath, limit);
        char* delimiter = " ";
        char* binValue = strtok(dataPtr, delimiter);
        int* intValues = (int*)malloc(strlen(dataPtr) / LENGTHOFENCODINGS);
        int index = 0;
        while (binValue != NULL){
            intValues[index] = strtol(binValue, NULL, 2);
            index++;
            binValue = strtok(NULL, delimiter);
        }
        char* data = getDataFromIntegerEncodedValues(intValues);
        writeTextDataToFile(data, outFilePath, index - 4);
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
        int* intValues = (int*)malloc(strlen(dataPtr) / LENGTHOFENCODINGS);
        int index = 0;
        while (hexValue != NULL){
            intValues[index] = strtol(hexValue, NULL, 16);
            index++;
            hexValue = strtok(NULL, delimiter);
        }
        char* data = getDataFromIntegerEncodedValues(intValues);
        writeTextDataToFile(data, outFilePath, index - 4);
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
