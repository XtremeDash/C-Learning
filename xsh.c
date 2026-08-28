#include "heads.h"
#include "builtins.h"

int pos;
char prompt[100];

void print_prompt()
{
    char cwd[1024];

    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("┌──(xsh@xsh)-[%s]\n", cwd);
        printf("└─$ ");
    }
}

void handle_sigint(int sig)
{
    (void)sig;

    printf("\n");
    print_prompt();
    fflush(stdout);
}


int exec_command(char *args[])
{
    pid_t pid = fork();
            
    if(pid < 0)
    {
        perror("fork");
        return 1;
    }
    if(pid == 0)
    {
        signal(SIGINT, SIG_DFL);

        execvp(args[0], args);
        perror("\nexecvp");
        _exit(127);
    }
    else
    {
        waitpid(pid, NULL, 0);
        return 0;
    }
}

int main()
{
    signal(SIGINT, handle_sigint);

    char *home = getenv("HOME");

    if(home != NULL)
    {
        chdir(home);
    }

    printf("The X-Shell Developed by Avinash Adithya.\n");

    while(1)
    {
        print_prompt();
        read_line(prompt, sizeof(prompt));

        char *args[MAX_ARGS];

        int argc = parse(prompt, args);

        if (argc == 0)
            continue;

        if (strcmp(args[0], "mkdir") == 0)
        {
            if (args[1] == NULL)
            {
                printf("xsh: mkdir: missing operand\n");
            }
            else if (mkdir(args[1], 0755) != 0)
            {
                perror("xsh: mkdir");
            }

            continue;
        }

        if(strcmp(args[0], "exit") == 0)
        {
            printf("Exiting xsh v0.1\n");
            break;
        }
        if (strcmp(args[0], "clscrn") == 0)
        {
            printf("\033[2J\033[H");
            continue;
        }
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL || strcmp(args[1], "~") == 0)
            {
                if (home != NULL)
                {
                    if (chdir(home) != 0)
                    perror("xsh: cd");
                }
            }
            else
            {
                if (chdir(args[1]) != 0)
                perror("xsh: cd");
            }

            continue;
        }
        if(strcmp(args[0], "help") == 0)
        {
            printf("Welcome to The X - Shell\n");
            printf("- Arguments and commands must have spaces between them\n");
            printf("This is inspired by Kali Linux and other Linux related content\n");
        }
        /*if(strcmp(args[0], "clear") == 0)
        {
            clear() * 50;
        } */
        // uncomment the above code if needed

        exec_command(args);
    }
}