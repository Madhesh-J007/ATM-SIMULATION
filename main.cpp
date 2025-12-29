#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>

using namespace std;

/* =======================
   Account Class
   ======================= */
class Account {
public:
    string name;
    int accountNum;
    int pin;
    float balance;
};

/* =======================
   Function Declarations
   ======================= */
void checkBalance(const Account &acc);
void deposit(Account &acc);
void withdraw(Account &acc);
void atmMenu(vector<Account> &accounts, int index);

void saveAccountToFile(const vector<Account> &accounts);
void loadAccountDetails(vector<Account> &accounts);
bool accountExists(const vector<Account> &accounts, int accNo);
void createAccount(vector<Account> &accounts);
void deleteAccount(vector<Account> &accounts, int index);
int login(vector<Account> &accounts);

/* =======================
   File Handling
   ======================= */
void saveAccountToFile(const vector<Account> &accounts) {
    ofstream file("accounts.txt");

    if (!file) {
        cout << "Error saving account data!" << endl;
        return;
    }

    for (const auto &acc : accounts) {
        file << acc.name << " "
             << acc.accountNum << " "
             << acc.pin << " "
             << acc.balance << endl;
    }

    file.close();
}

void loadAccountDetails(vector<Account> &accounts) {
    ifstream file("accounts.txt");
    Account acc;

    if (!file) return;

    while (file >> acc.name >> acc.accountNum >> acc.pin >> acc.balance) {
        accounts.push_back(acc);
    }

    file.close();
}

/* =======================
   Account Utilities
   ======================= */
bool accountExists(const vector<Account> &accounts, int accNo) {
    for (const auto &acc : accounts) {
        if (acc.accountNum == accNo)
            return true;
    }
    return false;
}

void createAccount(vector<Account> &accounts) {
    Account acc;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Account Holder Name (No Spaces): ";
    getline(cin, acc.name);

    do{
        cout << "Enter 5-Digit Account Number: ";
        cin >> acc.accountNum;
    }while(acc.accountNum < 10000 || acc.accountNum > 99999);

    if (accountExists(accounts, acc.accountNum)) {
        cout << "Account number already exists!" << endl;
        return;
    }

    do {
        cout << "Set 4-digit PIN: ";
        cin >> acc.pin;
    } while (acc.pin < 1000 || acc.pin > 9999);

    acc.balance = 500; // Minimum balance

    accounts.push_back(acc);
    cout << "Account created successfully!" << endl;
}

void deleteAccount(vector<Account> &accounts, int index) {
    char confirm;
    cout << "Are you sure you want to delete this account? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        accounts.erase(accounts.begin() + index);
        cout << "Account deleted successfully!" << endl;
    } else {
        cout << "Account deletion cancelled." << endl;
    }
}

/* =======================
   Login System
   ======================= */
int login(vector<Account> &accounts) {
    int accNo, enteredPin;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    cout << "Enter Account Number: ";
    cin >> accNo;

    int index = -1;
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].accountNum == accNo) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Account not found!" << endl;
        return -1;
    }

    while (attempts < MAX_ATTEMPTS) {
        cout << "Enter PIN: ";
        cin >> enteredPin;

        if (enteredPin == accounts[index].pin) {
            cout << "Login successful!" << endl;
            return index;
        }

        attempts++;
        cout << "Incorrect PIN. Attempts left: "
             << (MAX_ATTEMPTS - attempts) << endl;
    }

    cout << "Too many wrong attempts. Card blocked!" << endl;
    return -1;
}

/* =======================
   ATM Operations
   ======================= */
void checkBalance(const Account &acc) {
    cout << "---------------------------" << endl;
    cout << "Current Balance: " << acc.balance << endl;
    cout << "---------------------------" << endl;
}

void deposit(Account &acc) {
    float amount;
    cout << "Enter amount to deposit: ";
    cin >> amount;

    if (amount > 0) {
        acc.balance += amount;
        cout << "Deposit successful!" << endl;
    } else {
        cout << "Invalid amount!" << endl;
    }
}

void withdraw(Account &acc) {
    float amount;
    cout << "Enter amount to withdraw: ";
    cin >> amount;

    if (amount > 0 && acc.balance - amount >= 500) {
        acc.balance -= amount;
        cout << "Withdrawal successful!" << endl;
    } else {
        cout << "Insufficient balance or minimum balance rule violated!" << endl;
    }
}

void atmMenu(vector<Account> &accounts, int index) {
    int choice;

    do {
        cout << "\n----- ATM MENU -----" << endl;
        cout << "1. Check Balance" << endl;
        cout << "2. Deposit Money" << endl;
        cout << "3. Withdraw Money" << endl;
        cout << "4. Delete Account" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: checkBalance(accounts[index]); break;
            case 2: deposit(accounts[index]); break;
            case 3: withdraw(accounts[index]); break;
            case 4: deleteAccount(accounts, index); return;
            case 5: cout << "Thank you for using ATM!" << endl; break;
            default: cout << "Invalid choice!" << endl;
        }
    } while (choice != 5);
}

/* =======================
   Main Function
   ======================= */
int main() {
    vector<Account> accounts;
    loadAccountDetails(accounts);

    int choice;
    cout << "\n1. Create New Account" << endl;
    cout << "2. Login" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        createAccount(accounts);
        saveAccountToFile(accounts);
    }
    else if (choice == 2) {
        int index = login(accounts);
        if (index != -1) {
            atmMenu(accounts, index);
            saveAccountToFile(accounts);
        } else {
            cout << "Login failed. Exiting..." << endl;
        }
    }
    else {
        cout << "Invalid option!" << endl;
    }

    return 0;
}
