#include "test.h"
#include "../src/utils/error.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fail("One argument expected");
    }

    FILE * command = popen(argv[1], "r");
    if(command == NULL)
    {
        fail("Failed to run the command");
    }

    char line[4096];
    while (fgets(line, sizeof(line), command))
    {
        printf("%s", line);
    }
    pclose(command);
    return 0;
}
