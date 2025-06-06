#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct customer {
    int account_no;
    char customer_first_name[25];
    char customer_last_name[25];
    float balance;
};

void newcustomer();
void customerdetails();
void update_customerdetails();
void deposit();
void withdraw();

int main() {
    int option;
    while (1) {
        printf("\n_______________________________\n");
        printf("1. New Customer\n");
        printf("2. Customer Details\n");
        printf("3. Update Customer Details\n");
        printf("4. Deposit Amount\n");
        printf("5. Withdraw Amount\n");
        printf("6. Exit\n");
        printf("_______________________________\n");
        printf("Enter option: ");
        scanf("%d", &option);

        switch (option) {
            case 1: newcustomer(); break;
            case 2: customerdetails(); break;
            case 3: update_customerdetails(); break;
            case 4: deposit(); break;
            case 5: withdraw(); break;
            case 6: exit(0);
            default: printf("Invalid option. Please try again.\n");
        }
    }
}

void newcustomer() {
    FILE *fb = fopen("customer.dat", "ab+"), *ftxt;
    struct customer c;
    int exists = 0;

    if (!fb) {
        printf("File error.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &c.account_no);

    rewind(fb);
    while (fread(&c, sizeof(c), 1, fb)) {
        if (c.account_no == c.account_no) {
            exists = 1;
            break;
        }
    }

    if (exists) {
        printf("Account number already exists.\n");
        fclose(fb);
        return;
    }

    printf("Enter Customer First Name: ");
    scanf("%s", c.customer_first_name);
    printf("Enter Customer Last Name: ");
    scanf("%s", c.customer_last_name);
    printf("Enter Account Balance in Rs: ");
    scanf("%f", &c.balance);

    fwrite(&c, sizeof(c), 1, fb);
    fclose(fb);

    ftxt = fopen("customer.txt", "a");
    if (ftxt) {
        fprintf(ftxt, "%d %s %s %.2f\n", c.account_no, c.customer_first_name, c.customer_last_name, c.balance);
        fclose(ftxt);
    }

    printf("Customer added successfully.\n");
}

void customerdetails() {
    FILE *fb = fopen("customer.dat", "rb"), *ftxt = fopen("customer.txt", "w");
    struct customer c;

    if (!fb || !ftxt) {
        printf("File error.\n");
        if (fb) fclose(fb);
        if (ftxt) fclose(ftxt);
        return;
    }

    printf("\nCustomer Account Details:\n");
    printf("--------------------------------------------------\n");

    while (fread(&c, sizeof(c), 1, fb)) {
        printf("Account No: %d\n", c.account_no);
        printf("Name      : %s %s\n", c.customer_first_name, c.customer_last_name);
        printf("Balance   : %.2f\n", c.balance);
        printf("--------------------------------------------------\n");

        fprintf(ftxt, "%d %s %s %.2f\n", c.account_no, c.customer_first_name, c.customer_last_name, c.balance);
    }

    fclose(fb);
    fclose(ftxt);
}

void update_customerdetails() {
    FILE *fb = fopen("customer.dat", "rb+");
    struct customer c;
    int account_no, found = 0;

    if (!fb) {
        printf("File error.\n");
        return;
    }

    printf("Enter Account Number to update: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(c), 1, fb)) {
        if (c.account_no == account_no) {
            found = 1;
            printf("Current Name: %s %s\n", c.customer_first_name, c.customer_last_name);
            printf("Enter New First Name: ");
            scanf("%s", c.customer_first_name);
            printf("Enter New Last Name: ");
            scanf("%s", c.customer_last_name);

            fseek(fb, -sizeof(c), SEEK_CUR);
            fwrite(&c, sizeof(c), 1, fb);
            break;
        }
    }

    fclose(fb);
    found ? printf("Customer updated successfully.\n") : printf("Account number not found.\n");

    if (found) customerdetails();
}

void deposit() {
    FILE *fb = fopen("customer.dat", "rb+");
    struct customer c;
    int account_no, found = 0;
    float amount;

    if (!fb) {
        printf("File error.\n");
        return;
    }

    printf("Enter Account Number to deposit into: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(c), 1, fb)) {
        if (c.account_no == account_no) {
            found = 1;
            printf("Current Balance: %.2f\n", c.balance);
            printf("Enter Deposit Amount: ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("Invalid amount.\n");
                break;
            }

            c.balance += amount;
            fseek(fb, -sizeof(c), SEEK_CUR);
            fwrite(&c, sizeof(c), 1, fb);
            printf("Deposit successful. New Balance: %.2f\n", c.balance);
            break;
        }
    }

    fclose(fb);
    if (!found) printf("Account number not found.\n");
}

void withdraw() {
    FILE *fb = fopen("customer.dat", "rb+");
    struct customer c;
    int account_no, found = 0;
    float amount;

    if (!fb) {
        printf("File error.\n");
        return;
    }

    printf("Enter Account Number to withdraw from: ");
    scanf("%d", &account_no);

    while (fread(&c, sizeof(c), 1, fb)) {
        if (c.account_no == account_no) {
            found = 1;
            printf("Current Balance: %.2f\n", c.balance);
            printf("Enter Withdrawal Amount: ");
            scanf("%f", &amount);

            if (amount <= 0 || amount > c.balance) {
                printf("Invalid amount. Withdrawal failed.\n");
                break;
            }

            c.balance -= amount;
            fseek(fb, -sizeof(c), SEEK_CUR);
            fwrite(&c, sizeof(c), 1, fb);
            printf("Withdrawal successful. New Balance: %.2f\n", c.balance);
            break;
        }
    }

    fclose(fb);
    if (!found) printf("Account number not found.\n");
}
