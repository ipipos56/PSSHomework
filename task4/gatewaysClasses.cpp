#include "initialClasses.cpp"

class Gateways {
private:
    static json login(const string &userLogin, const string &userPassword, UserType tempUser) {
        ifstream dataOUT;
        int tempIterator = 0;
        string checkGateway = PASSENGERS;
        string tempOutput = "passengers";
        json tempJson;
        if (tempUser == driver) {
            checkGateway = DRIVERS;
            tempOutput = "drivers";
        }
        if (workWithDB::makeIfstream(checkGateway, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    string login = tempJson["login"];
                    string password = tempJson["password"];
                    if (userLogin == login && userPassword == password) {
                        dataOUT.close();
                        if (tempUser == driver) {
                            tempJson["isWorking"] = true;
                            workWithDB::changeLineInAFile(DRIVERS,tempJson["id"],tempJson.dump());
                        }
                        cout<<"You successfully logged into account"<<endl;
                        tempJson["success"] = true;
                        return tempJson;
                    }
                    tempJson["success"] = false;
                }
            }
            cout << "Login or password are incorrect" << endl;
            dataOUT.close();
        } else {
            cout << "We have no " << tempOutput << " yet" << endl;
        }
        return tempJson;
    }

protected:
    static Passenger * passengerCreate(const string &userLogin, const string &userPassword, UserType tempUser)
    {
        json tempJson = login(userLogin,userPassword,tempUser);
        if(tempJson["success"]) {
            Passenger * tempPass;
            tempPass = new Passenger(tempJson["name"], tempJson["surname"], tempJson["id"], tempJson["rating"]);
            return tempPass;
        }
        return nullptr;
    }
    static Driver * driverCreate(const string &userLogin, const string &userPassword, UserType tempUser)
    {
        json tempJson = login(userLogin,userPassword,tempUser);
        if(tempJson["success"]) {
            Driver * tempDriver;
            tempDriver = new Driver(tempJson["name"], tempJson["surname"], tempJson["id"], tempJson["rating"]);
            return tempDriver;
        }
        return nullptr;
    }
public:
    static void endOfProgram()
    {
        ifstream dataOUT;
        json tempJson;
        if (workWithDB::makeIfstream(DRIVERS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if(tempJson["isWorking"])
                    {
                        dataOUT.close();
                        tempJson["isWorking"] = false;
                        workWithDB::changeLineInAFile(DRIVERS,tempJson["id"],tempJson.dump());
                        endOfProgram();
                    }
                }
            }
            dataOUT.close();
        }
    }
    static void timeChanger(int timeToSkip)
    {
        set<json> jsonsChange;
        set<json> :: iterator it;
        ifstream dataOUT;
        json tempJson;
        if (workWithDB::makeIfstream(ORDERS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if(tempJson["status"] != completeMessage || tempJson["passengerCoordinates"] != tempJson["to"])
                    {
                        tempJson["timeSpend"] = (int)tempJson["timeSpend"] + timeToSkip;
                        if(tempJson["passengerCoordinates"] != tempJson["driverCoordinates"]) {
                            int signToRide = sgn((int)tempJson["passengerCoordinates"]
                                    - (int)tempJson["driverCoordinates"]);
                            tempJson["driverCoordinates"] = (int)tempJson["driverCoordinates"] +
                                    timeToSkip/(int)tempJson["carTimeForKM"] * signToRide;
                            if((int)tempJson["timeSpend"] > abs((int)tempJson["driverCoordinates"] - (int)tempJson["from"]))
                                tempJson["driverCoordinates"] = (int)tempJson["passengerCoordinates"];
                        }
                        else if(tempJson["passengerCoordinates"] != tempJson["to"]) {
                            int signToRide = sgn((int)tempJson["to"]
                                                 - (int)tempJson["driverCoordinates"]);
                            tempJson["driverCoordinates"] = (int)((int)tempJson["from"] +
                                    (int)tempJson["timeSpend"]/(int)tempJson["carTimeForKM"] * signToRide);
                            tempJson["passengerCoordinates"] = (int)tempJson["driverCoordinates"];
                            if((signToRide == 1 && (int)tempJson["driverCoordinates"] >
                                                   (int)tempJson["to"]) || signToRide == -1 &&
                                                   (int)tempJson["driverCoordinates"] < (int)tempJson["to"])
                            {
                                tempJson["driverCoordinates"] = (int)tempJson["to"];
                                tempJson["passengerCoordinates"] = (int)tempJson["to"];
                            }
                        }

                        if(tempJson["passengerCoordinates"] != tempJson["driverCoordinates"])
                            tempJson["status"] = driverToPassengerMessage;
                        else if(tempJson["passengerCoordinates"] != tempJson["to"])
                            tempJson["status"] = onARideMessage;
                        else
                            tempJson["status"] = completeMessage;

                        jsonsChange.insert(tempJson);
                    }
                }
            }
            dataOUT.close();
            int sizeOfJsons = jsonsChange.size();
            for(int f = 0;f<sizeOfJsons;f++) {
                it = jsonsChange.begin();
                json tempSetJson = *it;
                json tempDriver = User::getDriver(tempSetJson["idDriver"]);
                json tempCar = Car::getThisCar(tempDriver["carID"]);
                tempCar["coordinates"] = tempSetJson["passengerCoordinates"];
                if(tempSetJson["status"] == completeMessage)
                {
                    if(tempDriver["isOnRide"]) {
                        tempDriver["isOnRide"] = false;
                        workWithDB::changeLineInAFile(DRIVERS, tempDriver["id"], tempDriver.dump());
                    }
                    Passenger::passengersIT = Passenger::passengersSet.begin();
                    for(;Passenger::passengersIT != Passenger::passengersSet.end();Passenger::passengersIT++)
                    {
                        if(Passenger::passengersIT->passengerID == tempSetJson["idPassenger"]
                        && tempSetJson["status"] == completeMessage) {
                            Passenger::passengersIT->setRideParameterInDB(false);
                        }
                    }
                }
                workWithDB::changeLineInAFile(CARS, tempCar["id"], tempCar.dump());
                workWithDB::changeLineInAFile(ORDERS, tempSetJson["id"], tempSetJson.dump());
            }
        }
    }
};


class DriverGateway : public Gateways{
public:
    static Driver * loginDriver(const string& userLogin, const string& userPassword, UserType tempUser)
    {
        return driverCreate(userLogin,userPassword,tempUser);
    }
    static void showOrderHistory(Driver * tempDriver)
    {
        if(tempDriver != nullptr && tempDriver->amILogged())
            tempDriver->showOrdersHistory(tempDriver->getMyTypeOfUser(),tempDriver->getMyID());
        else
            cout<<"You have not logged to an account"<<endl;
    }

    static void answerToAQuestion(Driver *tempDriver, bool answer) {
        if(tempDriver != nullptr && tempDriver->amILogged())
            tempDriver->answerToAQuestion(answer);
        else
            cout<<"You have not logged to an account"<<endl;
    }
    static void parkRightInFrontOfTheEntrance(Driver * tempDriver)
    {
        if(tempDriver != nullptr && tempDriver->amILogged()) {
            int tempid = tempDriver->getMyID();
            json tempDriver = Driver::getDriver(tempid);
            json tempCar = Car::getThisCar(tempDriver["carID"]);
            if(tempCar["type"] == Business.type)
                cout << "Parked right in front of the entrance" << endl;
            else
                cout<<"Only business cars can park right in front of the entrance"<<endl;
        }
        else
            cout<<"You have not logged to an account"<<endl;
    }
};

class PassengerGateway :  public Gateways{
private:
    static void showRidesWithCarType(int from, int to, CarTypeProperties carType)
    {
        ifstream dataOUT;
        json tempJson;
        int countOfRides = 0;
        if (workWithDB::makeIfstream(DRIVERS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if(tempJson["isWorking"] && !tempJson["isOnRide"])
                    {
                        json tempCar = Car::getThisCar(tempJson["carID"]);
                        if(tempCar["type"] == carType.type)
                        {
                            countOfRides++;
                            string driverName = tempJson["name"];
                            string driverSurname = tempJson["surname"];
                            int tempCoordinates = tempCar["coordinates"];
                            int willBeAfter = abs(tempCoordinates - from) * carType.timeForKM;
                            int tempCost = abs(tempCoordinates - from) * carType.priceForKM +
                                    abs(to-from) * carType.priceForKM;
                            int tempTime = abs(to-from) * carType.timeForKM;
                            cout << "(" << countOfRides << ")" << " from coordinates: " << from
                                 << ", to coordinates: " << to<< ", cost: "<<tempCost<<", car will be after "
                                 <<willBeAfter << " min"<<","<<endl<<"    time of ride: "
                                 << tempTime <<" min, with driver: " <<
                                 driverName << " "<<driverSurname<<endl;
                        }
                    }
                }
            }
            dataOUT.close();
        }
        if(countOfRides == 0)
            cout<<"We have no drivers for this moment for this request, wait please:)"<<endl;
    }
public:
    static Passenger * loginPassenger(const string& userLogin, const string& userPassword, UserType tempUser)
    {
        return passengerCreate(userLogin,userPassword,tempUser);
    }
    static void createPaymentMethod(Passenger * tempPassenger, string title, int moneyCount, bool makeDefault = false)
    {
        if(tempPassenger != nullptr && tempPassenger->amILogged())
            tempPassenger->createPaymentMethod(title,moneyCount,makeDefault);
        else
            cout<<"You have not logged to an account"<<endl;
    }
    static void showMyPayments(Passenger * tempPassenger)
    {
        if(tempPassenger != nullptr && tempPassenger->amILogged())
            tempPassenger->showPayments();
        else
            cout<<"You have not logged to an account"<<endl;
    }
    static void changeDefaultPayment(Passenger * tempPassenger, int whichPaymentNeedToChange)
    {
        if(tempPassenger != nullptr && tempPassenger->amILogged())
            tempPassenger->changeDefaultPaymentMethod(whichPaymentNeedToChange);
        else
            cout<<"You have not logged to an account"<<endl;
    }
    static void showOrderHistory(Passenger * tempPassenger)
    {
        if(tempPassenger != nullptr && tempPassenger->amILogged())
            tempPassenger->showOrdersHistory(tempPassenger->getMyTypeOfUser(),tempPassenger->getMyID());
        else
            cout<<"You have not logged to an account"<<endl;
    }
    static void showMyRideCoordinates(Passenger * tempPassenger)
    {
        if(tempPassenger != nullptr && tempPassenger->amILogged())
            tempPassenger->showMyRideCoordinates();
        else
            cout<<"You have not logged to an account"<<endl;
    }

    static void showRidesThatICanTake(Passenger * tempPassenger, int from, int to,const CarTypeProperties properties) {
        if(tempPassenger != nullptr && tempPassenger->amILogged())
            showRidesWithCarType(from,to,properties);
        else
            cout<<"You have not logged to an account"<<endl;
    }

    static void orderTheRide(Passenger * tempPassenger, int from, int to, const CarTypeProperties properties, int numberOfRide, int numberOfQuestion, int whichPayment = -1) {
        if(tempPassenger != nullptr && tempPassenger->amILogged()) {
            if(!tempPassenger->areIInRide())
                tempPassenger->orderTheRide(from, to, properties, numberOfRide,numberOfQuestion, whichPayment);
            else
                cout<<"You already in a ride"<<endl;
        }
        else
            cout<<"You have not logged to an account"<<endl;
    }

    static void createPinnedAddress(Passenger *tempPassenger,string title, int address) {
        if(tempPassenger != nullptr && tempPassenger->amILogged())
            tempPassenger->createPinnedAddress(title,address);
        else
            cout<<"You have not logged to an account"<<endl;
    }

    static void updatePinnedAddress(Passenger *tempPassenger, int number,string title, int address, bool setDelete = false) {
        if(tempPassenger != nullptr && tempPassenger->amILogged())
            tempPassenger->updatePinnedAddress(number,address,title,setDelete);
        else
            cout<<"You have not logged to an account"<<endl;
    }

    static void showPinnedAddresses(Passenger *tempPassenger) {
        if(tempPassenger != nullptr && tempPassenger->amILogged())
            tempPassenger->showAddresses();
        else
            cout<<"You have not logged to an account"<<endl;
    }

    static int askForADriver(Passenger * tempPassenger, int from, int to, const CarTypeProperties properties, int numberOfRide, int whichPayment = -1) {
        if(tempPassenger != nullptr && tempPassenger->amILogged())
            return tempPassenger->askForADriver(from, to, properties, numberOfRide);
        else
            cout<<"You have not logged to an account"<<endl;
    }
};
