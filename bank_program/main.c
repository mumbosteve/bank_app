#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME "accounts.dat"
//structure to hold account details
struct Account {
    int accountNumber;
    char name[100];
    float balance;
};

// Function prototypes
void createAccount();
void viewAccount();
void updateAccount();
void deleteAccount();
void displayAllAccounts();
void clearInputBuffer();

int main() {
    int choice;

    while (1) {
        printf("\n=== Bank Management System ===\n");
        printf("1. Create Account\n");
        printf("2. View Account\n");
        printf("3. Update Account\n");
        printf("4. Delete Account\n");
        printf("5. Display All Accounts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
//Read user choice and validate input
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        //Perform the selected operation
        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccount(); break;
            case 3: updateAccount(); break;
            case 4: deleteAccount(); break;
            case 5: displayAllAccounts(); break;
            case 6: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

//Function to create a new account
void createAccount() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "ab");

    if (fp == NULL) {
        perror("Error opening file for writing");
        return;
    }

   //Get account number from user
    printf("Enter account number: ");
    if (scanf("%d", &acc.accountNumber) != 1) {
        printf("Invalid input for account number.\n");
        clearInputBuffer();
        fclose(fp);
        return;
    }
    // Preventing duplicate account numbers
     FILE *checkFp = fopen("accounts.dat", "rb");
struct Account temp;
while (fread(&temp, sizeof(temp), 1, checkFp)) {
    if (temp.accountNumber == acc.accountNumber) {
        printf("Account number already exists.\n");
        fclose(fp);
        fclose(checkFp);
        return;
    }
}
fclose(checkFp);
    clearInputBuffer();

    //Get account holder's name
    printf("Enter name: ");
    if (fgets(acc.name, sizeof(acc.name), stdin) == NULL) {
        printf("Error reading name.\n");
        fclose(fp);
        return;
    }
    acc.name[strcspn(acc.name, "\n")] = 0;

    //Get initial account balance
    printf("Enter initial balance: ");
    if (scanf("%f", &acc.balance) != 1 || acc.balance < 0) {
        printf("Invalid balance amount.\n");
        clearInputBuffer();
        fclose(fp);
        return;
    }

    if (fwrite(&acc, sizeof(acc), 1, fp) != 1) {
        printf("Error writing to file.\n");
    } else {
        printf("Account created successfully.\n");
    }

    fclose(fp);
}

void viewAccount() {
    int num, found = 0;
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "rb");

    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }
    
    printf("Enter account number to view: ");
    if (scanf("%d", &num) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        fclose(fp);
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNumber == num) {
            printf("Account Number: %d\n", acc.accountNumber);
            printf("Name: %s\n", acc.name);
            printf("Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found) printf("Account not found.\n");

    fclose(fp);
}

void updateAccount() {
    int num, found = 0;
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "rb+");

    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("Enter account number to update: ");
    if (scanf("%d", &num) != 1) {
        printf("Invalid account number.\n");
        clearInputBuffer();
        fclose(fp);
        return;
    }
    clearInputBuffer();

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNumber == num) {
            printf("Enter new name: ");
            if (fgets(acc.name, sizeof(acc.name), stdin) == NULL) {
                printf("Error reading name.\n");
                break;
            }
            acc.name[strcspn(acc.name, "\n")] = 0;

            printf("Enter new balance: ");
            if (scanf("%f", &acc.balance) != 1 || acc.balance < 0) {
                printf("Invalid balance.\n");
                clearInputBuffer();
                break;
            }

            fseek(fp, -sizeof(acc), SEEK_CUR);
            if (fwrite(&acc, sizeof(acc), 1, fp) == 1) {
                printf("Account updated successfully.\n");
            } else {
                printf("Error updating account.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) printf("Account not found.\n");

    fclose(fp);
}

void deleteAccount() {
    int num, found = 0;
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("Error accessing files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    printf("Enter account number to delete: ");
    if (scanf("%d", &num) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        fclose(fp);
        fclose(temp);
        return;
    }

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accountNumber == num) {
            found = 1;
            continue;
        }
        fwrite(&acc, sizeof(acc), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (remove("accounts.dat") != 0 || rename("temp.dat", "accounts.dat") != 0) {
        printf("Error updating data files.\n");
        return;
    }

    if (found)
        printf("Account deleted successfully.\n");
    else
        printf("Account not found.\n");
}

void displayAllAccounts() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "rb");

    if (fp == NULL) {
        printf("No accounts to display.\n");
        return;
    }

    printf("\n--- All Accounts ---\n");
    int count = 0;
    while (fread(&acc, sizeof(acc), 1, fp)) {
        printf("Account Number: %d\n", acc.accountNumber);
        printf("Name: %s\n", acc.name);
        printf("Balance: %.2f\n\n", acc.balance);
        count++;
    }

    if (count == 0)
        printf("No accounts found.\n");

    fclose(fp);
}

// Utility to clear the input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
