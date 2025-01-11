#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define FILENAME "users.txt"
#define MAX_USERS 100

typedef struct User
{
    char fullName[100];
    char referralCode[10];
    int id;
    int pin;
    char referredBy[10];
    int discount;
    struct User *left;
    struct User *right;
} User;

void homepageADMIN();
int viewtree();
void buildTree(User *users[], int userCount, User **root);
void addChild(User *parent, User *child);
User *createUser(const char *line);

/////////////////////////////////////////////////////////////////
User *createUser(const char *line)
{
    User *user = (User *)malloc(sizeof(User));

    sscanf(line, "%[^,],%[^,],%d,%d,%[^,],%d",
           user->fullName, user->referralCode, &user->id, &user->pin,
           user->referredBy, &user->discount);
    user->left = NULL;
    user->right = NULL;
    return user;
}
/////////////////////////////////////////////////////////////////
void addChild(User *parent, User *child)
{
    if (!parent->left)
    {
        parent->left = child;
    }
    else if (!parent->right)
    {
        parent->right = child;
    }
    else
    {
        fprintf(stderr, "Error: Node %s already has two children.\n", parent->referralCode);
    }
}
/////////////////////////////////////////////////////////////////
void buildTree(User *users[], int userCount, User **root)
{
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i]->referredBy, "ROOT") == 0)
        {
            *root = users[i];
        }
        for (int j = 0; j < userCount; j++)
        {
            if (strcmp(users[j]->referredBy, users[i]->referralCode) == 0)
            {
                addChild(users[i], users[j]);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////
void printTree(User *root, int level)
{
    Sleep(250);
    if (!root)
        return;
    for (int i = 0; i < level; i++)
    {
        printf("\t");
    }
    printf("%s/%s\n", root->referralCode, root->fullName);
    printTree(root->left, level + 1);
    printTree(root->right, level + 1);
}
/////////////////////////////////////////////////////////////////
int viewtree()
{
    FILE *file = fopen(FILENAME, "r");

    User *users[MAX_USERS];
    int userCount = 0;
    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        User *newUser = createUser(line);
        if (strcmp(newUser->fullName, "admin") != 0)
        {
            users[userCount++] = newUser;
        }
        else
        {
            free(newUser);
        }
    }
    fclose(file);

    User *root = NULL;
    buildTree(users, userCount, &root);

    if (!root)
    {
        printf("No root node found. Check the data for a 'ROOT' referral.\n");
        return EXIT_FAILURE;
    }

    printf("\nReferral Tree:\n");
    printTree(root, 0);
    printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------------------------------+\n");
    printf(" +-------+---------+\t -> Enter any key to get back to menu <- \t|\n\t +--------------------------------------------------------------+\n");
    getch();
    for (int i = 0; i < userCount; i++)
    {
        free(users[i]);
    }

    return EXIT_SUCCESS;
}
/////////////////////////////////////////////////////////////////
void homepageADMIN()
{
    int choice;

    do
    {
        Sleep(950);
        system("cls");
        printf("Welcome to the Home Page, ADMIN!\n\n");
        printf("1. View accounts\n");
        printf("2. Log-out\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            viewtree();
            break;
        case 2:
            printf("Logging out, Admin");
            Sleep(800);
            printf("\r                     ");
            printf("\n\nLogged out!");
            Sleep(500);
            break;
        default:
            printf("\n\n +-----------------+\n | POP-UP MESSAGE! |------------------------------------+\n");
            printf(" +-------+---------+\t\tInvalid choice.  \t|\n\t |\t\t-> Enter any key to continue <-\t|\n\t +----------------------------------------------+\n");
            getch();
        }
    } while (choice != 2);
}
/////////////////////////////////////////////////////////////////
