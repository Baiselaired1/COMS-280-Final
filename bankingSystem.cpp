#include <iostream>
#include <string>
#include <limits>
#include <vector>

using std::cout, std::cin, std::getline, std::string, std::vector, std::numeric_limits, std::streamsize;         //Namespace directives for simplicity's sake

class Transaction{                          //Simple transaction node class, includes type, balance and alteration, simple constructor, and next pointer
    private:
        Transaction* next;

    public:
        string type;
        double oldBalance;
        double balanceChange;
        double newBalance;
        
        Transaction() : next(nullptr) {}            //Simple constructor for next pointer

        void setInfo(string w, double x, double y){                 //Setter for transaction info, simpler than using constructor
            type = w;
            oldBalance = x;
            balanceChange = y;
            newBalance = oldBalance + balanceChange;
        }

        void displayTransaction() const{                //Method to be called by TransactionHistory class
            cout << "\n***************************************\n";
            cout << "Transaction type: " << type << "\n";
            cout << "\nOld Balance: $" << oldBalance << "\n";
            cout << "Transaction: $" << balanceChange << "\n";
            cout << "End Balance: $" << newBalance << "\n";
        }

        Transaction* getNext() const{
            return next;
        }

        void setNext(Transaction* newNext){
            next = newNext;
        }
};

class TransactionHistory{           //Single link list class for transaction history, includes head pointer, add transaction function, and display history function
    private:
        Transaction* head;
        Transaction* tail;

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

        void addTransaction(string type, double oldBalance, double balanceChange){              //In order: Construct new transaction via pointer, set info, set next pointer to head, set head pointer to this transaction
            Transaction* newTransaction = new Transaction();
            newTransaction -> setInfo(type, oldBalance, balanceChange);
            newTransaction -> setNext(nullptr);
            
            if(head == nullptr){                  //If head is null, set both head and tail to new transaction
                head = newTransaction;
                tail = newTransaction;
            } else {                            //If head is not null, set next pointer of tail to new transaction, set tail to new transaction
                tail -> setNext(newTransaction);
                tail = newTransaction;                
            }
        }

        void displayHistory() const{                //Iterate through list via current pointer and call display function from each node -- because head is recorded, this will iterate chronologically
            Transaction* current = head;
            while(current != nullptr){
                current -> displayTransaction();
                current = current -> getNext();
            }
        }
};

class Account{              //Account class, includes username, password, balance, and transaction history object
    private:
        string username;
        string password;
        double balance;
        TransactionHistory History;

    public:
        Account(string accountName, string accountPassword) : username(accountName), password(accountPassword), balance(0) {}       //Simple constructor opted for over setter method, should probably change the transaction class to make this uniform

        string getUsername() const{
            return username;
        }

        string getPassword() const{
            return password;
        }

        void bankingFunctions(){                //Bulk of the program stored here
            History.addTransaction("Deposit", balance, 10);             //Create a ten dollar deposit as a 'minimum balance requirement' (to create a transaction instance)
            balance += 10;

            while(true){                        //See the loop in main
                cout << "\nWelcome, " << username << "\n";
                cout << "Current balance: $" << balance << "\n";
                cout << "Would you like to deposit (D), withdraw (W), view history (H), or logout (X)?\n";             //Gather user choice

                string accountChoice;
                getline(cin, accountChoice);

                if(accountChoice == "D" || accountChoice == "d"){                     //Create node with user input, with a little error handling, pass balance value BEFORE changing it, clear input buffer -- Also, only process non-zero requests
                    int depositAmount;
                    cout << "\nHow much would you like to deposit? (0 to cancel)\n";

                    while(!(cin >> depositAmount) || depositAmount < 0){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid amount. How much would you like to deposit?\n";
                    }

                    cin.ignore();                       //Because I keep forgetting: cin leaves a newline in the buffer, getline() properly handles this but cannot handle numeric input, and either method might read the newline from the previous input

                    if(depositAmount > 0){
                        History.addTransaction("Deposit", balance, depositAmount);
                        balance += depositAmount;
                    }

                } else if(accountChoice == "W" || accountChoice == "w"){              //Same as deposit, just passes user input as negative and subtracts from balance
                    int withdrawAmount;
                    cout << "\nHow much would you like to withdraw? (0 to cancel)\n";

                    while(!(cin >> withdrawAmount) || withdrawAmount < 0 || withdrawAmount > balance){
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid amount. How much would you like to withdraw?\n";
                    }

                    cin.ignore();

                    if(withdrawAmount > 0){
                        History.addTransaction("Withdrawal", balance, -withdrawAmount);
                        balance -= withdrawAmount;
                    }

                } else if(accountChoice == "H" || accountChoice == "h"){              //Display history, see TransactionHistory class
                    History.displayHistory();

                } else if(accountChoice == "X" || accountChoice == "x"){          //Return to end this menu function and return to login menu
                    return;
                }
            }
        }
};

class Bank{
    private:
        vector<Account> accounts;                       //Vector of account objects, more memory efficient than array or linked list
    
    public:
        bool accountExists(string username){                            //Iterate through vector and check if username exists
            for(size_t i = 0; i < accounts.size(); i++){
                if(accounts[i].getUsername() == username){
                    return true;
                }
            }
            return false;
        }

        void addAccount(string username, string password){              //Create new account while appending to end of vector
            accounts.push_back(Account(username, password));
        }

        void createAccount(){
            string username, password;

            while(true){                    //Gathers input for username with an exit option. For testing purposes, username/pass are only required to be 3 characters long
                cout << "Username? (minimum 3 characters, X to cancel)\n";
                getline(cin, username);

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

                bool accountFound = false;          //Check account exists, then check password, also use this to display error messages

                for(size_t i = 0; i < accounts.size(); i++){        //Iterate through vector, check for supplied username, set bool, check password, call banking functions, return to main menu
                    if(accounts[i].getUsername() == username){
                        accountFound = true;

                        if(accounts[i].getPassword() == password){
                            accounts[i].bankingFunctions();
                            return;

                        } else {
                            cout << "Incorrect password.\n";
                            break;                  //On unsuccessful login, break the for loop but continue the while loop
                        }
                    }
                }

                if(!accountFound){                            //If account not found, display error message and continue while loop
                    cout << "Account does not exist.\n";
                    continue;
                }
            }            
        }
};



int main(){
    Bank bank;
    
    while(true){                        //The initial menu loop -- Sentinel variables are less memory efficient because these loops exit via return statements
        cout << "\nWelcome! Please create an account (C), login (L), or exit (X).\n";
        string mainMenuChoice;
        getline(cin, mainMenuChoice);

        if(mainMenuChoice == "C" || mainMenuChoice == "c"){
            bank.createAccount();

        } else if(mainMenuChoice == "L" || mainMenuChoice == "l"){
            bank.login();

        } else if(mainMenuChoice == "X" || mainMenuChoice == "x"){
            return 0;

        } else {
            cout << "Please select a menu choice.\n";
        }
    }

    return 0;
}