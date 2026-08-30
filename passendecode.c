#include <stdio.h>
#include <string.h>

void encode(char pass[])
{
    for(int i = 0; pass[i] != '\0'; i++)
    {
        printf("%02X ", (unsigned char)pass[i]);
    }
    printf("\n");
}

void decode(char encoded[])
{
    char *token = strtok(encoded, " ");
    
    while(token != NULL)
    {
        unsigned int value;

        sscanf(token, "%2X", &value);
        printf("%c", (char)value);

        token = strtok(NULL, " ");
    }
    printf("\n");
}

int main()
{
    printf("ENCODER/DECODER v2. Type 'X' to quit. 'encode' to encode. 'decode' to decode\n");

    char pass[100];
    char encoded[300];
    while(1)
    {
        printf("MAIN STARTED>");
        fgets(pass, sizeof(pass), stdin);
        pass[strcspn(pass, "\n")] = '\0';

        if(strcmp(pass, "X") == 0)
        {
            break;
        }
        else if(strcmp(pass, "encode") == 0)
        {
            printf("ENTER TEXT> ");
            fgets(pass, sizeof(pass), stdin);
            pass[strcspn(pass, "\n")] = '\0';

            encode(pass);
        }
        else if(strcmp(pass, "decode") == 0)
        {
            printf("ENTER TEXT> ");
            fgets(encoded, sizeof(encoded), stdin);
            encoded[strcspn(encoded, "\n")] = '\0';

            decode(encoded);
        }
        else if(strcmp(pass, "") == 0)
        {
            continue;
        }
        else
        {
            printf("Unknown Command\n");
        }
    }
    printf("\n");
    return 0;
}