#include "heads.h"

int parse(char *input, char *args[])
{
    int argc = 0;

    char *token = strtok(input, " ");

    while (token != NULL && argc < MAX_ARGS - 1) // 
    {
        args[argc++] = token;
        token = strtok(NULL, " ");
    }

    args[argc] = NULL;

    return argc;
}

void clear()
{
    system("clear");
}

void read_line(char *buffer, int size)
{
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int pos = 0;
    int len = 0;

    while (1)
    {
        char c;

        if (read(STDIN_FILENO, &c, 1) != 1)
            continue;

        // Enter
        if (c == '\n')
        {
            buffer[len] = '\0';
            printf("\n");
            break;
        }

        // Backspace
        if (c == 127)
        {
            if (pos > 0)
            {
                for (int i = pos - 1; i < len - 1; i++)
                {
                    buffer[i] = buffer[i + 1];
                }

                pos--;
                len--;

                buffer[len] = '\0';

                printf("\b \b");

                // Redraw the rest of the line
                printf("\033[K");

                fflush(stdout);
            }

            continue;
        }

        // Escape sequence
        if (c == '\033')
        {
            char seq[2];

            if (read(STDIN_FILENO, &seq[0], 1) != 1)
                continue;

            if (seq[0] != '[')
                continue;

            if (read(STDIN_FILENO, &seq[1], 1) != 1)
                continue;

            // Left arrow
            if (seq[1] == 'D')
            {
                if (pos > 0)
                {
                    printf("\033[D");
                    pos--;
                }
            }

            // Right arrow
            else if (seq[1] == 'C')
            {
                if (pos < len)
                {
                    printf("\033[C");
                    pos++;
                }
            }

            fflush(stdout);
            continue;
        }

        // Normal character
        if (c >= 32 && c <= 126)
        {
            if (len < size - 1)
            {
                for (int i = len; i > pos; i--)
                {
                    buffer[i] = buffer[i - 1];
                }

                buffer[pos] = c;

                len++;
                pos++;

                buffer[len] = '\0';

                // Print character and everything after it
                printf("%s", &buffer[pos - 1]);

                // Move cursor back over characters after insertion
                for (int i = pos; i < len; i++)
                {
                    printf("\033[D");
                }

                fflush(stdout);
            }
        }
    }

    // Restore terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}


void exec_pipe(char *left[], char *right[])
{
    int fd[2];
    pid_t pid1, pid2;

    if (pipe(fd) == -1)
    {
        printf(RED);
        perror("pipe");
        printf(RESET);
        return;
    }

    pid1 = fork();

    if (pid1 < 0)
    {
        printf(RED);
        perror("fork");
        return;
        printf(RESET);
    }

    if (pid1 == 0)
    {
        // Child 1: command on the left

        close(fd[0]);

        // stdout -> pipe
        dup2(fd[1], STDOUT_FILENO);

        close(fd[1]);

        execvp(left[0], left);

        printf(RED);
        perror("execvp");
        printf(RESET);
        exit(1);
    }

    pid2 = fork();

    if (pid2 < 0)
    {
        printf(RED);
        perror("fork");
        printf(RESET);
        return;
    }

    if (pid2 == 0)
    {
        // Child 2: command on the right

        close(fd[1]);

        // stdin <- pipe
        dup2(fd[0], STDIN_FILENO);

        close(fd[0]);

        execvp(right[0], right);

        printf(RED);
        perror("execvp");
        printf(RESET);
        exit(1);
    }

    // Parent doesn't need either end
    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}
