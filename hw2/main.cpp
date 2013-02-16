#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

using namespace std;

struct params
{
    string name;
    string file;
};

void * FatherThread(void *ptr);
void * MotherThread(void *ptr);
void * ChildThread(void *ptr);
void banking(params parameters);

bool allThreadsCreated = false;
bool fatherDone = false;
bool motherDone = false;
bool childDone = false;

int checking = 0;
int savings = 0;

pthread_mutex_t savingsMutex;
pthread_mutex_t checkingMutex;
pthread_mutex_t processingMutex;

int main (int argc, char * const argv[]) {

    // Only proceed if we have the proper number of arguments
    if (argc == 4){

        // Create the parameters to pass to the threads
        params fatherParams;
        params motherParams;
        params childParams;

        // Give each parameter a name
        fatherParams.name = "Bruce";
        motherParams.name = "Jennier";
        childParams.name = "Jill";

        // Populate the file for each thread
        fatherParams.file = argv[1];
        motherParams.file = argv[2];
        childParams.file = argv[3];

        // Create the mutex's and add them to the params
        pthread_mutex_t savingsMutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_t checkingMutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_t processingMutex = PTHREAD_MUTEX_INITIALIZER;

        // Declare threads
        pthread_t fatherThread;
        pthread_t motherThread;
        pthread_t childThread;

        // Create threads
        if (pthread_create(&fatherThread, NULL, FatherThread, &fatherParams) != 0){
            cout << "Error creating father thread.";
        }
        else if (pthread_create(&motherThread, NULL, MotherThread, &motherParams) != 0){
            cout << "Error creating mother thread.";
        }
        else if (pthread_create(&childThread, NULL, ChildThread, &childParams) != 0){
            cout << "Error creating child thread.";
        }
        else{
            // All threads have been created successfully
            allThreadsCreated = true;

            pthread_join(fatherThread, NULL);
            pthread_join(motherThread, NULL);
            pthread_join(childThread, NULL);

            while(!fatherDone || !motherDone || !childDone) pthread_yield();
            cout << "\nFinal checking balance is " << checking << " dollars; final savings balance is " << savings << " dollars.\n";
        }
    }
    else {
        // There is an incorrect number of arguments.  Output error.
        cout << "Please ensure three valid input .txt's have been supplied as arguments.\n";
    }
    return 0;
}

void * FatherThread(void *ptr){
    while(!allThreadsCreated) pthread_yield();
    struct params *reformedstruct;
    reformedstruct = (struct params *) ptr;
    params parameters = *reformedstruct;
    banking(parameters);
    fatherDone = true;
    pthread_exit(ptr);
}

void * MotherThread(void *ptr){
    while(!allThreadsCreated) pthread_yield();
    struct params *reformedstruct;
    reformedstruct = (struct params *) ptr;
    params parameters = *reformedstruct;
    banking(parameters);
    motherDone = true;
    pthread_exit(ptr);
}

void * ChildThread(void *ptr){
    while(!allThreadsCreated) pthread_yield();
    struct params *reformedstruct;
    reformedstruct = (struct params *) ptr;
    params parameters = *reformedstruct;
    banking(parameters);
    childDone = true;
    pthread_exit(ptr);
}

void banking(params parameters){
    ifstream ifs;
    ifs.open(parameters.file.c_str());
    if (ifs.fail()){
        cout << "Could not open file " << parameters.file << "\n";
    }
    else{
        srand(time(NULL));
        char character = 0;
        int value = 0;

        while(!ifs.eof()){
            pthread_mutex_lock(&processingMutex);
            ifs >> character;
            if (character == 'd'){
                // Deposit
                ifs >> character;
                if (character == '0'){
                    // Checking
                    ifs >> value;
                    pthread_mutex_lock(&checkingMutex);
                    cout << parameters.name << " wants to deposit " << value << " dollars into checking.\n";
                    checking += value;
                    cout << "\tDeposit successful.\n";
                    cout << "\tChecking balance is now " << checking << " dollars.\n";
                    pthread_mutex_unlock(&checkingMutex);
                }
                else if (character == '1'){
                    // Savings
                    ifs >> value;
                    pthread_mutex_lock(&savingsMutex);
                    cout << parameters.name << " wants to deposit " << value << " dollars into savings.\n";
                    savings += value;
                    cout << "\tDeposit successful.\n";
                    cout << "\tSavings balance is now " << savings << " dollars.\n";
                    pthread_mutex_unlock(&savingsMutex);
                }
                else {
                    cout << "File formatted improperly.\n";
                    break;
                }
            }
            else if (character == 'w'){
                // Withdraw
                ifs >> character;
                if (character == '0'){
                    // Checking
                    ifs >> value;
                    pthread_mutex_lock(&checkingMutex);
                    cout << parameters.name << " wants to withdraw " << value << " dollars from checking.\n";
                    if (value>checking){
                        pthread_mutex_lock(&savingsMutex);
                        if(savings >= checking-value && savings > 0){
                            savings += (checking-value);
                            checking = 0;
                            cout << "\tWithdrawal successful.  Checking was overspent - the deficit has been taken out of savings.\n";
                            cout << "\tChecking balance is now " << checking << " dollars, and savings balance is now " << savings << " dollars.\n";
                        }
                        else{
                            cout << "\tWithdrawal unsuccessful.  There is not enough money in savings and checking to process transaction.\n";
                            cout << "\tChecking balance is still " << checking << " dollars, and savings balance is still " << savings << " dollars.\n";
                        }
                        pthread_mutex_unlock(&savingsMutex);
                    }
                    else{
                        checking += -value;
                        cout << "\tWithdrawal successful.\n";
                        cout << "\tChecking balance is now " << checking << " dollars.\n";
                    }
                    pthread_mutex_unlock(&checkingMutex);
                }
                else if (character == '1'){
                    // Savings
                    ifs >> value;
                    pthread_mutex_lock(&savingsMutex);
                    cout << parameters.name << " wants to withdraw " << value << " dollars from savings.\n";
                    if (savings > 0){
                        savings += -value;
                        cout << "\tWithdrawal successful.\n";
                        cout << "\tSavings balance is now" << savings << " dollars.\n";
                    }
                    else{
                        cout << "\tWithdrawal unsuccessful. Savings is already negative.\n";
                        cout << "\tSavings balance is still" << savings << " dollars.\n";
                    }
                    pthread_mutex_unlock(&savingsMutex);
                }
                else {
                    cout << "File formatted improperly.\n";
                    break;
                }
            }
            else if (character == 't'){
                // Transfer
                ifs >> character;
                if (character == '0'){
                    // Checking
                    ifs >> value;
                    pthread_mutex_lock(&savingsMutex);
                    pthread_mutex_lock(&checkingMutex);
                    cout << parameters.name << " wants to transfer " << value << " dollars from savings into checking.\n";
                    if (savings > 0){
                        checking += value;
                        savings += -value;
                        cout << "\tTransfer successful.\n";
                        cout << "\tChecking balance is now " << checking << " dollars, and savings balance is now " << savings << " dollars.\n";
                    }
                    else{
                        cout << "\tTransfer unsuccessful.  Savings is already negative.\n";
                        cout << "\tChecking balance is still " << checking << " dollars, and savings balance is still " << savings << " dollars.\n";
                    }
                    pthread_mutex_unlock(&savingsMutex);
                    pthread_mutex_unlock(&checkingMutex);
                }
                else if (character == '1'){
                    // Savings
                    ifs >> value;
                    pthread_mutex_lock(&savingsMutex);
                    pthread_mutex_lock(&checkingMutex);
                    cout << parameters.name << " wants to transfer " << value << " dollars from checking into savings.\n";
                    if (checking > 0){
                        savings += value;
                        checking += -value;
                        cout << "\tTransfer successful.\n";
                        cout << "\tChecking balance is now " << checking << " dollars, and savings balance is now " << savings << " dollars.\n";
                    }
                    else{
                        cout << "\tTransfer unsuccessful.  Checking account is already negative.\n";
                        cout << "\tChecking balance is still " << checking << " dollars, and savings balance is still " << savings << " dollars.\n";
                    }
                    pthread_mutex_unlock(&savingsMutex);
                    pthread_mutex_unlock(&checkingMutex);
                }
                else {
                    cout << "File formatted improperly.\n";
                    break;
                }
            }
            else{
                cout << "File formatted improperly.\n";
                break;
            }
            pthread_mutex_unlock(&processingMutex);
            usleep((rand()%2500)+1);
        }
    }
}
