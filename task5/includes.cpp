#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <set>


//JSON library by nlohmann
#include "JsonParser/json.hpp"
using json = nlohmann::json;
//JSON for Modern C++ version 3.9.1
//https://github.com/nlohmann/json

//Json used to save "DataBase" lines in convenient format

using namespace std;

const string driverToPassengerMessage = "Driver go to a passenger";
const string onARideMessage = "In a ride with passenger";
const string completeMessage = "Complete";
const string requestedMessage = "Requested";
const string declinedMessage = "Declined";

const string PASSENGERS = "../dataBase/passengers.txt";
const string DRIVERS = "../dataBase/drivers.txt";
const string CARS = "../dataBase/cars.txt";
const string ORDERS = "../dataBase/orders.txt";
const string PAYMENTS = "../dataBase/payments.txt";
const string ADDRESSES = "../dataBase/addresses.txt";
const string DRIVERSQUESTIONS = "../dataBase/questions.txt";

enum UserType {driver = 1,passenger = 2};
enum statusOfDriver{working = true, notWorking = false};

class CarTypeProperties{
public:
    int type;
    int priceForKM;
    int timeForKM;
    CarTypeProperties(int type, int priceForKM, int timeForKM)
    {
        this->type = type;
        this->priceForKM = priceForKM;
        this->timeForKM = timeForKM;
    }
};

const CarTypeProperties Economy(1,10,10);
const CarTypeProperties Comfort(2,20,9);
const CarTypeProperties ComfortPlus(3,30,5);
const CarTypeProperties Business(4,40,1);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}