#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <set>


//JSON library by nlohmann
#include "JsonParser/json.hpp"
using json = nlohmann::json;
//JSON for Modern C++ version 3.9.1
//https://github.com/nlohmann/json

//Json used to save "DataBase" lines in convenient format


using namespace std;

const string PASSENGERS = "../dataBase/passengers.txt";
const string DRIVERS = "../dataBase/drivers.txt";
const string CARS = "../dataBase/cars.txt";
const string ORDERS = "../dataBase/orders.txt";

enum userType {driver = 1,passenger = 2};

void writeToFile(string fileName,string line)
{
    ofstream fileIN;
    fileIN.open(fileName,ios_base::app);
    if(!fileIN.is_open()) {
        cout << "File " << fileName << " does not work to write"<<endl;
        return;
    }
    fileIN<<line<<endl;
    fileIN.close();
}

bool makeIfstream(string fileName, ifstream &fileOUT)
{
    fileOUT.open(fileName,ios_base::out);
    if(!fileOUT.is_open()) {
        //cout << "File " << fileName << " does not work to read"<<endl;
        return false;
    }
    return true;
}

string readFromFile(ifstream &tempFileOUT)
{
    string outLine = "";
    getline(tempFileOUT,outLine);
    return outLine;
}

int countOfLinesInFile(string fileName)
{
    ifstream orderOUT;
    int tempIterator = 0;
    if(makeIfstream(fileName, orderOUT)) {
        string lineOUT = "Message";
        while (lineOUT.size() > 0) {
            lineOUT = readFromFile(orderOUT);
            if (lineOUT.size() > 0) {
                tempIterator++;
            }
        }
        orderOUT.close();
        return tempIterator;
    }
    return 0;
}

class User{
private:
    inline static int allIDs = 0;
    string userLogin;
    string password;
protected:
    int id;
    bool logged;
    string name;
    string surname;
    int rating;
    void createOrder(int idPassenger, int idDriver,
                     string from, string to, int cost, string status){
        json order;
        order["id"] = countOfLinesInFile(ORDERS);
        order["idPassenger"] = idPassenger;
        order["idDriver"] = idDriver;
        order["from"] = from;
        order["to"] = to;
        order["cost"] = cost;
        order["status"] = status;
        writeToFile(ORDERS,order.dump());
    }
public:
    User(string name, string surname, string userLogin, string password){
        this->name = name;
        this->surname = surname;
        this->rating = 0;
        this->logged = false;
        this->userLogin = userLogin;
        this->password = password;
        this->id = allIDs;
        allIDs++;
    }
    void showOrdersHistory()
    {
        if(logged) {
            ifstream orderOUT;
            int tempIterator = 1;
            if (makeIfstream(ORDERS, orderOUT)) {
                string lineOUT = "Message";
                while (lineOUT.size() > 0) {
                    lineOUT = readFromFile(orderOUT);
                    if (lineOUT.size() > 0) {
                        json tempJson = json::parse(lineOUT);
                        int cost = tempJson["cost"];
                        int idPassenger = tempJson["idPassenger"];
                        string from = tempJson["from"];
                        string to = tempJson["to"];
                        int idDriver = tempJson["idDriver"];
                        string status = tempJson["status"];
                        if (idPassenger == this->id)
                            cout << "Order " << tempIterator << " - from: " << from
                                 << ", to: " << to << ", driver: " << idDriver << ", cost: " << cost << ", status: "
                                 << status << endl;
                    }
                }
                orderOUT.close();
            } else
                cout << "You have no orders yet" << endl;
        }
    }
    void orderTheRide()
    {
        if(logged)
        {
            createOrder(this->id + 5, 200, "Almetyevsk","Kazan",4500,"Complete yet");
        }
    }
};

class Passenger : public User {
public:
    friend bool operator<(const Passenger &obj1, const Passenger &obj2) {
        return obj1.name < obj2.name;
    }
    inline static set<Passenger> passengersSet;
    Passenger(string name, string surname, string login, string password)
    : User(name, surname, login, password)
    {
        passengersSet.insert(*this);
    }
};

class Driver : public User{
public:
    friend bool operator<(const Driver &obj1, const Driver &obj2) {
        return obj1.name < obj2.name;
    }
    inline static set<Driver> driversSet;
    Driver(string name, string surname, string login, string password)
    : User(name, surname, login, password)
    {
        driversSet.insert(*this);
    }
};

class Car{

};

class DriverGateway{

};

class PassengerGateway{

};

void login(string userLogin, string password, userType tempUser)
{
    ifstream passengersOUT;
    int tempIterator = 0;
    if (makeIfstream(PASSENGERS, passengersOUT)) {
        string lineOUT = "Message";
        while (lineOUT.size() > 0) {
            lineOUT = readFromFile(passengersOUT);
            if (lineOUT.size() > 0) {
                json tempJson = json::parse(lineOUT);
                string login = tempJson["login"];
                string password = tempJson["password"];
            }
        }
        orderOUT.close();
    }
    else
        cout << "You have no orders yet" << endl;
    if(this->userLogin == userLogin && this->password == password) {
        logged = true;
        cout<<"You successfully loged to the account"<<endl;
        return;
    }
    cout<<"Login or password are incorrect"<<endl;

    //json order;
    //order["id"] = countOfLinesInFile(ORDERS);
    //order["idPassenger"] = idPassenger;
    //order["idDriver"] = idDriver;
    //order["from"] = from;
    //order["to"] = to;
    //order["cost"] = cost;
    //order["status"] = status;
    //writeToFile(ORDERS,order.dump());
}

int main() {
    //!!!!!!!!!!!!!!!!!!!!!!!
    //Set enum to driver or passenger to check different gateways
    userType usr = driver;
    //!!!!!!!!!!!!!!!!!!!!!!!

    login("log","password",usr);
    login("login","pass",usr);
    login("login","password",usr);
    return 0;
}