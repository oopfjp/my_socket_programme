#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    long fileSize = 300 * 1024 * 1024; // 300MB
    long bytesWritten = 0;
    char buffer[1024] = {0}; // 初始化缓冲区，并使用可打印字符填充

    // 使用可打印字符填充缓冲区，例如空格
    for (int i = 0; i < sizeof(buffer); i++) {
      buffer[i] = ' ';
    }

    fp = fopen("large_file.txt", "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    while (bytesWritten < fileSize) {
        long remainingBytes = fileSize - bytesWritten;
        long bytesToWrite = (remainingBytes < sizeof(buffer)) ? remainingBytes : sizeof(buffer);

        size_t written = fwrite(buffer, 1, bytesToWrite, fp);
        if (written != bytesToWrite) {
            perror("Error writing to file");
            fclose(fp);
            return 1;
        }
        bytesWritten += written;
    }

    fclose(fp);
    printf("File created successfully.\n");
    return 0;
}