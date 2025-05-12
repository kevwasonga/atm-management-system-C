#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void updateAccountInfo(struct User u)
{
    int accountNum, choice;
    char country[100];
    int phone;
    int found = 0;

    FILE *oldFile, *newFile;
    char userName[100];
    struct Record r;

    system("clear");
    printf("\t\t\t===== Update Account Information =====\n");
    printf("\nEnter account number to update: ");
    scanf("%d", &accountNum);

    oldFile = fopen(RECORDS, "r");
    newFile = fopen("./data/temp.txt", "w");

    while (getAccountFromFile(oldFile, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNum)
        {
            found = 1;
            printf("\nAccount found!\n");
            printf("\nWhat would you like to update?\n");
            printf("1. Country\n");
            printf("2. Phone number\n");
            printf("Enter your choice (1-2): ");
            scanf("%d", &choice);

            switch (choice)
            {
                case 1:
                    printf("\nEnter new country: ");
                    scanf("%s", country);
                    strcpy(r.country, country);
                    break;
                case 2:
                    printf("\nEnter new phone number: ");
                    scanf("%d", &phone);
                    r.phone = phone;
                    break;
                default:
                    printf("\nInvalid choice. No changes made.\n");
                    break;
            }
        }

        // Write the record to the new file (updated or not)
        fprintf(newFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                r.id,
                r.userId,
                userName,
                r.accountNbr,
                r.deposit.month,
                r.deposit.day,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
    }

    fclose(oldFile);
    fclose(newFile);

    // Replace the old file with the new file
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    if (found)
    {
        printf("\nAccount information updated successfully!\n");
    }
    else
    {
        printf("\nAccount not found!\n");
    }

    success(u);
}

void checkAccountDetails(struct User u)
{
    int accountNum;
    int found = 0;
    double interest = 0;

    FILE *fp;
    char userName[100];
    struct Record r;

    system("clear");
    printf("\t\t\t===== Check Account Details =====\n");
    printf("\nEnter account number to check: ");
    scanf("%d", &accountNum);

    fp = fopen(RECORDS, "r");

    while (getAccountFromFile(fp, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNum)
        {
            found = 1;
            printf("\n_____________________\n");
            printf("\nAccount Details:\n");
            printf("Account Number: %d\n", r.accountNbr);
            printf("Deposit Date: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("Country: %s\n", r.country);
            printf("Phone Number: %d\n", r.phone);
            printf("Current Balance: $%.2f\n", r.amount);
            printf("Account Type: %s\n", r.accountType);

            // Calculate interest based on account type
            if (strcmp(r.accountType, "saving") == 0)
            {
                interest = r.amount * 0.07 / 12; // 7% annual interest, monthly calculation
                printf("\nYou will get $%.2f as interest on day %d of every month\n", interest, r.deposit.day);
            }
            else if (strcmp(r.accountType, "fixed01") == 0)
            {
                interest = r.amount * 0.04; // 4% interest for 1 year
                printf("\nYou will get $%.2f as interest after 1 year\n", interest);
            }
            else if (strcmp(r.accountType, "fixed02") == 0)
            {
                interest = r.amount * 0.05; // 5% interest for 2 years
                printf("\nYou will get $%.2f as interest after 2 years\n", interest);
            }
            else if (strcmp(r.accountType, "fixed03") == 0)
            {
                interest = r.amount * 0.08; // 8% interest for 3 years
                printf("\nYou will get $%.2f as interest after 3 years\n", interest);
            }
            else if (strcmp(r.accountType, "current") == 0)
            {
                printf("\nThis account type does not provide any interest\n");
            }

            break;
        }
    }

    fclose(fp);

    if (!found)
    {
        printf("\nAccount not found!\n");
    }

    success(u);
}

void makeTransaction(struct User u)
{
    int accountNum, choice;
    double amount;
    int found = 0;

    FILE *oldFile, *newFile;
    char userName[100];
    struct Record r;

    system("clear");
    printf("\t\t\t===== Make Transaction =====\n");
    printf("\nEnter account number: ");
    scanf("%d", &accountNum);

    oldFile = fopen(RECORDS, "r");
    newFile = fopen("./data/temp.txt", "w");

    while (getAccountFromFile(oldFile, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNum)
        {
            found = 1;

            // Check if account type is fixed - no transactions allowed
            if (strcmp(r.accountType, "fixed01") == 0 ||
                strcmp(r.accountType, "fixed02") == 0 ||
                strcmp(r.accountType, "fixed03") == 0)
            {
                printf("\nError: Transactions are not allowed for fixed deposit accounts.\n");

                // Write the unchanged record
                fprintf(newFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                        r.id,
                        r.userId,
                        userName,
                        r.accountNbr,
                        r.deposit.month,
                        r.deposit.day,
                        r.deposit.year,
                        r.country,
                        r.phone,
                        r.amount,
                        r.accountType);

                continue;
            }

            printf("\nAccount found!\n");
            printf("Current Balance: $%.2f\n", r.amount);
            printf("\nWhat would you like to do?\n");
            printf("1. Deposit\n");
            printf("2. Withdraw\n");
            printf("Enter your choice (1-2): ");
            scanf("%d", &choice);

            switch (choice)
            {
                case 1:
                    printf("\nEnter amount to deposit: $");
                    scanf("%lf", &amount);
                    r.amount += amount;
                    printf("\nDeposit successful. New balance: $%.2f\n", r.amount);
                    break;
                case 2:
                    printf("\nEnter amount to withdraw: $");
                    scanf("%lf", &amount);
                    if (amount > r.amount)
                    {
                        printf("\nError: Insufficient funds.\n");
                    }
                    else
                    {
                        r.amount -= amount;
                        printf("\nWithdrawal successful. New balance: $%.2f\n", r.amount);
                    }
                    break;
                default:
                    printf("\nInvalid choice. No transaction made.\n");
                    break;
            }
        }

        // Write the record to the new file (updated or not)
        fprintf(newFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                r.id,
                r.userId,
                userName,
                r.accountNbr,
                r.deposit.month,
                r.deposit.day,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
    }

    fclose(oldFile);
    fclose(newFile);

    // Replace the old file with the new file
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    if (!found)
    {
        printf("\nAccount not found!\n");
    }

    success(u);
}

void removeAccount(struct User u)
{
    int accountNum;
    int found = 0;
    char confirm;

    FILE *oldFile, *newFile;
    char userName[100];
    struct Record r;

    system("clear");
    printf("\t\t\t===== Remove Account =====\n");
    printf("\nEnter account number to remove: ");
    scanf("%d", &accountNum);

    oldFile = fopen(RECORDS, "r");
    newFile = fopen("./data/temp.txt", "w");

    while (getAccountFromFile(oldFile, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNum)
        {
            found = 1;
            printf("\nAccount found!\n");
            printf("\nAccount Details:\n");
            printf("Account Number: %d\n", r.accountNbr);
            printf("Current Balance: $%.2f\n", r.amount);
            printf("Account Type: %s\n", r.accountType);

            printf("\nAre you sure you want to remove this account? (y/n): ");
            scanf(" %c", &confirm);

            if (confirm == 'y' || confirm == 'Y')
            {
                printf("\nAccount successfully removed!\n");
                continue; // Skip writing this record to the new file
            }
            else
            {
                printf("\nAccount removal cancelled.\n");
                // Write the record since removal was cancelled
                fprintf(newFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                        r.id,
                        r.userId,
                        userName,
                        r.accountNbr,
                        r.deposit.month,
                        r.deposit.day,
                        r.deposit.year,
                        r.country,
                        r.phone,
                        r.amount,
                        r.accountType);
            }
        }
        else
        {
            // Write all other records to the new file
            fprintf(newFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                    r.id,
                    r.userId,
                    userName,
                    r.accountNbr,
                    r.deposit.month,
                    r.deposit.day,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType);
        }
    }

    fclose(oldFile);
    fclose(newFile);

    // Replace the old file with the new file
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    if (!found)
    {
        printf("\nAccount not found!\n");
    }

    success(u);
}

void transferOwnership(struct User u)
{
    int accountNum;
    int found = 0;
    char newOwner[50];
    char confirm;
    int newOwnerId = -1;

    FILE *oldFile, *newFile, *userFile;
    char userName[100];
    struct Record r;
    struct User userChecker;

    system("clear");
    printf("\t\t\t===== Transfer Account Ownership =====\n");
    printf("\nEnter account number to transfer: ");
    scanf("%d", &accountNum);

    // First check if the account exists and belongs to the current user
    oldFile = fopen(RECORDS, "r");

    while (getAccountFromFile(oldFile, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNum)
        {
            found = 1;
            break;
        }
    }

    fclose(oldFile);

    if (!found)
    {
        printf("\nAccount not found or does not belong to you!\n");
        success(u);
        return;
    }

    // Now get the new owner's username
    printf("\nAccount found!\n");
    printf("\nEnter the username of the new owner: ");
    scanf("%s", newOwner);

    // Check if the new owner exists
    userFile = fopen("./data/users.txt", "r");
    int userExists = 0;

    while (fscanf(userFile, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, newOwner) == 0)
        {
            userExists = 1;
            newOwnerId = userChecker.id;
            break;
        }
    }

    fclose(userFile);

    if (!userExists)
    {
        printf("\nError: User '%s' does not exist!\n", newOwner);
        success(u);
        return;
    }

    // Confirm the transfer
    printf("\nAre you sure you want to transfer account #%d to %s? (y/n): ", accountNum, newOwner);
    scanf(" %c", &confirm);

    if (confirm != 'y' && confirm != 'Y')
    {
        printf("\nTransfer cancelled.\n");
        success(u);
        return;
    }

    // Perform the transfer
    oldFile = fopen(RECORDS, "r");
    newFile = fopen("./data/temp.txt", "w");

    while (getAccountFromFile(oldFile, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNum)
        {
            // Update the owner information
            r.userId = newOwnerId;

            // Write the updated record
            fprintf(newFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                    r.id,
                    r.userId,
                    newOwner, // Use the new owner's name
                    r.accountNbr,
                    r.deposit.month,
                    r.deposit.day,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType);

            printf("\nAccount successfully transferred to %s!\n", newOwner);
        }
        else
        {
            // Write all other records unchanged
            fprintf(newFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                    r.id,
                    r.userId,
                    userName,
                    r.accountNbr,
                    r.deposit.month,
                    r.deposit.day,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType);
        }
    }

    fclose(oldFile);
    fclose(newFile);

    // Replace the old file with the new file
    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    success(u);
}
