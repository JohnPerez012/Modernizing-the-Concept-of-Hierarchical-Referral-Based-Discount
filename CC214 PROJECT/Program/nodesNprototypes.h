#ifndef NODES_PROTOTYPES
#define NODES_PROTOTYPES

#define FILENAME "users.txt"
#define FILENAMEr "referrals.txt"
#define ROOT_REFERRAL "100000"
#define FIRSTRULE 20
#define SECONDRULE 5
#define THIRDRULE 2
#define USERADMINPRIVID 0
#define USERADMINPRIVPIN 0


#include <stdbool.h>

typedef struct UserNode
{
    char fullName[100];
    char userReferral[10];
    int id;
    int pin;
    char userReferralFrom[10];
    int discount;
    int usage;
} UserNode;

// program runner c
void checkRootNreferrals();
void menu();
void clearBuffer();
int isValidReferral(const char *referral);
bool checkReferralUsage(const char *referral);
void suggestReferralCodes();
int linkMyAccount(const char *username, const char *refcode);
bool independentId(int id);
void generateUniqueReferral(char *newReferral);
void hideId();
void discount(const char *value);
void animation(int value);
void referralUsage(const char *referralCode, int order);

// homepage.h
void homepage(const char *username);
void viewAccount(const char *username);
void whoUsesMyReferral(const char *referralCode);
void unlinkReferral(const char *referralCode);

// loginNregister.h
void registerUser();
int loginUser();
char *maskFull(char *input);
void appendReferralCodeToDataSheet(const char *referral);
char *getUsername(const char *referral);

void homepageADMIN();


#endif
