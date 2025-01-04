
#include <ctype.h>   // For tolower
#include <conio.h>   // For getch
#include <windows.h> // For Sleep
#include "nodesNprototypes.h"
#include "admin.h"

/////////////////////////////////////////////////////////////////
void homepage(const char *referral)
{
    int choice;
    char userReferral[10] = "";
    FILE *file = fopen(FILENAME, "r");
    char fullName[100];

    char line[200];
    while (fgets(line, sizeof(line), file))
    {
        fullName[0] = '\0';
        userReferral[0] = '\0';
        sscanf(line, "%[^,],%[^,]", fullName, userReferral);
        if (strcmp(userReferral, referral) == 0)
        {
            break;
        }
    }
    fclose(file);

    do
    {
        Sleep(950);
        system("cls");
        printf("Welcome to the Home Page, %s!\n\n", fullName);
        printf("1. View Account Details\n");
        printf("2. Who uses my referral\n");
        printf("3. Unlink who uses my referral\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            animation(1);
            discount("update");
            viewAccount(referral);
            discount("reset");
            break;
        case 2:
            animation(3);
            discount("update");
            whoUsesMyReferral(userReferral);
            discount("reset");
            break;
        case 3:
            animation(3);
            discount("update");
            unlinkReferral(userReferral);
            discount("reset");
            break;
        case 4:
            animation(2);
            break;
        default:
            printf("\n\n +-----------------+\n | POP-UP MESSAGE! |--------------------------------------------+\n");
            printf(" +-------+---------+\t Invalid choice. Please try again. \t|\n\t +------------------------------------------------------+\n");
            Sleep(1000);
        }
    } while (choice != 4);
}
/////////////////////////////////////////////////////////////////
void viewAccount(const char *referral)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file)
    {
        printf("Error opening file!\n");
    }
    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), file))
    {
        char fullName[100], userReferral[10], userReferralFrom[10];
        int id, pin, discount;
        sscanf(line, "%[^,],%[^,],%d,%d,%[^,],%d", fullName, userReferral, &id, &pin, userReferralFrom, &discount);
        if (strcmp(userReferral, referral) == 0)
        {
            printf("\n\n--- Account Details ---\n");
            printf("Full Name: %s\n", fullName);
            printf("Referral Code: %s\n", userReferral);
            printf("ID: %d\n", id);
            printf("User Discount: %d Percent\n", discount);

            printf("-----------------------\n");
            found = 1;
        }
    }
    fclose(file);
    if (!found)
    {
        printf("Account details not found!\n");
    }
    printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------------------------------+\n");
    printf(" +-------+---------+\t -> Enter any key to get back to menu <- \t|\n\t +--------------------------------------------------------------+\n");
    getch();
}
/////////////////////////////////////////////////////////////////
void whoUsesMyReferral(const char *referralCode)
{
    FILE *file = fopen(FILENAME, "r");

    char line[200];
    int found = 0;

    printf("\n--- Users Who Used Referral Code: %s ---\n", referralCode);

    while (fgets(line, sizeof(line), file))
    {
        char fullName[100], userReferral[10], userReferralFrom[10];
        int id, pin, discount;

        sscanf(line, "%[^,],%[^,],%d,%d,%[^,],%d",
               fullName, userReferral, &id, &pin, userReferralFrom, &discount);

        if (strcmp(userReferralFrom, referralCode) == 0)
        {

            printf("Full Name: %s\n", fullName);
            Sleep(350);
            printf("Referral Code: %s\n", userReferral);
            Sleep(250);
            printf("ID: %d\n", id);
            Sleep(150);
            printf("User Discount: %d%%\n", discount);
            printf("----------------------------\n");
            Sleep(50);

            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------------------------------+\n");
        printf(" +-------+---------+\t   No users have used your referral code.\t|\n\t |\t\t  -> Enter any key to get back to menu <- \t|\n\t +--------------------------------------------------------------+\n", "123456");
    }
    else
    {
        printf("\n\n +-----------------+\n | POP-UP MESSAGE! |--------------------------------------------+\n");
        printf(" +-------+---------+\t     Operation Complete\t\t\t|\n\t |\t -> Enter any key to get back to menu <- \t|\n\t +------------------------------------------------------+\n", "123456");
    }
    getch();
}
/////////////////////////////////////////////////////////////////
void unlinkReferral(const char *referralCode)
{
    FILE *file = fopen(FILENAME, "r");

    char line[200];
    int found = 0;
    int childCount = 0;
    char leftChildReferral[20] = "";
    char rightChildReferral[20] = "";

    printf("\n\n ----[ Account who are in with you ]----\n");

    while (fgets(line, sizeof(line), file))
    {
        char fullName[100], userReferral[10], userReferralFrom[10];
        int id, pin, discount;

        sscanf(line, "%[^,],%[^,],%d,%d,%[^,],%d",
               fullName, userReferral, &id, &pin, userReferralFrom, &discount);

        if (strcmp(userReferralFrom, referralCode) == 0)
        {
            childCount++;
            found = 1;

            if (childCount == 1)
            {
                printf("Left Account: [ %s ] (Referral: -> %s <-)\n", fullName, userReferral);
                strcpy(leftChildReferral, userReferral);
            }
            else if (childCount == 2)
            {
                printf("Right Account: [ %s ] (Referral: -> %s <-)\n", fullName, userReferral);
                strcpy(rightChildReferral, userReferral);
            }
            else
            {
                break;
            }
        }
    }
    fclose(file);

    if (!found)
    {
        printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------------------------------+\n");
        printf(" +-------+---------+\t   No users have used your referral code.\t|\n\t |\t\t  -> Enter any key to get back to menu <- \t|\n\t +--------------------------------------------------------------+\n", "123456");
        getch();
        return;
    }

    bool correctOption = false;
    int attempts = 2;
    char selectedReferral[10] = "";

    do
    {

        printf("\nDelete child: left or right? ");

        char choice[10];
        scanf("%s", choice);
        clearBuffer(); 

        if (strcmp(choice, "left") == 0 && strlen(leftChildReferral) > 0)
        {
            strcpy(selectedReferral, leftChildReferral);
            correctOption = true;
        }
        else if (strcmp(choice, "right") == 0 && strlen(rightChildReferral) > 0)
        {
            strcpy(selectedReferral, rightChildReferral);
            correctOption = true;
        }
        attempts--;

        if (attempts == 1 && !correctOption)
        {
            printf("\nInvalid choice or no such node.");
            Sleep(2000);
            printf("\r                                           ");
        }
        else if (attempts == 0 && !correctOption)
        {
            printf("Invalid choice or no such node.\nReturning to menu");
            Sleep(1200);
            printf("\rReturning to menu.");
            Sleep(999);
            printf("\rReturning to menu..");
            Sleep(550);
            printf("\rReturning to menu...");
            Sleep(100);

            return;
        }

    } while (attempts > 0 && !correctOption);

    file = fopen(FILENAME, "r");

    found = 0;
    while (fgets(line, sizeof(line), file))
    {
        char fullName[100], userReferral[10], userReferralFrom[10];
        int id, pin, discount;

        sscanf(line, "%[^,],%[^,],%d,%d,%[^,],%d",
               fullName, userReferral, &id, &pin, userReferralFrom, &discount);

        if (strcmp(userReferralFrom, selectedReferral) == 0)
        {
            printf("\n\n +-----------------+\n | POP-UP MESSAGE! |--------------------------------------------+\n");
            printf(" +-------+---------+\tAccount cannot be unlinked because \t|\n\t |\t\t it has another associated account \t|\n\t |\t      -> Enter any key to get back to menu <- \t|\n\t +------------------------------------------------------+\n");
            fclose(file);
            getch();
            return;
        }
    }
    fclose(file);

    FILE *tempFile = fopen("temp.txt", "w");
    file = fopen(FILENAME, "r");

    if (!tempFile)
    {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        char fullName[100], userReferral[10], userReferralFrom[10];
        int id, pin, discount;

        sscanf(line, "%[^,],%[^,],%d,%d,%[^,],%d",
               fullName, userReferral, &id, &pin, userReferralFrom, &discount);

        if (strcmp(userReferral, selectedReferral) == 0)
        {

            strcpy(userReferralFrom, "-1");
        }

        fprintf(tempFile, "%s,%s,%d,%d,%s,%d\n",
                fullName, userReferral, id, pin, userReferralFrom, discount);
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);
    printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------------------------------+\n");
    printf(" +-------+---------+\t Unlinking operation completed successfully \t|\n\t +--------------------------------------------------------------+\n");
    Sleep(1999);
    referralUsage(referralCode, 2);
}
/////////////////////////////////////////////////////////////////
