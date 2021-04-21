#include "workWithFiles.cpp"

class Car{
private:
    string model;
    int type;
    string color;
    string number;
    int coordinates;
    int freeBottleOfWater = -1;
public:
    Car(string model, int type, string color, string number, int coordinates, int freeBottleOfwater = -1)
    {
        this->color = color;
        this->coordinates = coordinates;
        this->model = model;
        this->number = number;
        this->type = type;
        if(type == Business.type)
            this->freeBottleOfWater = freeBottleOfwater;
        else
            this->freeBottleOfWater = -1;
    }
    static json getThisCar(int tempCarID)
    {
        ifstream dataOUT;
        json tempJson;
        if (workWithDB::makeIfstream(CARS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if(tempJson["id"] == tempCarID)
                    {
                        tempJson["success"] = true;
                        dataOUT.close();
                        return tempJson;
                    }
                    tempJson["success"] = false;
                }
            }
            dataOUT.close();
        }
        return tempJson;
    }
};

class User{
private:
    inline static int allIDs = 0;
protected:
    int id;
    bool logged;
    string name;
    string surname;
    float rating;
    UserType accountType;
public:
    User(string name, string surname, float rating, UserType accountType){
        this->name = name;
        this->surname = surname;
        this->rating = rating;
        this->logged = false;
        this->id = allIDs;
        this->accountType = accountType;
        allIDs++;
    }
    static json getDriver(int tempDriverID)
    {
        ifstream dataOUT;
        json tempJson;
        if (workWithDB::makeIfstream(DRIVERS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if(tempJson["id"] == tempDriverID)
                    {
                        tempJson["success"] = true;
                        dataOUT.close();
                        return tempJson;
                    }
                    tempJson["success"] = false;
                }
            }
            dataOUT.close();
        }
        return tempJson;
    }
    static json getPassenger(int tempPassengerID)
    {
        ifstream dataOUT;
        json tempJson;
        if (workWithDB::makeIfstream(PASSENGERS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if(tempJson["id"] == tempPassengerID)
                    {
                        tempJson["success"] = true;
                        dataOUT.close();
                        return tempJson;
                    }
                    tempJson["success"] = false;
                }
            }
            dataOUT.close();
        }
        return tempJson;
    }
    void showOrdersHistory(UserType typeOfUser, int userID)
    {
        ifstream orderOUT;
        int tempIterator = 1;
        string idTaker = "idPassenger";
        string idGiver = "idDriver";
        string idMessage = " driver: ";
        if(typeOfUser == driver) {
            idTaker = "idDriver";
            idMessage = " passenger: ";
            idGiver = "idPassenger";
        }
        if (workWithDB::makeIfstream(ORDERS, orderOUT)) {
            string lineOUT = "Message";
            while (lineOUT.size() > 0) {
                lineOUT = workWithDB::readFromFile(orderOUT);
                if (lineOUT.size() > 0) {
                    json tempUser;
                    json tempJson = json::parse(lineOUT);
                    if(typeOfUser == driver)
                        tempUser = getDriver(tempJson[idGiver]);
                    if(typeOfUser == passenger)
                        tempUser = getPassenger(tempJson[idGiver]);
                    string tempName = tempUser["name"];
                    string tempSurname = tempUser["surname"];
                    int idAccount = tempJson[idTaker];
                    string status = tempJson["status"];
                    if (idAccount == userID && !tempJson["deleted"])
                        cout <<"("<<tempIterator<<") "<< "Ride from coordinates: " << tempJson["from"]
                             << ", to coordinates: " << tempJson["to"]
                             << ","<<idMessage<< tempName<<" "<<tempSurname<<endl << "     cost: " << tempJson["cost"] << ", status: "
                             << status << endl;
                    tempIterator++;
                }
            }
            orderOUT.close();
        } else
            cout << "You have no orders yet" << endl;
    }
    bool amILogged(){ return logged;}
    UserType getMyTypeOfUser(){ return accountType;}
};

class Passenger : public User {
private:
    bool changingMoney = false;
    void createOrder(int idDriver,
                     int from, int to, int cost, string status, int driverCoordinates, int timeOfOrder, int timeForKM){
        json order;
        order["id"] = workWithDB::countOfLinesInFile(ORDERS);
        order["idPassenger"] = passengerID;
        order["idDriver"] = idDriver;
        order["from"] = from;
        order["passengerCoordinates"] = from;
        order["driverCoordinates"] = driverCoordinates;
        order["timeOfOrder"] = timeOfOrder;
        order["carTimeForKM"] = timeForKM;
        order["timeSpend"] = 0;
        order["to"] = to;
        order["cost"] = cost;
        order["status"] = status;
        order["deleted"] = false;
        workWithDB::writeToFile(ORDERS,order.dump());
    }
    json askForARide(int from, int to, const CarTypeProperties properties, int numberOfRide)
    {
        ifstream dataOUT;
        json tempJson;
        int countOfRides = 0;
        json outputInfo;
        outputInfo["success"] = false;
        if (workWithDB::makeIfstream(DRIVERS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if(tempJson["isWorking"] && !tempJson["isOnRide"])
                    {
                        json tempCar = Car::getThisCar(tempJson["carID"]);
                        if(tempCar["type"] == properties.type)
                        {
                            countOfRides++;
                            if(countOfRides == numberOfRide) {
                                int tempCoordinates = tempCar["coordinates"];
                                outputInfo["driverID"] = tempJson["id"];
                                outputInfo["success"] = true;
                                outputInfo["driverCoordinates"] = tempCoordinates;
                                outputInfo["willBeAfter"] = abs(tempCoordinates - from) * properties.timeForKM;
                                outputInfo["cost"] = abs(tempCoordinates - from) * properties.priceForKM +
                                                     abs(to - from) * properties.priceForKM;
                                outputInfo["time"] = abs(to - from) * properties.timeForKM;
                                dataOUT.close();
                                return outputInfo;
                            }
                        }
                    }
                }
            }
            dataOUT.close();
        }
        return outputInfo;
    }
    json takeARide(int from, int to, const CarTypeProperties properties, int numberOfRide)
    {
        ifstream dataOUT;
        json tempJson;
        int countOfRides = 0;
        json outputInfo;
        outputInfo["success"] = false;
        if (workWithDB::makeIfstream(DRIVERS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if(tempJson["isWorking"] && !tempJson["isOnRide"])
                    {
                        json tempCar = Car::getThisCar(tempJson["carID"]);
                        if(tempCar["type"] == properties.type)
                        {
                            countOfRides++;
                            if(countOfRides == numberOfRide) {
                                int tempCoordinates = tempCar["coordinates"];
                                outputInfo["driverID"] = tempJson["id"];
                                outputInfo["success"] = true;
                                outputInfo["driverCoordinates"] = tempCoordinates;
                                outputInfo["willBeAfter"] = abs(tempCoordinates - from) * properties.timeForKM;
                                outputInfo["cost"] = abs(tempCoordinates - from) * properties.priceForKM +
                                                     abs(to - from) * properties.priceForKM;
                                outputInfo["time"] = abs(to - from) * properties.timeForKM;
                                tempJson["isOnRide"] = true;
                                dataOUT.close();
                                workWithDB::changeLineInAFile(DRIVERS, tempJson["id"], tempJson.dump());
                                return outputInfo;
                            }
                        }
                    }
                }
            }
            dataOUT.close();
        }
        return outputInfo;
    }
    void giveMoney(int whichPayment, int cost)
    {
        changingMoney = true;
        updatePaymentMethod(whichPayment,(-1)*cost);
    }
    int deleteAnotherDefaultPayment()
    {
        ifstream dataOUT;
        json tempJson;
        bool checked = false;
        int countPayments = 0;
        if (workWithDB::makeIfstream(PAYMENTS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if (tempJson["idPassenger"] == passengerID && !tempJson["deleted"]) {
                        countPayments++;
                        if(tempJson["default"] ) {
                            checked = true;
                            dataOUT.close();
                            tempJson["default"] = false;
                            workWithDB::changeLineInAFile(PAYMENTS, tempJson["id"], tempJson.dump());
                            return countPayments;
                        }
                    }
                }
            }
            if(!checked)
                dataOUT.close();
        }
        return countPayments;
    }
    json countOfPaymentMethods()
    {
        json outputInfo;
        ifstream dataOUT;
        json tempJson;
        int countOfPayments = 0;
        if (workWithDB::makeIfstream(PAYMENTS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if (tempJson["idPassenger"] == passengerID && !tempJson["deleted"]) {
                        countOfPayments++;
                        if(tempJson["default"])
                            outputInfo["default"] = countOfPayments;
                    }
                }
            }
            dataOUT.close();
        }
        outputInfo["count"] = countOfPayments;
        return outputInfo;
    }
    int countOfAddresses()
    {
        json outputInfo;
        ifstream dataOUT;
        json tempJson;
        int countOfPayments = 0;
        if (workWithDB::makeIfstream(PAYMENTS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if (tempJson["idPassenger"] == passengerID && !tempJson["deleted"]) {
                        countOfPayments++;
                    }
                }
            }
            dataOUT.close();
        }
        return countOfPayments;
    }
public:
    int passengerID;
    friend bool operator<(const Passenger &obj1, const Passenger &obj2) {
        return obj1.name < obj2.name;
    }
    inline static set<Passenger> passengersSet;
    inline static set<Passenger> :: iterator passengersIT;
    Passenger(string name, string surname, int passengerID, float rating)
            : User(name, surname, rating,passenger)
    {
        this->passengerID = passengerID;
        logged = true;
        passengersSet.insert(*this);
    }
    void createPaymentMethod(string title, int moneyCount, bool makeDefault = false)
    {
        deleteAnotherDefaultPayment();
        json payment;
        payment["id"] = workWithDB::countOfLinesInFile(PAYMENTS);
        payment["idPassenger"] = passengerID;
        payment["title"] = title;
        payment["moneyCount"] = moneyCount;
        payment["default"] = makeDefault;
        payment["deleted"] = false;
        workWithDB::writeToFile(PAYMENTS, payment.dump());
    }
    void createPinnedAddress(string title, int address)
    {
        json tempAddress;
        tempAddress["id"] = workWithDB::countOfLinesInFile(ADDRESSES);
        tempAddress["idPassenger"] = passengerID;
        tempAddress["title"] = title;
        tempAddress["address"] = address;
        tempAddress["deleted"] = false;
        workWithDB::writeToFile(ADDRESSES, tempAddress.dump());
    }
    void showPayments()
    {
        cout << endl;
        ifstream dataOUT;
        json tempJson;
        int countOfPayments = 1;
        if (workWithDB::makeIfstream(PAYMENTS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if (tempJson["idPassenger"] == passengerID && !tempJson["deleted"]) {
                        string tempTitle = tempJson["title"];
                        cout << "(" << countOfPayments << ")" << " Payment " << tempTitle << ": "
                             << tempJson["moneyCount"] << " rub";
                        if (tempJson["default"])
                            cout << " - default" << endl;
                        else
                            cout << endl;
                        countOfPayments++;
                    }
                }
            }
            dataOUT.close();
        }
        if (countOfPayments == 1)
            cout << "You have no payments" << endl;
        else
            cout << endl;

    }
    void showAddresses()
    {
        cout << endl;
        ifstream dataOUT;
        json tempJson;
        int countOfPayments = 1;
        if (workWithDB::makeIfstream(ADDRESSES, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if (tempJson["idPassenger"] == passengerID && !tempJson["deleted"]) {
                        string tempTitle = tempJson["title"];
                        cout << "(" << countOfPayments << ")" << " Address " << tempTitle << ": coordinates - "
                             << tempJson["address"] << endl;
                        countOfPayments++;
                    }
                }
            }
            dataOUT.close();
        }
        if (countOfPayments == 1)
            cout << "You have no pinned addresses" << endl;
        else
            cout << endl;

    }
    void changeDefaultPaymentMethod(int changePaymentToDefault)
    {
        json countOfPaymentsAll = countOfPaymentMethods();
        if(changePaymentToDefault > 0 && changePaymentToDefault <= countOfPaymentsAll["count"]) {
            if(countOfPaymentsAll["default"] == changePaymentToDefault)
            {
                cout<<"Your default payment method already " <<changePaymentToDefault<<endl;
                cout<<"Nothing changed"<<endl;
                return;
            }
            deleteAnotherDefaultPayment();
            ifstream dataOUT;
            json tempJson;
            int countOfPaymentsCurrent = 0;
            if (workWithDB::makeIfstream(PAYMENTS, dataOUT)) {
                string lineOUT = "Message";
                while (!lineOUT.empty()) {
                    lineOUT = workWithDB::readFromFile(dataOUT);
                    if (!lineOUT.empty()) {
                        tempJson = json::parse(lineOUT);
                        if (tempJson["idPassenger"] == passengerID && !tempJson["deleted"]) {
                            countOfPaymentsCurrent++;
                            if (countOfPaymentsCurrent == changePaymentToDefault) {
                                tempJson["default"] = true;
                                dataOUT.close();
                                workWithDB::changeLineInAFile(PAYMENTS, tempJson["id"], tempJson.dump());
                                cout << "Default payment method successfully changed to " << changePaymentToDefault
                                     << endl;
                                return;
                            }
                        }
                    }
                }
                dataOUT.close();
            }
        }
        else {
            cout << "You do not have payment method " << changePaymentToDefault << endl;
            cout << "Default payment do not changed"<<endl;
        }
    }
    void updatePaymentMethod(int numberOfPayment, int newMoney = -1, string newTitle = "-1", bool setDefault = false)
    {
        json countOfPaymentsAll = countOfPaymentMethods();
        bool changedToDefault = false;
        int paymentIndex = numberOfPayment;
        if((paymentIndex > 0 && paymentIndex <= countOfPaymentsAll["count"]) || changingMoney) {
            if(paymentIndex == -1 && changingMoney)
                paymentIndex = countOfPaymentsAll["default"];
            if(countOfPaymentsAll["default"] == paymentIndex && setDefault)
                changedToDefault = true;
            if(setDefault && !changedToDefault)
                deleteAnotherDefaultPayment();
            ifstream dataOUT;
            json tempJson;
            int countOfPaymentsCurrent = 0;
            if (workWithDB::makeIfstream(PAYMENTS, dataOUT)) {
                string lineOUT = "Message";
                while (!lineOUT.empty()) {
                    lineOUT = workWithDB::readFromFile(dataOUT);
                    if (!lineOUT.empty()) {
                        tempJson = json::parse(lineOUT);
                        if (tempJson["idPassenger"] == passengerID && !tempJson["deleted"]) {
                            countOfPaymentsCurrent++;
                            if (countOfPaymentsCurrent == paymentIndex) {
                                tempJson["default"] = setDefault;
                                if(newTitle != "-1")
                                    tempJson["title"] = newTitle;
                                if(newMoney > -1)
                                    tempJson["moneyCount"] = newMoney;
                                if(changingMoney) {
                                    int moneyCount = tempJson["moneyCount"];
                                    tempJson["moneyCount"] = moneyCount  + newMoney;
                                }
                                dataOUT.close();
                                workWithDB::changeLineInAFile(PAYMENTS, tempJson["id"], tempJson.dump());
                                if(!changingMoney)
                                    cout << "Payment method "<<paymentIndex << " successfully updated" <<endl;
                                else {
                                    changingMoney = false;
                                    cout<<endl;
                                    cout<<"You paid from payment method "<< paymentIndex<<endl;
                                }
                                return;
                            }
                        }
                    }
                }
                dataOUT.close();
            }
        }
        else {
            cout << "You do not have payment method " << numberOfPayment << endl;
            cout << "Payment do not changed"<<endl;
        }
    }
    void updatePinnedAddress(int numberOfPayment, int address = -1, string newTitle = "-1", bool setDelete = false)
    {
        json countOfAll = countOfAddresses();
        bool changedToDefault = false;
        int paymentIndex = numberOfPayment;
        if(paymentIndex > 0 && paymentIndex <= countOfAll) {
            ifstream dataOUT;
            json tempJson;
            int countOfPaymentsCurrent = 0;
            if (workWithDB::makeIfstream(ADDRESSES, dataOUT)) {
                string lineOUT = "Message";
                while (!lineOUT.empty()) {
                    lineOUT = workWithDB::readFromFile(dataOUT);
                    if (!lineOUT.empty()) {
                        tempJson = json::parse(lineOUT);
                        if (tempJson["idPassenger"] == passengerID && !tempJson["deleted"]) {
                            countOfPaymentsCurrent++;
                            if (countOfPaymentsCurrent == paymentIndex) {
                                if(newTitle != "-1")
                                    tempJson["title"] = newTitle;
                                if(address > -1)
                                    tempJson["address"] = address;
                                dataOUT.close();
                                if(!setDelete)
                                    workWithDB::changeLineInAFile(ADDRESSES, tempJson["id"], tempJson.dump());
                                else
                                    workWithDB::deleteLineInAFile(ADDRESSES,tempJson["id"]);
                                cout << "Address "<<paymentIndex << " successfully updated" <<endl;
                                return;
                            }
                        }
                    }
                }
                dataOUT.close();
            }
        }
        else {
            cout << "You do not have payment method " << numberOfPayment << endl;
            cout << "Payment do not changed"<<endl;
        }
    }
    int getMyID(){return passengerID;}
    bool areIInRide(){return getRideParameterInDB();}
    void setRideParameterInDB(bool setValue) const {
        json outputInfo;
        ifstream dataOUT;
        json tempJson;
        if (workWithDB::makeIfstream(PASSENGERS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if (tempJson["id"] == passengerID && !tempJson["deleted"]) {
                        tempJson["isOnRide"] = setValue;
                        dataOUT.close();
                        workWithDB::changeLineInAFile(PASSENGERS,tempJson["id"],tempJson.dump());
                        return;
                    }
                }
            }
            dataOUT.close();
        }
    }
    bool getRideParameterInDB() {
        json outputInfo;
        ifstream dataOUT;
        json tempJson;
        if (workWithDB::makeIfstream(PASSENGERS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if (tempJson["id"] == passengerID && !tempJson["deleted"]) {
                        dataOUT.close();
                        return (bool)tempJson["isOnRide"];
                    }
                }
            }
            dataOUT.close();
        }
    }
    bool checkNumberQuestion(int numberOfQuestion){
        json outputInfo;
        ifstream dataOUT;
        json tempJson;
        int countASK = 0;
        if (workWithDB::makeIfstream(DRIVERSQUESTIONS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    countASK++;
                    if (tempJson["id"] == numberOfQuestion && !tempJson["deleted"]) {
                        dataOUT.close();
                        if(tempJson["status"] == "yes")
                            return true;
                        else
                            return false;
                    }
                }
            }
            dataOUT.close();
        }
    }
    void orderTheRide(int from, int to, const CarTypeProperties properties, int numberOfRide, int numberOfQuestion, int whichPayment)
    {
        if(checkNumberQuestion(numberOfQuestion)) {
            json outputInfo = takeARide(from, to, properties, numberOfRide);
            if (outputInfo["success"]) {
                setRideParameterInDB(true);
                giveMoney(whichPayment, outputInfo["cost"]);
                string tempStatus = driverToPassengerMessage;
                if (outputInfo["willBeAfter"] == 0)
                    tempStatus = onARideMessage;
                createOrder(outputInfo["driverID"], from, to, outputInfo["cost"],
                            tempStatus, outputInfo["driverCoordinates"], outputInfo["time"], properties.timeForKM);
                cout << "You successfully took a ride" << endl;
                cout << "Have a good one" << endl << endl;
            } else
                cout << "You can not take this ride, wait please:)" << endl;
            cout << "or take another if it is exists" << endl;
        }
        cout<<"Driver did not took your order"<<endl;
    }

    void showMyRideCoordinates() {
        if(!getRideParameterInDB())
        {
            cout<<"You not on a ride"<<endl;
            return;
        }
        ifstream dataOUT;
        json tempJson;
        if (workWithDB::makeIfstream(ORDERS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if (tempJson["idPassenger"] == passengerID && !tempJson["deleted"] && tempJson["status"] != completeMessage) {
                        if(tempJson["status"] != completeMessage)
                        {
                            cout<<"Your coordinates is " << (int)tempJson["passengerCoordinates"]<<endl;
                            dataOUT.close();
                            return;
                        }
                    }
                }
            }
            dataOUT.close();
        }
    }

    int askForADriver(int from, int to, const CarTypeProperties properties, int numberOfRide)
    {
        json outputInfo = askForARide(from,to,properties,numberOfRide);
        json question;
        question["id"] = workWithDB::countOfLinesInFile(DRIVERSQUESTIONS);
        question["idPassenger"] = passengerID;
        question["idDriver"] = outputInfo["driverID"];
        question["from"] = from;
        question["to"] = to;
        question["properties"] = properties.type;
        question["status"] = "-1";
        question["deleted"] = false;
        workWithDB::writeToFile(DRIVERSQUESTIONS, question.dump());
        cout<<"Wait while driver accept or decline your order"<<endl;
        return (int)question["id"];
    }
};

class Driver : public User{
private:
    int driverID;
    bool isOnRide;
    bool isWorking;
public:
    friend bool operator<(const Driver &obj1, const Driver &obj2) {
        return obj1.name < obj2.name;
    }
    inline static set<Driver> driversSet;
    inline static set<Driver> :: iterator driversIT;
    Driver(string name, string surname, int driverID, float rating)
            : User(name, surname, rating, driver)
    {
        this->driverID = driverID;
        logged = true;
        isOnRide = false;
        isWorking = false;
        driversSet.insert(*this);
    }
    int getMyID(){return driverID;}

    void answerToAQuestion(bool answer) {
        ifstream dataOUT;
        json tempJson;
        if (workWithDB::makeIfstream(DRIVERSQUESTIONS, dataOUT)) {
            string lineOUT = "Message";
            while (!lineOUT.empty()) {
                lineOUT = workWithDB::readFromFile(dataOUT);
                if (!lineOUT.empty()) {
                    tempJson = json::parse(lineOUT);
                    if (tempJson["idDriver"] == driverID && !tempJson["deleted"] && tempJson["status"] == "-1") {
                        if(answer)
                            tempJson["status"] = "yes";
                        else
                            tempJson["status"] = "no";
                        workWithDB::changeLineInAFile(DRIVERSQUESTIONS,tempJson["id"], tempJson.dump());
                        return;
                    }
                }
            }
            dataOUT.close();
        }
    }
};
