#include <stdio.h>
#include <string.h>

void textMatrixCreate(char* str, char mat[][10]) {
    char temp[10];
    for (int i = 0; i < 10; i++) {
        strncpy(temp, str + i*10, 10);
        for (int j = 0; j < 10; j++) {
            mat[i][j] = temp[j];
        }
        // printf("%s\n", temp);
    }
}

void textMatrixTranzpoze(char dest[][10], char mat[][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            dest[i][j] = mat[j][i];
        }
    }
}

void textMatrixCoder(char* destStr, char mat[][10]) {
    char temp[11];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            temp[j] = mat[i][j];
        }
        strcat(destStr, temp);
    }
}

void textMatrixDecoder(char* originalStr, char* codedStr) {
    char matrix[10][10];
    textMatrixCreate(codedStr, matrix);
    char decodedMatrix[10][10];
    textMatrixTranzpoze(decodedMatrix, matrix);
    textMatrixCoder(originalStr, decodedMatrix);
}

int main() {
    char str[101] = "Erciyes Universitesi, Kayserinin bilim ve gelecek icin gercekten cok ama cok onemli bir okuludur...";
    char mat[10][10];
    textMatrixCreate(str, mat);
    char codedMat[10][10];
    textMatrixTranzpoze(codedMat, mat);
    char codedStr[100];
    textMatrixCoder(codedStr, codedMat);
    printf("Sifreli Metin: %s\n", codedStr);
    char decodedStr[100];
    textMatrixDecoder(decodedStr, codedStr);
    printf("Sifresiz Metin: %s\n\n\n", decodedStr);
    return 0;

}