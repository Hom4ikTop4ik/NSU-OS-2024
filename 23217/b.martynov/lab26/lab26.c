#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define STR_SIZE 1024
#define BUFET_SIZE 1024

int main(int argc, char** argv)
{
    int child = 0;
    if (argc >= 2) {
        child = 1;
    }

    unsigned char bufet[] = "SASha was WaLkInG on HIGHway and SUCKed BAGel.";

    if (child == 1) {
        
    }
    else {
        char str[STR_SIZE] = { 0 };
        strcat(str, argv[0]);
        strcat(str, " biba boba");

        printf("'%s'\n", str);

        FILE* input = popen(str, "r");

        unsigned char bufet_for_read[BUFET_SIZE] = { 0 };

        ssize_t read_res = read(input, (void*)bufet_for_read, BUFET_SIZE);
        if (read_res == -1) {
            perror("read() unsuccess");
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < BUFET_SIZE; i++) {
            bufet_for_read[i] = toupper((int)(bufet_for_read[i]));
        }

        printf("%s\n", bufet_for_read);

        pclose(input);
    }

    return 0;
}
