#include <iostream>
#include <string>
#include <limits>

using std::cout, std::cin, std::getline, std::string, std::numeric_limits, std::streamsize;         //Namespace directives for simplicity's sake

string stripSpace(string& str){                             //Strips leading and trailing whitespace
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == string::npos) {
        return "";
    }
    
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

class Transaction{                          //Simple transaction node class, includes type, balance and alteration, simple constructor, and next pointer/related functions, includes display function
    private:
        string type;
        double oldBalance;
        double balanceChange;
        double newBalance;
        Transaction* next;

    public:        
        Transaction(string w, double x, double y) : type(w), oldBalance(x), balanceChange(y), newBalance(x + y), next(nullptr) {}                 //Simple constructor for next pointer, calls setter for transaction info, simpler than using constructor

        void displayTransaction(){                //Method to be called by TransactionHistory class
            cout << "\n***************************************\n";
            cout << "Transaction type: " << type << "\n";
            cout << "\nOld Balance: $" << oldBalance << "\n";
            if(balanceChange > 0){
                cout << "Transaction: +$" << balanceChange << "\n";

            } else {
                cout << "Transaction: -$" << balanceChange << "\n";
            }

            cout << "End Balance: $" << newBalance << "\n";
        }

        Transaction* getNext(){
            return next;
        }

        void setNext(Transaction* newNext){
            next = newNext;
        }
};

class TransactionHistory{           //Single link list class for transaction history, includes head pointer, add transaction function, and display history function
    private:
        Transaction* head = nullptr;
        Transaction* tail = nullptr;

    public:
        TransactionHistory() : head(nullptr), tail(nullptr) {}             //Simple constructor for head pointer

        ~TransactionHistory(){              //Destructor to delete all nodes in list
            Transaction* current = head;
            while(current != nullptr){
                Transaction* next = current -> getNext();
                delete current;
                current = next;
            }
        }

        void addRecord(string type, double oldBalance, double balanceChange){              //In order: Construct new transaction via pointer, set info, set next pointer to head, set head pointer to this transaction
            Transaction* newTransaction = new Transaction(type, oldBalance, balanceChange);
            
            if(head == nullptr){                  //If head is null, set both head and tail to new transaction
                head = newTransaction;
                tail = newTransaction;
            } else {                            //If head is not null, set next pointer of tail to new transaction, set tail to new transaction
                tail -> setNext(newTransaction);
                tail = newTransaction;                
            }
        }

        void displayRecords(){                //Iterate through list via current pointer and call display function from each node -- because head is recorded, this will iterate chronologically
            Transaction* current = head;
            while(current != nullptr){
                current -> displayTransaction();
                current = current -> getNext();
            }
        }
};

class SubAccount{                                           //Class to inherit basic account functions from, includes balance and history, deposit/withdraw/display history functions
    protected:
        TransactionHistory History;
        double balance;

    public:
        SubAccount() : History(), balance(0) {}             //Simple constructor to initialize balance and history

        void showHistory(){                             //Call display function from TransactionHistory class of individual account
            History.displayRecords();
        }

        double getBalance(){                            //Getter for balance
            return balance;
        }

        void deposit(){                                 //Deposit logic -- All accounts currently function as debit accounts, too tired to change that right now
            int depositAmount;
            cout << "\nHow much would you like to deposit? (0 to cancel)\n";

            while(!(cin >> depositAmount) || depositAmount < 0){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid amount. How much would you like to deposit?\n";
            }                                       

            if(depositAmount > 0){
                History.addRecord("Deposit", balance, depositAmount);
                balance += depositAmount;
            }
        }

        virtual void withdraw() = 0;                    //Withdraw function pure virtual, to be overridden in lower classes because they have different limits
};

class CheckingAccount : public SubAccount{                 //Checking account class, doesn't need to do anything but inherit
    public:
        void withdraw(){                            //Allow down to $20 negative balance
            int withdrawAmount;
            cout << "\nHow much would you like to withdraw? (0 to cancel)\n";

            while(!(cin >> withdrawAmount) || withdrawAmount < 0 || withdrawAmount > balance + 20){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid amount. How much would you like to withdraw?\n";
            }

            if(withdrawAmount > 0){
                History.addRecord("Withdrawal", balance, -withdrawAmount);
                balance -= withdrawAmount;
            }
        }
};

class SavingsAccount : public SubAccount{              //Savings account class
    private:
        void savingsInit(){
            balance = 10;
            History.addRecord("Deposit", 0, 10);
        }

    public:
        SavingsAccount() : SubAccount() {           //Extends constructor from SubAccount to provide an initial deposit (not getting rid of this feature)
            savingsInit();
        }

        void withdraw(){                            //Require minimum $10 balance
            int withdrawAmount;
            cout << "\nHow much would you like to withdraw? (0 to cancel)\n";

            while(!(cin >> withdrawAmount) || withdrawAmount < 0 || withdrawAmount > balance - 10){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid amount. How much would you like to withdraw?\n";
            }

            if(withdrawAmount > 0){
                History.addRecord("Withdrawal", balance, -withdrawAmount);
                balance -= withdrawAmount;
            }
        }
};

class BankAccount{              //Account class, includes username, password, checking account, savings account, and next pointer
    private:
        string username;
        string password;
        CheckingAccount checking;
        SavingsAccount savings;
        BankAccount* next;

    public:
        BankAccount(string accountName, string accountPassword) : username(accountName), password(accountPassword), checking(), savings(), next(nullptr) {}       //Constructor for all private members, only takes name/pass

        string getUsername(){                   //Getters for username, password, and next pointer
            return username;
        }

        string getPassword(){
            return password;
        }

        void setPassword(string newPass){
            password = newPass;
        }

        BankAccount* getNext(){
            return next;
        }

        void setNext(BankAccount* newNext){             //Setter for next pointer
            next = newNext;
        }

        void bankingFunctions(){                //Bulk of the program stored here
            while(true){
                cout << "\nWelcome, " << username << "\n";
                cout << "Checking balance: $" << checking.getBalance() << "\n";
                cout << "Savings balance: $" << savings.getBalance() << "\n";
                cout << "Would you like to deposit (D), withdraw (W), view history (H), or logout (X)?\n";             //Gather user choice, then gather account choice before calling deposit/withdraw methods

                string actionChoice;
                getline(cin, actionChoice);

                if(actionChoice == "D" || actionChoice == "d"){
                    cout << "\nWhich account would you like to deposit to? (C for checking, S for savings, X to cancel)\n";
                    string accountChoice;
                    getline(cin, accountChoice);

                    if(accountChoice == "C" || accountChoice == "c"){
                        checking.deposit();

                    } else if(accountChoice == "S" || accountChoice == "s"){
                        savings.deposit();

                    } else if(accountChoice == "X" || accountChoice == "x"){
                        continue;

                    } else {
                        cout << "Invalid account.\n";
                        continue;
                    }

                } else if(actionChoice == "W" || actionChoice == "w"){              //Same as deposit entirely, just will call withdraw instead -- which logically translates to a negative deposit
                    cout << "\nWhich account would you like to withdraw from? (C for checking, S for savings, X to cancel)\n";
                    string accountChoice;
                    getline(cin, accountChoice);

                    if(accountChoice == "C" || accountChoice == "c"){
                        checking.withdraw();

                    } else if(accountChoice == "S" || accountChoice == "s"){
                        savings.withdraw();

                    } else if(accountChoice == "X" || accountChoice == "x"){
                        continue;

                    } else {
                        cout << "Invalid account.\n";
                        continue;
                    }

                } else if(actionChoice == "H" || actionChoice == "h"){              //Display history per account, see TransactionHistory class
                    cout << "\nWhich account would you like to view history for? (C for checking, S for savings, X to cancel)\n";
                    string accountChoice;
                    getline(cin, accountChoice);
                    if(accountChoice == "C" || accountChoice == "c"){
                        checking.showHistory();

                    } else if(accountChoice == "S" || accountChoice == "s"){
                        savings.showHistory();

                    } else if(accountChoice == "X" || accountChoice == "x"){
                        continue;

                    } else {
                        cout << "Invalid account.\n";
                        continue;
                    }

                } else if(actionChoice == "X" || actionChoice == "x"){          //End this menu function and return to login menu
                    return;
                }
            }
        }
};

class AccountList{                                      //Linked list class for bank accounts, includes head/tail pointers, add account function, and display accounts function -- I still think a map was more efficient, but I guess this is simple
    private:
        BankAccount* head = nullptr;
        BankAccount* tail = nullptr;
        int members;                                        //Literally only used one time to see if any accounts exist, might be unnecessary

    public:
        AccountList() : head(nullptr), tail(nullptr), members(0) {}             //Simple constructor for head pointer, set members to 0 for consistency

        ~AccountList(){              //Destructor to delete all nodes in list -- Won't actually get used, so far, at least
            BankAccount* current = head;
            while(current != nullptr){
                BankAccount* next = current -> getNext();
                delete current;
                current = next;
            }
        }

        void addAccount(string username, string password){              //In order: Construct new account via pointer, set info, set next pointer to head, set head pointer to this account, increment members
            BankAccount* newAccount = new BankAccount(username, password);

            if(head == nullptr){                  //If head is null, set both head and tail to new account
                head = newAccount;
                tail = newAccount;

            } else {                            //If head is not null, set next pointer of tail to new account, set tail to new account
                tail -> setNext(newAccount);
                tail = newAccount;                
            }

            members++;
        }

        void deleteAccount(string username, string password){
            BankAccount* current = head;
            BankAccount* previous = nullptr;

            if(head == nullptr){                  //If head is null, list is empty, return
                return;
            }

            if(current -> getUsername() == username && current -> getPassword() == password){       //If head is the account to be deleted, set head to next account(null if N/A) and decrement members
                head = current -> getNext();
                delete current;                                             //This line calls the BankAccount destructor and destroys the chain of all the info relevant to the account
                members--;
                return;
            }

            while(current != nullptr){                                      //Iterate accounts until match is found
                if(current -> getUsername() == username && current -> getPassword() == password){
                    previous -> setNext(current -> getNext());

                    if(current == tail)
                        tail = previous;                     //If tail is the account to be deleted, set tail to previous (null if N/A)
                    delete current;
                    members--;
                    return;
                }

                previous = current;                     //Set previous to current, then set current to next account in list, repeat until current is null (end of list)
                current = current -> getNext();
            }
        }

        void displayAccounts() const{                //Iterate through list via current pointer and call display function from each node -- I could probably sort them alphabetically
            BankAccount* current = head;
            if (members == 0){
                cout << "No accounts exist.\n";
                return;

            } else {
                cout << "\nAccounts:\n";
            }

            while(current != nullptr){
                cout << current -> getUsername() << "\n";
                current = current -> getNext();
            }
        }

        BankAccount* getHead(){
            return head;
        }
};

class Bank{
    private:
        AccountList accounts;                       //Initialize account list

    public:
        bool accountExists(string username){                            //Iterate through list and check if username exists
            BankAccount* current = accounts.getHead();

            while(current != nullptr){
                if(current -> getUsername() == username){
                    return true;
                }

                current = current -> getNext();
            }

            return false;
        }

        void addAccount(string username, string password){              //Create new account by appending to list
            accounts.addAccount(username, password);
        }

        void createAccount(){                   //Looks repetitive, but this method gathers the info that the other one gets called with
            string username, password;

            while(true){                    //Gathers input for username with an exit option. For testing purposes, username/pass are only required to be 3 characters long
                cout << "Username? (minimum 3 characters, X to cancel, spaces allowed)\n";
                getline(cin, username);
                username = stripSpace(username);

                if(username == "X" || username == "x"){
                    return;
                }

                if(username.length() < 3){
                    cout << "Username must be at least 3 characters long.\n";
                    continue;
                }

                if(accountExists(username)){
                    cout << "Username already exists.\n";
                    continue;
                }

                break;
            }

            while(true){                    //Same as above, but for password
                cout << "Password? (minimum 3 characters, X to cancel)\n";
                getline(cin, password);

                if(password == "X" || password == "x"){
                    return;
                }

                if(password.length() < 3){
                    cout << "Password must be at least 3 characters long.\n";
                    continue;
                }

                break;
            }

            addAccount(username, password);
            cout << "\nAccount " << username << " created successfully!\n";
        }

        void updateAccount(){               //Simple account update method
            string username, password, newPass;
            
            while(true){                    //Gathers input for account details with an exit option
                cout << "\nFirst, please login; press X to exit.\n";
                cout << "\nUsername?\n";
                getline(cin, username);
                
                if(username == "X" || username == "x"){
                    return;
                }

                cout << "\nPassword?\n";
                getline(cin, password);

                if(password == "X" || password == "x"){
                    return;
                }

                BankAccount* current = accounts.getHead();

                while(current != nullptr){
                    if(current -> getUsername() == username){
                        if(current -> getPassword() == password){
                            while(true){                    //Gathers input for new password with an exit option
                                cout << "New password? (minimum 3 characters, X to cancel)\n";
                                getline(cin, newPass);
                
                                if(newPass == "X" || newPass == "x"){
                                    return;
                                }
                
                                if(newPass.length() < 3){
                                    cout << "Password must be at least 3 characters long.\n";
                                    continue;
                                }

                                break;
                            }

                            current -> setPassword(newPass);
                            cout << "Password updated successfully!\n";
                            return;
                        }
                    }

                    current = current -> getNext();
                }

                cout << "Account info not found.\n";
            }
        }

        void displayAccounts() const{           //This exists purely for testing purposes, would *never* be part of a finished product -- for an end user, anyway
            accounts.displayAccounts();
        }

        void login(){
            while(true){                    //See the loop in main -- Gathers input for account details with an exit option
                string username, password;
                cout << "\nLogin page; press X to exit.\n";
                cout << "\nUsername?\n";
                getline(cin, username);
                
                if(username == "X" || username == "x"){
                    return;
                }

                cout << "\nPassword?\n";
                getline(cin, password);

                if(password == "X" || password == "x"){
                    return;
                }

                bool accountFound = false;          //Assume account does not exist, iterate through list to find account, use this as a flag

                BankAccount* current = accounts.getHead();

                while(current != nullptr){
                    if(current -> getUsername() == username){
                        accountFound = true;

                        if(current -> getPassword() == password){
                            current -> bankingFunctions();
                            return;

                        } else {
                            cout << "Incorrect password.\n";
                            break;
                        }
                    }

                    current = current -> getNext();
                }

                cout << "Account info not found.\n";
            }
        }
};

int main(){
    Bank bank;
    
    while(true){                        //The initial menu loop -- Sentinel variables are less memory efficient because these loops exit via return statements
        cout << "\nWelcome! Please create an account (C), login (L), update account (U), list accounts (A), or exit (X).\n";
        string mainMenuChoice;
        getline(cin, mainMenuChoice);

        if(mainMenuChoice == "C" || mainMenuChoice == "c"){
            bank.createAccount();

        } else if(mainMenuChoice == "L" || mainMenuChoice == "l"){
            bank.login();

        } else if(mainMenuChoice == "U" || mainMenuChoice == "u"){
            bank.updateAccount();

        } else if(mainMenuChoice == "A" || mainMenuChoice == "a"){
            bank.displayAccounts();
            
        } else if(mainMenuChoice == "X" || mainMenuChoice == "x"){
            return 0;

        } else {
            cout << "Please select a menu choice.\n";
        }
    }

    return 0;
}
