#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define COMMENT ';'
#define METKA '-'

void parse(char *buf);
void printResult(char *str, char *str1, char *str2, char *comment);
bool checkForOperator(char *str);
void addFieldClearBuffer(char *field, char *buffer , int len);
void check(char *buf, char *metka, char *operator, char *operand, char *comment);

int main(void) {
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    FILE *fp = fopen("../test.txt", "r");
    if (!fp) {
        printf("Error opening file '%s'\n", "test.txt");
        return 1;
    }

    while (getline(&line_buf, &line_buf_size, fp) > 0) {
        char firstChar = line_buf[0];
        if (firstChar == COMMENT) {
            printf("%s", "Метка: Оператор: Операнд: Комментарий: ");
            printf("%s", ++line_buf);
            getline(&line_buf, &line_buf_size, fp);
            continue;
        }
        parse(line_buf);
    }
    line_buf = NULL;
    free(line_buf);
    fclose(fp);
    return EXIT_SUCCESS;
}

void parse(char *buf) {
    const ulong len = strlen(buf);
    int i = 0;
    char *metka = (char *) calloc( sizeof(char),len);
    char *tmpBuf = (char*) calloc ( sizeof(char),len);
    char *operator = (char*) calloc( sizeof(char),len);
    char *operand = (char*) calloc( sizeof(char),len);
    char *comment = (char*) calloc( sizeof(char),len);

    int count = 0;
    bool flag = false;
    for (i; i < len; i++) {
        if(buf[i] != ' ' && buf[i] != '\t' && buf[i] != COMMENT) {
            tmpBuf[count] = buf[i];
            ++count;
        }
        if(buf[i] == METKA) {
            addFieldClearBuffer(metka,tmpBuf,len);
            count = 0;
        }
        else if(checkForOperator(tmpBuf)){
            addFieldClearBuffer(operator,tmpBuf,len);
            count = 0;
            flag = true;
        }
        else if(flag == true){
            while( buf[i] != '\n' && buf[i] != '\r' && buf[i] != COMMENT) {
                if(buf[i] != ' ') {
                    tmpBuf[count] = buf[i];
                    ++count;
                }
                ++i;
            }
            addFieldClearBuffer(operand,tmpBuf,len);
            count = 0;
            flag = false;
        }
        if(buf[i] == COMMENT){
            while( buf[i] != '\n') {
                tmpBuf[count] = buf[i];
                ++count;
                ++i;
            }
            addFieldClearBuffer(comment,tmpBuf,len);
        }
        if (buf[i] == '\n') break;
    }
    printResult(metka, operator, operand, comment);
    free(metka);
    free(operator);
    free(operand);
    free(comment);
}

void addFieldClearBuffer(char *field, char *buffer , int len){
    memcpy(field, buffer, len * sizeof(char));
    for(int i = 0; i < len; ++i)
        buffer[i] = *"";
}
bool checkForOperator(char *str) {
    if (strcmp(str, "BEGIN") == 0 || strcmp(str, "END") == 0 ||
        strcmp(str, "DB") == 0 || strcmp(str, "DW") == 0 ||
        strcmp(str, "RB") == 0 ||  strcmp(str, "RW" ) == 0)
        return true;

    return false;
}

void printResult(char *metka, char *operator, char *operand,char *comment) {
    printf("%s", "Метка: ");
    printf("%s", metka);
    printf("%s", " Оператор: ");
    printf("%s", operator);
    printf("%s", " Операнд: ");
    printf("%s", operand);
    printf("%s", " Комментарий: ");
    printf("%s", comment);
    printf("\n");
}