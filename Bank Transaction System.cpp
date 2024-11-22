#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct Account {
    int account_number;
    string account_holder_name;
    double balance;
    Account* next;

    Account(int acc_n, string nam, double bal) {
        account_number = acc_n;
        account_holder_name = nam;
        balance = bal;
        next = nullptr;
    }
};

class Bank {
private:
    Account* head;

    void clearList() {
        while (head) {
            Account* temp = head;
            head = head->next;
            delete temp;
        }
    }

public:
    Bank() 
    {
        head = NULL;
    }
    ~Bank() {
        clearList();
    }

    void loadFromFile() {
        clearList();
        ifstream file("accounts.txt");
        if (!file.is_open()) {
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string acc_num_str, name, balance_str;
            getline(ss, acc_num_str, ',');
            getline(ss, name, ',');
            getline(ss, balance_str, ',');

            if (acc_num_str.empty() || name.empty() || balance_str.empty()) continue;

            int acc_num = stoi(acc_num_str);
            double balance = stod(balance_str);

            Account* newAccount = new Account(acc_num, name, balance);
            newAccount->next = head;
            head = newAccount;
        }
        file.close();
    }

    void saveToFile() {
        ofstream file("accounts.txt");
        if (!file.is_open()) {
            cout << "Error: Could not save to file." << endl;
            return;
        }

        Account* temp = head;
        while (temp) {
            file << temp->account_number << "," << temp->account_holder_name << "," << temp->balance << endl;
            temp = temp->next;
        }
        file.close();
    }

    void createAccount(int acc_n, string name, double bal) {
        if (bal < 0) {
            cout << "Initial balance cannot be negative." << endl;
            return;
        }

        loadFromFile();
        Account* temp = head;
        while (temp) {
            if (temp->account_number == acc_n) {
                cout << "Account number is already exist. Please Enter a different number." << endl<<endl;
                return;
            }
            temp = temp->next;
        }

        Account* newAccount = new Account(acc_n, name, bal);
        newAccount->next = head;
        head = newAccount;
        saveToFile();
        cout << "Account created successfully!" << endl<<endl;
    }

    void displayAccount(int acc_n) {
        loadFromFile();
        Account* temp = head;
        while (temp) {
            if (temp->account_number == acc_n) {
                cout << "Account Number: " << temp->account_number << ", "
                    << "Name: " << temp->account_holder_name << ", "
                    << "Balance: " << temp->balance << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Account not found." << endl<<endl;
    }

    void deposit(int acc_n, double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount. Please enter a positive value." << endl<<endl;
            return;
        }

        loadFromFile();
        Account* temp = head;
        while (temp) {
            if (temp->account_number == acc_n) {
                temp->balance += amount;
                cout << "The amount " << amount << "Rs/- deposited to account *" << acc_n
                    << "* Your new Balance is: " << temp->balance << endl << endl;
                saveToFile();
                return;
            }
            temp = temp->next;
        }
        cout << "Account not found." << endl<<endl;
    }

    void withdraw(int acc_n, double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount. Please enter a positive value." << endl<<endl;
            return;
        }

        loadFromFile();
        Account* temp = head;
        while (temp) {
            if (temp->account_number == acc_n) {
                if (temp->balance >= amount) {
                    double tax = 0;
                    if (amount >= 1000 && amount < 10000)
                    {
                        tax = 30;
                    }
                    else if (amount >= 10000)
                    {
                        tax = 70;
                    }
                    temp->balance -= (amount + tax);
                    cout << "The amount " << amount << "Rs/-Withdrawn from account *" << acc_n
                        << "* with tax " << tax
                        << ". Your new Balance is : " << temp->balance << endl << endl;
                    saveToFile();
                }
                else {
                    cout << " Insufficient balance in your Account ." << endl<<endl;
                }
                return;
            }
            temp = temp->next;
        }
        cout << "Account not found." << endl<<endl;
    }

    void displayAllAccounts() {
        loadFromFile();
        Account* temp = head;
        if (temp==NULL)
        {
            cout << "No accounts to display.Please create an account first: " << endl;
            return;
        }
        while (temp!=NULL)
        {
            cout << "Account Number: " << temp->account_number << ", "
                << "Name: " << temp->account_holder_name << ", "
                << "Balance: " << temp->balance << endl;
            temp = temp->next;
        }
    }
};

int main() {
    Bank bank;
    int choice, acc_n;
    string name;
    double balance, amount;

    do {
        cout << "\n***********BANK TRANSACTION SYSTEM************" << endl<<endl;
        cout << "************************************************" << endl;
        cout << "** Enter '1' to create a new Account :        **" << endl;
        cout << "** Enter '2' to Display Account Details :     **" << endl;
        cout << "** Enter '3' to Deposit Money in Account :    **" << endl;
        cout << "** Enter '4' to Withdraw Money from Account:  **" << endl;
        cout << "** Enter '5' to Display all Accounts:         **" << endl;
        cout << "** Enter '6' to Exit the program :            **" << endl;
        cout << "************************************************" << endl << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter account number: ";
            cin >> acc_n;
            cin.ignore();
            cout << "Enter name: ";
            getline(cin, name);
            cout << "Enter initial balance: ";
            cin >> balance;
            bank.createAccount(acc_n, name, balance);
            break;

        case 2:
            cout << "Enter account number: ";
            cin >> acc_n;
            bank.displayAccount(acc_n);
            break;

        case 3:
            cout << "Enter account number: ";
            cin >> acc_n;
            cout << "Enter amount to deposit: ";
            cin >> amount;
            bank.deposit(acc_n, amount);
            break;

        case 4:
            cout << "Enter account number: ";
            cin >> acc_n;
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            bank.withdraw(acc_n, amount);
            break;

        case 5:
            bank.displayAllAccounts();
            break;

        case 6:
            cout << "Exiting program. Thank you! for using our BANK TRANSACTION SYSTEM. " << endl;
            break;

        default:
            cout << "Invalid choice.Please Try again." << endl<<endl;
        }
    } while (choice != 6);

    return 0;
}