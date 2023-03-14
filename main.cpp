#include <iostream>
#include <pthread.h>
#include <fstream>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <map>
#include <chrono>
#include <thread>



pthread_mutex_t vm[10]; // mutex lock for machines
pthread_mutex_t total; // mutex lock for transactions
pthread_mutex_t printFile; // mutex lock for printing
pthread_mutex_t kevin; // mutex lock for companies
pthread_mutex_t bob;
pthread_mutex_t otto;
pthread_mutex_t dave;
pthread_mutex_t stuart;

pthread_t vendingMachine[10];

int totalArray[300][5]; // the array for customers' infos

int machineArray[10]; //customers who uses the machines, if machine is idle, value is 0
int kevinGain; // gainings of companies
int bobGain;
int stuartGain;
int ottoGain;
int daveGain;
int inputLength; // will be readed from file
int totalNum; // total transactions

std::string fileName; // actual file name
std::string woFileName; //.txt is excluded from file name
std::ofstream myFile; // output file

std::map<std::string, int> my_map = { 
    { "Kevin", 0 },
    { "Bob", 1 },
    { "Stuart", 2 },
    { "Otto", 3 },
    { "Dave", 4 }
};

std::map<int, std::string> my_map2 = {
    { 0,"Kevin" },
    { 1,"Bob" },
    { 2,"Stuart"},
    { 3,"Otto"},
    { 4,"Dave"}
};


void* machineFunction(void* num) {
    int a = *((int*) num);
    a=a-1; //index in the list for machineArray
    while(totalNum!=inputLength || totalNum==0) { /*inputLength and totalNum is firstly 0, so need to check it is not 0. 
    If num of transactions equal to num of customers the work is finished.*/
        if (machineArray[a]!=0) { //if there is any customer, transaction starts
            pthread_mutex_lock(&printFile);
            std::string res="[VTM"+std::to_string(totalArray[machineArray[a]-1][1])+"]: Customer"+std::to_string(totalArray[machineArray[a]-1][4])+","+std::to_string(totalArray[machineArray[a]-1][3])+"TL,"+my_map2[totalArray[machineArray[a]-1][2]]+"\n";
            myFile<<res;
            pthread_mutex_unlock(&printFile);
            pthread_mutex_lock(&total);
            totalNum+=1; // transactions are incremented by one.
            pthread_mutex_unlock(&total);
            if (totalArray[machineArray[a]-1][2]==0) { //companies receives the gaining
                pthread_mutex_lock(&kevin);
                kevinGain+=totalArray[machineArray[a]-1][3];
                pthread_mutex_unlock(&kevin);
            }
            else if (totalArray[machineArray[a]-1][2]==1) {
                pthread_mutex_lock(&bob);
                bobGain+=totalArray[machineArray[a]-1][3];
                pthread_mutex_unlock(&bob);
            }
            else if (totalArray[machineArray[a]-1][2]==2) {
                pthread_mutex_lock(&stuart);
                stuartGain+=totalArray[machineArray[a]-1][3];
                pthread_mutex_unlock(&stuart);
            }
            else if (totalArray[machineArray[a]-1][2]==3) {
                pthread_mutex_lock(&otto);
                ottoGain+=totalArray[machineArray[a]-1][3];
                pthread_mutex_unlock(&otto);
            }
            else if (totalArray[machineArray[a]-1][2]==4) {
                pthread_mutex_lock(&dave);
                daveGain+=totalArray[machineArray[a]-1][3];
                pthread_mutex_unlock(&dave);
            }
            machineArray[a]=0; //machine is idle now
        }
    }
    pthread_exit(NULL);    
    
    
}

void* customerFunction(void* list1) {
    int* aList=(int*)list1;
    int sleepVal=aList[0];
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepVal)); //the customer sleeps
    pthread_mutex_lock(vm+aList[1]-1);
    machineArray[aList[1]-1]=aList[4]; //machine starts to work after this line
    while (machineArray[aList[1]-1]!=0) {
        sleep(0.001);
    } //customer waits for transaction, and loop is ended after transaction is done.
    pthread_mutex_unlock(vm+aList[1]-1); // machine is ready for another customer
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    totalNum=0;
    fileName=argv[1];
    woFileName = fileName.substr(0, fileName.size()-4)+"_log.txt"; //output file name 
    std::ifstream inputFile(fileName);
    std::string myIL;
    std::string inputLine;
    std::string vendingMach;
    std::string comAmount;
    std::string sleepPeriod;
    std::string company;
    int iSleep;
    int iVending;
    int iAmount;
    int iCompany;
    int customerId;
    pthread_mutex_init(&total, NULL); //initialiazing mutexes
    pthread_mutex_init(&kevin, NULL);
    pthread_mutex_init(&otto, NULL);
    pthread_mutex_init(&dave, NULL);
    pthread_mutex_init(&stuart, NULL);
    pthread_mutex_init(&bob, NULL);
    pthread_mutex_init(&printFile, NULL);
    for (int i=0; i<10; i++) {
        pthread_mutex_init(vm+i, NULL);
        machineArray[i]=0;
    }
    
    int index1=1;
    int index2=2;
    int index3=3;
    int index4=4;
    int index5=5;
    int index6=6;
    int index7=7;
    int index8=8;
    int index9=9;
    int index10=10;
    myFile.open (woFileName);
    pthread_create(vendingMachine+1, NULL, machineFunction, (void *) &index2); //starting machines wiht their ids
    pthread_create(vendingMachine+2, NULL, machineFunction, (void *) &index3);
    pthread_create(vendingMachine+3, NULL, machineFunction, (void *) &index4);
    pthread_create(vendingMachine+4, NULL, machineFunction, (void *) &index5);
    pthread_create(vendingMachine+5, NULL, machineFunction, (void *) &index6);
    pthread_create(vendingMachine+6, NULL, machineFunction, (void *) &index7);
    pthread_create(vendingMachine+7, NULL, machineFunction, (void *) &index8);
    pthread_create(vendingMachine+8, NULL, machineFunction, (void *) &index9);
    pthread_create(vendingMachine+9, NULL, machineFunction, (void *) &index10);
    pthread_create(vendingMachine+0, NULL, machineFunction, (void *) &index1);
    
    
    getline (inputFile, myIL);
    inputLength =std::stoi(myIL);

    pthread_t customers[inputLength];

    
    for (int i=0; i<inputLength; i++) {
        getline (inputFile, inputLine);
        std::stringstream sInputLine(inputLine);
        getline (sInputLine, sleepPeriod, ',');
        getline (sInputLine, vendingMach, ',');
        getline (sInputLine, company, ',');
        getline (sInputLine, comAmount, ',');
        iSleep=std::stoi(sleepPeriod);
        iVending=std::stoi(vendingMach);
        iAmount=std::stoi(comAmount);
        customerId=i+1;
        iCompany=my_map[company];

        totalArray[i][0]=iSleep; //inputs are put into array of array
        totalArray[i][1]=iVending;
        totalArray[i][2]=iCompany;
        totalArray[i][3]=iAmount;
        totalArray[i][4]=customerId;    
        
    }
    for (int i=0; i<inputLength; i++) {
        pthread_create(customers+i, NULL, customerFunction, (void*) totalArray[i]); //each customer receives its input
    }
    for (int i=0; i<inputLength; i++) {
        pthread_join(customers[i], NULL);
    }
    for (int i=0; i<10; i++) {
        pthread_join(vendingMachine[i], NULL);
    }
    myFile<<"[Main]: All payments are completed\n"; //output
    myFile<<"[Main]: Kevin: "<<kevinGain<<"\n";
    myFile<<"[Main]: Bob: "<<bobGain<<"\n";
    myFile<<"[Main]: Stuart: "<<stuartGain<<"\n";
    myFile<<"[Main]: Otto: "<<ottoGain<<"\n";
    myFile<<"[Main]: Dave: "<<daveGain<<"\n";
    myFile.close();

   
}