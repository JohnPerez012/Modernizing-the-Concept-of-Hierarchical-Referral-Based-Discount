
#include <ctype.h>   // For tolower
#include <windows.h> // For Sleep
#include <conio.h>   // For getch
#include "nodesNprototypes.h"

/////////////////////////////////////////////////////////////////
void checkRootNreferrals()
{
    FILE *file = fopen(FILENAME, "r");
    FILE *fileR = fopen(FILENAMEr, "r");
    char line[200];
    bool rootExists = false;
    bool referralExists = false;

    if (fileR)
    {
        while (fgets(line, sizeof(line), fileR))
        {
            if (strstr(line, ROOT_REFERRAL))
            {
                referralExists = true;
                break;
            }
        }
        fclose(fileR);
    }

    if (!referralExists)
    {
        fileR = fopen(FILENAMEr, "a");
        if (fileR)
        {
            fprintf(fileR, "%s,0\n", ROOT_REFERRAL);
            fclose(fileR);
        }
    }

    if (file)
    {
        while (fgets(line, sizeof(line), file))
        {
            if (strstr(line, "ROOT"))
            {
                rootExists = true;
                break;
            }
        }
        fclose(file);
    }

    if (!rootExists)
    {
        file = fopen(FILENAME, file ? "a" : "w");
        if (file)
        {
            fprintf(file, "admin,0,0,0,0,0\n");
            fprintf(file, "head,%s,1010,100230,ROOT,0\n", ROOT_REFERRAL);
            fclose(file);
        }
    }
}
/////////////////////////////////////////////////////////////////
void menu()
{
    checkRootNreferrals();

    int choice;
    do
    {
        system("cls");
        printf("Welcome User!\n");
        printf("1. Login\n");

        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice)
        {
        case 1:
            loginUser();
            break;
        case 2:
            registerUser();
            break;
        case 3:
            system("cls");
            printf("\n\n\t\tThank you for using the program.\n\n\n");
            Sleep(2000);
            system("cls");
            printf("\n\n\t\tGoodbye!\n\n\n");
            break;
        default:
            printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------+\n +-------+---------+\t ");
            printf("Invalid choice. \t|\n\t |\t\tPlease try again.\t|\n\t +--------------------------------------+\n");
            Sleep(950);
        }
    } while (choice != 3);
}
/////////////////////////////////////////////////////////////////
void clearBuffer()
{
    while (getchar() != '\n')
        ;
}
/////////////////////////////////////////////////////////////////
int isValidReferral(const char *referral)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file)
        return 0;

    char line[200];
    while (fgets(line, sizeof(line), file))
    {
        char tempName[100], tempReferral[10], tempReferral1[10];
        int tempId, tempPin;
        sscanf(line, "%[^,],%[^,],%d,%d", tempName, tempReferral, &tempId, &tempPin, tempReferral1);
        if (strcmp(tempReferral, referral) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}
/////////////////////////////////////////////////////////////////
bool checkReferralUsage(const char *referral)
{
    FILE *file = fopen(FILENAMEr, "r");
    char line[100];
    bool isValid = false;

    while (fgets(line, sizeof(line), file))
    {
        char refCode[100];
        int currentUsage = 0;

        if (sscanf(line, "%[^,],%d", refCode, &currentUsage) == 2)
        {
            if (strcmp(refCode, referral) == 0 && currentUsage < 2)
            {
                isValid = true;
                break;
            }
        }
    }

    fclose(file);
    return isValid;
}
/////////////////////////////////////////////////////////////////
void suggestReferralCodes()
{
    FILE *file = fopen(FILENAMEr, "r");

    char line[100];
    printf("\rHere are some referral codes you can use:\n");

    while (fgets(line, sizeof(line), file))
    {
        char refCode[100];
        int currentUsage = 0;

        if (sscanf(line, "%[^,],%d", refCode, &currentUsage) == 2)
        {
            if (currentUsage < 2)
            {
                printf("->  %s\n", refCode);
            }
        }
    }

    fclose(file);
}
/////////////////////////////////////////////////////////////////
int linkMyAccount(const char *username, const char *refcode)
{
    FILE *file = fopen(FILENAME, "r");

    UserNode users[100];
    int userCount = 0;

    char line[200];
    while (fgets(line, sizeof(line), file))
    {
        UserNode user;
        sscanf(line, "%[^,],%[^,],%d,%d,%[^,],%d",
               user.fullName,
               user.userReferral,
               &user.id,
               &user.pin,
               user.userReferralFrom,
               &user.discount);

        if (strcmp(user.fullName, username) == 0)
        {
            strcpy(user.userReferralFrom, refcode);
            printf("Updated referral for user %s to %s\n", username, refcode);
        }

        users[userCount++] = user;
    }

    fclose(file);

    referralUsage(refcode, 1);

    file = fopen(FILENAME, "w");

    for (int i = 0; i < userCount; i++)
    {
        fprintf(file, "%s,%s,%d,%d,%s,%d\n",
                users[i].fullName,
                users[i].userReferral,
                users[i].id,
                users[i].pin,
                users[i].userReferralFrom,
                users[i].discount);
    }

    fclose(file);
    return 0;
}
/////////////////////////////////////////////////////////////////
bool independentId(int id)
{
    FILE *file = fopen(FILENAME, "r");
    char line[200];
    UserNode user;
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%[^,],%d,%d", user.fullName, user.userReferral, &user.id, &user.pin);
        if (user.id == id)
        {
            return false;
        }
    }
    return true;
}
/////////////////////////////////////////////////////////////////
void generateUniqueReferral(char *newReferral)
{
    FILE *file;
    char line[200];
    int isUnique;

    do
    {
        isUnique = 1;

        sprintf(newReferral, "%06d", (rand() % 900000) + 100000);

        file = fopen(FILENAME, "r");

        while (fgets(line, sizeof(line), file))
        {
            UserNode user;
            sscanf(line, "%[^,],%[^,],%d,%d", user.fullName, user.userReferral, &user.id, &user.pin);

            if (strcmp(newReferral, user.userReferral) == 0)
            {
                isUnique = 0;
                break;
            }
        }

        fclose(file);
    } while (!isUnique);
}
/////////////////////////////////////////////////////////////////
void hideId()
{
    system("cls");
    printf("Welcome User!\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
    printf("Enter your choice: 1");
    printf("\n\nEnter your ID: \n");
}
/////////////////////////////////////////////////////////////////
void discount(const char *value)
{
    if (strcmp(value, "update") == 0)
    {

        FILE *file = fopen(FILENAME, "r");

        UserNode users[100];
        int userCount = 0;

        char line[200];
        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%[^,],%[^,],%d,%d,%[^,],%d",
                   users[userCount].fullName,
                   users[userCount].userReferral,
                   &users[userCount].id,
                   &users[userCount].pin,
                   users[userCount].userReferralFrom,
                   &users[userCount].discount);
            userCount++;
        }
        fclose(file);

        // First rule: ->20% discount to those parent who have two nodes
        for (int i = 0; i < userCount; i++)
        {
            int referralCount = 0;

            for (int j = 0; j < userCount; j++)
            {
                if (strcmp(users[j].userReferralFrom, users[i].userReferral) == 0)
                {
                    referralCount++;
                }
            }

            if (referralCount >= 2)
            {
                users[i].discount = FIRSTRULE;
            }
        }

        // Second rule: ->5% discount for parents of nodes with ->20% discount
        for (int i = 0; i < userCount; i++)
        {
            for (int j = 0; j < userCount; j++)
            {
                if (strcmp(users[j].userReferralFrom, users[i].userReferral) == 0 && users[j].discount % 20 == 0)
                {
                    if (users[j].discount != 0)
                    {
                        users[i].discount += SECONDRULE;
                    }
                }
            }
        }

        // Third rule: Increment parent by ->2% if a child has ->30% discount
        for (int i = 0; i < userCount; i++)
        {
            for (int j = 0; j < userCount; j++)
            {
                if (strcmp(users[j].userReferralFrom, users[i].userReferral) == 0 && users[j].discount % 30 == 0)
                {
                    if (users[j].discount != 0)
                    {
                        users[i].discount += THIRDRULE;
                    }
                }
            }
        }

        file = fopen(FILENAME, "w");

        for (int i = 0; i < userCount; i++)
        {
            fprintf(file, "%s,%s,%d,%d,%s,%d\n",
                    users[i].fullName,
                    users[i].userReferral,
                    users[i].id,
                    users[i].pin,
                    users[i].userReferralFrom,
                    users[i].discount);
        }
        fclose(file);
    }
    else if (strcmp(value, "reset") == 0)
    {
        FILE *file = fopen(FILENAME, "r");

        UserNode users[100];
        int userCount = 0;

        char line[200];
        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%[^,],%[^,],%d,%d,%[^,],%d",
                   users[userCount].fullName,
                   users[userCount].userReferral,
                   &users[userCount].id,
                   &users[userCount].pin,
                   users[userCount].userReferralFrom,
                   &users[userCount].discount);
            userCount++;
        }
        fclose(file);

        for (int i = 0; i < userCount; i++)
        {
            users[i].discount = 0;
        }

        file = fopen(FILENAME, "w");

        for (int i = 0; i < userCount; i++)
        {
            fprintf(file, "%s,%s,%d,%d,%s,%d\n",
                    users[i].fullName,
                    users[i].userReferral,
                    users[i].id,
                    users[i].pin,
                    users[i].userReferralFrom,
                    users[i].discount);
        }
        fclose(file);
    }
}
/////////////////////////////////////////////////////////////////
void animation(int value)
{
    switch (value)
    {
    case 1:
        for (int i = 0; i < 3; i++)
        {
            printf("\rGetting account details. ");
            Sleep(500);

            printf("\rGetting account details  ");
            Sleep(450);

            printf("\rGetting account details. ");
            Sleep(300);

            printf("\rGetting account details.. ");
            Sleep(250);
        }
        printf("\r                                     ");
        break;
    case 2:

        for (int i = 0; i < 2; i++)
        {
            printf("\rLogging out ");
            Sleep(500);

            printf("\rLogging out! ");
            Sleep(450);

            printf("\rLogging out. ");
            Sleep(300);

            printf("\rLogging out ");
            Sleep(250);
        }
        printf("\rLogged out.");
        Sleep(999);
        break;
    case 3:

        for (int i = 0; i <= 3; i++)
        {
            switch (i)
            {
            case 0:
                printf("\rScanning Data Discounts  ");
                Sleep(200);
                printf("\rScanning Data Discounts.  ");
                Sleep(200);
                printf("\rScanning Data Discounts..  ");
                Sleep(200);
                printf("\rScanning Data Discounts...  ");
                Sleep(200);
                break;
            case 1:
                printf("\rGetting Data Discount  ");
                Sleep(200);
                printf("\rGetting Data Discount.  ");
                Sleep(200);
                printf("\rGetting Data Discount..  ");
                Sleep(200);
                printf("\rGetting Data Discount...  ");
                Sleep(200);
                break;
            case 2:
                printf("\rTransferring Data Discount ");
                Sleep(300);
                printf("\rTransferring Data Discount.  ");
                Sleep(300);
                printf("\rTransferring Data Discount..  ");
                Sleep(300);
                printf("\rTransferring Data Discount...  ");
                Sleep(300);
                break;

            case 3:
                printf("\rDisplaying account details ");
                Sleep(250);
                printf("\rDisplaying account details.  ");
                Sleep(250);
                printf("\rDisplaying account details.. ");
                Sleep(250);
                printf("\rDisplaying account details... ");
                Sleep(250);
                break;
            }
            printf("\r\r                                                ");
        }

        break;
    default:
        break;
    }
}
/////////////////////////////////////////////////////////////////
void referralUsage(const char *referralCode, int order)
{
    if (order == 1)
    {
        FILE *file = fopen(FILENAMEr, "r");

        FILE *tempFile = fopen("temp_usage.txt", "w");

        char line[100];
        int found = 0;

        while (fgets(line, sizeof(line), file))
        {
            char code[20];
            int usage;

            sscanf(line, "%[^,],%d", code, &usage);

            if (strcmp(code, referralCode) == 0)
            {
                usage++;
                found = 1;
            }

            fprintf(tempFile, "%s,%d\n", code, usage);
        }

        fclose(file);
        fclose(tempFile);

        if (!found)
        {
            printf("\nReferral code %s not found in usage data.\n", referralCode);
        }

        remove(FILENAMEr);
        rename("temp_usage.txt", FILENAMEr);
    }
    else
    {
        FILE *file = fopen(FILENAMEr, "r");

        FILE *tempFile = fopen("temp_usage.txt", "w");

        char line[100];
        int found = 0;

        while (fgets(line, sizeof(line), file))
        {
            char code[20];
            int usage;

            sscanf(line, "%[^,],%d", code, &usage);

            if (strcmp(code, referralCode) == 0)
            {
                usage--;
                found = 1;
            }

            fprintf(tempFile, "%s,%d\n", code, usage);
        }

        fclose(file);
        fclose(tempFile);

        if (!found)
        {
            printf("\nReferral code %s not found in usage data.\n", referralCode);
        }

        remove(FILENAMEr);
        rename("temp_usage.txt", FILENAMEr);
    }
}
/////////////////////////////////////////////////////////////////
