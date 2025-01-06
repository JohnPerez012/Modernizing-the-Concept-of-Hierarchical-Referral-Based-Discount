
#include <stdio.h>
#include "nodesNprototypes.h"

void registerUser()
{
    FILE *file = fopen(FILENAME, "a");
    UserNode newUser;

    system("cls");
    printf("\n\nRegister Account\n\n");

    printf("Enter your full name: ");
    fgets(newUser.fullName, sizeof(newUser.fullName), stdin);
    strtok(newUser.fullName, "\n");

    char referral[10];
    bool valid = false;
    do
    {
        printf("Enter referral code: ");
        scanf("%9s", referral);
        clearBuffer();

        if (!isValidReferral(referral) && strcmp(referral, ROOT_REFERRAL) != 0)
        {
            printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------+\n +-------+---------+\t ");
            printf("Invalid referral code \t|\n\t |\t\t Returning to menu.\t|\n\t +--------------------------------------+\n");
            Sleep(1999);
            fclose(file);
            return;
        }

        valid = checkReferralUsage(referral);
        if (!valid)
        {
            printf("\n\n +-----------------+\n | POP-UP MESSAGE! |--------------------------------------------+\n +-------+---------+\t ");
            printf(" Referral code has reached its limit. \t|\n\t |\t\t Please try again.\t\t\t|\n\t +------------------------------------------------------+\n");
            Sleep(1234);
            system("cls");
            printf("\n\nRegister Account\n\n");
            printf("Enter your full name: %s\n", newUser.fullName);
            printf("Enter referral code: %s\n\n", referral);
            suggestReferralCodes();
        }
    } while (!valid);

    strcpy(newUser.userReferralFrom, referral);

    do
    {
        printf("\n(4 - 6 digits)\nCreate your ID: ");
        scanf("%d", &newUser.id);
        clearBuffer();

        if (!independentId(newUser.id))
        {
            printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------+\n");
            printf(" +-------+---------+\t ID already taken.  \t|\n\t |\t\t Please try again.\t|\n\t +--------------------------------------+\n");
            Sleep(1999);
            system("cls");
            printf("\n\nRegister Account\n\n");
            printf("Enter your full name: %s\n", newUser.fullName);
            printf("Enter referral code: %s\n", referral);
        }
    } while (!independentId(newUser.id));

    printf("Create your PIN: ");
    scanf("%d", &newUser.pin);
    clearBuffer();

    printf("\n\n +-----------------------+\n | CONFIRMATION MESSAGE! |----------------------------------------------+\n +-------+---------------+\t Are you sure you want to use the ");
    printf("\t|\n\t |\t\t\t referral code.\t      [Yes or No]\t|\n\t +--------------------------------------------------------------+");
    printf("\n\t +--------------------------------------------------->\n\t |\tReferral Code: [%s]\n\t |\tReferral from: [%s]\n\t +----------------------------------------->\n\t", referral, getUsername(referral));

    printf(" Enter choice: ");
    char choice;
    scanf(" %c", &choice);
    clearBuffer();

    if (tolower(choice) != 'y')
    {
        printf("\n\n +-----------------+\n | POP-UP MESSAGE! |--------------------------------------------+\n +-------+---------+\t ");
        printf("Referral registration canceled \t|\n\t |\t\t Returning to menu. \t\t\t|\n\t +------------------------------------------------------+\n");

        Sleep(1950);
        fclose(file);
        return;
    }

    generateUniqueReferral(newUser.userReferral);

    fprintf(file, "%s,%s,%d,%d,%s,%d\n",
            newUser.fullName,
            newUser.userReferral,
            newUser.id,
            newUser.pin,
            newUser.userReferralFrom,
            (newUser.discount = 0));

    appendReferralCodeToDataSheet(newUser.userReferral);
    referralUsage(referral, 1);
    printf("\n\n +-----------------+\n | POP-UP MESSAGE! |------------------------------------+\n");
    printf(" +-------+---------+\t Account created successfully!\t|\n\t +----------------------------------------------+\n");
    Sleep(1500);
    system("cls");
    printf("\n\n\tSuccessful registration\n\nFullname: %s\n", newUser.fullName);
    Sleep(400);
    printf("ID: %d\n", newUser.id);
    Sleep(350);
    char pinString[20];
    sprintf(pinString, "%d", newUser.pin);
    Sleep(300);
    printf("PIN: %s\n", maskFull(pinString));
    Sleep(250);
    fclose(file);
    printf("Referral by: [%s] \n\n", getUsername(referral));
    Sleep(250);
    printf("\n\n +-----------------+\n | POP-UP MESSAGE! |--------------------------------------------+\n +-------+---------+\t   Account referral code: [%s]\t|", newUser.userReferral);
    printf("\n\t |\t       -> Enter any key to get back to menu <- \t|\n\t +------------------------------------------------------+\n");
    getch();
}
///////////////////////////////////////////////////////////////////
int loginUser()
{
    FILE *file = fopen(FILENAME, "r");

    int id, pin, referral;
    printf("\nEnter your ID: ");
    scanf("%d", &id);
    hideId();
    printf("Enter your PIN: ");
    scanf("%d", &pin);

    char line[200];
    while (fgets(line, sizeof(line), file))
    {
        UserNode user;
        sscanf(line, "%[^,],%[^,],%d,%d,%[^,]", user.fullName, user.userReferral, &user.id, &user.pin, user.userReferralFrom);
        if (user.id == id && user.pin == pin)
        {
            fclose(file);
            system("cls");
            if (strcmp(user.fullName, "admin") == 0 && (USERADMINPRIVID == id && USERADMINPRIVPIN == pin))
            {
                homepageADMIN();
            }
            else
            {
                if (strcmp(user.userReferralFrom, "-1") == 0)
                {
                    printf("\n\nYour account is not linked to another account\n\ndo you want to link your account? \nChoose option [YES or NO]: ");
                    char choice;
                    scanf(" %c", &choice);
                    clearBuffer();

                    if (tolower(choice) == 'y')
                    {

                        char refcode[10];
                        bool valid = false;
                        do
                        {
                            printf("Enter referral code to link with:\n");

                            scanf("%9s", refcode);
                            clearBuffer();

                            if (!isValidReferral(refcode) && strcmp(refcode, ROOT_REFERRAL) != 0)
                            {
                                printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------+\n");
                                printf(" +-------+---------+\t Invalid referral code \t|\n\t |\t\t Returning to menu.\t|\n\t +--------------------------------------+\n");
                                Sleep(1999);
                                fclose(file);
                                return 0;
                            }

                            valid = checkReferralUsage(refcode);
                            if (!valid)
                            {
                                printf("\n\n +-----------------+\n | POP-UP MESSAGE! |--------------------------------------------+\n");
                                printf(" +-------+---------+\t Referral code has reached its limit. \t|\n\t |\t\t Please try again.\t\t\t|\n\t +------------------------------------------------------+\n");
                                Sleep(999);
                                suggestReferralCodes();
                            }
                        } while (!valid);

                        linkMyAccount(user.fullName, refcode);
                        printf("Welcome to Home Page %s!", user.fullName);
                        Sleep(950);
                        homepage(user.userReferral);
                    }
                    else
                    {
                           printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------------------------------+\n");
                                printf(" +-------+---------+\t You need to link your account to continue \t|\n\t |\t\t Please try again.\t\t\t\t|\n\t +--------------------------------------------------------------+\n");
                                Sleep(1999);                    
                        return 0;
                    }
                }
                else
                {
                    printf("Welcome to Home Page %s!", user.fullName);
                    Sleep(950);
                    homepage(user.userReferral);
                }
            }
            return 1;
        }
    }
    printf("\n\n +-----------------+\n | POP-UP MESSAGE! |----------------------------+\n");
    printf(" +-------+---------+\t Invalid ID or PIN.\t|\n\t +--------------------------------------+\n");

    Sleep(750);
    fclose(file);
    return 0;
}
///////////////////////////////////////////////////////////////////
char *maskFull(char *input)
{
    for (int i = 0; i < strlen(input); i++)
    {
        input[i] = '*';
    }
    return input;
}
///////////////////////////////////////////////////////////////////
void appendReferralCodeToDataSheet(const char *referral)
{
    FILE *file = fopen(FILENAMEr, "a");
    fprintf(file, "%s,0\n", referral);
    fclose(file);
}
///////////////////////////////////////////////////////////////////
char *getUsername(const char *referral)
{
    FILE *file = fopen(FILENAME, "r");
    static char username[100];
    char line[200];

    while (fgets(line, sizeof(line), file))
    {
        char user[100];
        char userReferral[100];
        char id[50], pin[50], referralFrom[50], discount[50];

        if (sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",
                   user, userReferral, id, pin, referralFrom, discount) == 6)
        {
            if (strcmp(userReferral, referral) == 0)
            {
                fclose(file);
                strcpy(username, user);
                return username;
            }
        }
    }

    fclose(file);
    return NULL;
}
///////////////////////////////////////////////////////////////////
