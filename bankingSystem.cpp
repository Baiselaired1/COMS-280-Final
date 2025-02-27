#include <iostream>
#include <string>
#include <limits>

//This program is built largely with the book database from last semester as a template

using std::cout, std::endl, std::string;                //Namespace directives for simplicity's sake

class Transaction{                          //Simple transaction node class, includes type, balance and alteration, simple constructor, and next pointer
    private:
        Transaction* next;

    public:
        string type;
        double oldBalance;
        double balanceChange;
        double newBalance;
        
        Transaction() : next(nullptr) {}

        void setInfo(string w, double x, double y){
            type = w;
            oldBalance = x;
            balanceChange = y;
            newBalance = oldBalance + balanceChange;
        }

        void displayTransaction() const{
            cout << "Balance: $" << oldBalance << endl;
            cout << "Transaction: $" << balanceChange << endl;
            cout << "New Balance: $" << newBalance << endl;
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

    public:
        TransactionHistory() : head(nullptr) {}             //Simple constructor for head pointer

        void addTransaction(string type, double oldBalance, double balanceChange){              //In order: Construct new transaction via pointer, set info, set next pointer to head, set head pointer to this transaction
            Transaction* newTransaction = new Transaction();
            newTransaction -> setInfo(type, oldBalance, balanceChange);
            newTransaction -> setNext(head);
            head = newTransaction;
        }

        void displayHistory() const{                //Iterate through list via current pointer and call display function from each node
            Transaction* current = head;
            while(current != nullptr){
                current -> displayTransaction();
                current = current -> getNext();
            }
        }
};

/*class Account{
    // This may or may not become relevant to implement
};*/

int main(){
    using namespace std;

    int balance = 0;                //Master balance variable

    TransactionHistory History;     //List instance

    bool userActive = true;         //Program sentinel

    while(userActive){
        cout << "Current balance: $" << balance << endl;
        cout << "Would you like to deposit (D), withdraw (W), view history (H), or exit (X)? " << endl;             //Gather user choice

        string userChoice;
        getline(cin, userChoice);

        if(userChoice == "D" || userChoice == "d"){                     //Create node with user input, with a little error handling, pass balance value BEFORE changing it, clear input buffer
            int depositAmount;
            cout << "How much would you like to deposit? " << endl;
            while(!(cin >> depositAmount) || depositAmount < 0){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid amount. How much would you like to deposit? " << endl;
            }

            History.addTransaction("Deposit", balance, depositAmount);
            balance += depositAmount;

            cin.ignore();
        } else if(userChoice == "W" || userChoice == "w"){              //Same as deposit, just passes user input as negative and subtracts from balance
            int withdrawAmount;
            cout << "How much would you like to withdraw? " << endl;
            while(!(cin >> withdrawAmount) || withdrawAmount < 0 || withdrawAmount > balance){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid amount. How much would you like to withdraw? " << endl;
            }

            History.addTransaction("Withdrawal", balance, -withdrawAmount);
            balance -= withdrawAmount;

            cin.ignore();
        } else if(userChoice == "H" || userChoice == "h"){              //Display history, see TransactionHistory class
            History.displayHistory();

            cin.ignore();
        } else if(userChoice == "X" || userChoice == "x"){          //Set sentinel to false to kill program
            userActive = false;
        }
    }

    return 0;
}