#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

int getUserId()
{
    FILE *fp;
    struct User userChecker;
    int maxId = -1;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        return 0; // First user will have ID 0
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (userChecker.id > maxId)
        {
            maxId = userChecker.id;
        }
    }

    fclose(fp);
    return maxId + 1; // Return next available ID
}

int isUsernameTaken(char username[50])
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        return 0; // No users yet, so username is not taken
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, username) == 0)
        {
            fclose(fp);
            return 1; // Username is taken
        }
    }

    fclose(fp);
    return 0; // Username is not taken
}

void registerMenu(char name[50], char password[50])
{
    struct termios oflags, nflags;
    FILE *fp;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Registration:");

    // Get username
    while (1) {
        printf("\n\n\t\t\t\tEnter username: ");
        scanf("%s", name);

        // Check if username is already taken
        if (isUsernameTaken(name)) {
            printf("\n\t\t\t\tUsername already taken. Please choose another.");
        } else {
            break;
        }
    }

    // Get password with hidden input
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    printf("\n\n\t\t\t\tEnter password: ");
    scanf("%s", password);

    // Restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    // Save user to file
    int userId = getUserId();

    if ((fp = fopen("./data/users.txt", "a")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    fprintf(fp, "%d %s %s\n", userId, name, password);
    fclose(fp);

    printf("\n\n\t\t\t\tRegistration successful!");
    printf("\n\n\t\t\t\tYour user ID is: %d", userId);
    printf("\n\n\t\t\t\tPress any key to continue...");
    getchar(); // Consume newline
    getchar(); // Wait for keypress
}