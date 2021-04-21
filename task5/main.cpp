#include "gatewaysClasses.cpp"

class InitialCreationOfUsers{
public:
    static void createACar(string model, int type, string color, string number, int coordinates, int freeBottleOfWater = -1){
       json car;
        car["id"] = workWithDB::countOfLinesInFile(CARS);
        car["model"] = model;
        car["type"] = type;
        car["color"] = color;
        car["number"] = number;
        car["coordinates"] = coordinates;
        if(type == Comfort.type)
            car["freeBottleOfWater"] = freeBottleOfWater;
        else{
            cout<<"Only Comfort type of cars have BottleOfWater"<<endl;
            car["freeBottleOfWater"] = -1;
        }
        car["deleted"] = false;
        workWithDB::writeToFile(CARS,car.dump());
    }
    static void createAnAccount(string name, string surname, string login, string password, UserType typeOfUser, int carID = -1){
        json user;
        string checkGateway = PASSENGERS;
        if(typeOfUser == driver) {
            checkGateway = DRIVERS;
            user["carID"] = carID;
            user["isWorking"] = false;
            user["countOfRides"] = 0;
        }
        user["isOnRide"] = false;
        user["id"] = workWithDB::countOfLinesInFile(checkGateway);
        user["name"] = name;
        user["surname"] = surname;
        user["login"] = login;
        user["password"] = password;
        user["rating"] = 0;
        user["deleted"] = false;
        workWithDB::writeToFile(checkGateway,user.dump());
    }
};

int main() {
    //InitialCreationOfUsers::createAnAccount("Ken","Dre","login7","password",driver,5);
    //InitialCreationOfUsers::createAnAccount("Tor","Fru","login7","password2",passenger);
    //InitialCreationOfUsers::createACar("Shevrolet Camaro", ComfortPlus,"Yellow","F777FF",3);
    //deleteLineInAFile(CARS,1);


    //Driver gateway
    UserType usr = driver;
    Driver * driver1;
    Driver * driver2;
    Driver * driver3;
    Driver * driver4;
    Driver * driver5;
    cout<<endl;
    driver1 = DriverGateway::loginDriver("log","password",usr);
    driver1 = DriverGateway::loginDriver("login","pass",usr);
    driver1 = DriverGateway::loginDriver("login2","password",usr);
    cout<<endl;
    driver2 = DriverGateway::loginDriver("login6","password",usr);
    driver3 = DriverGateway::loginDriver("login5","password",usr);
    driver4 = DriverGateway::loginDriver("login4","password",usr);
    driver5 = DriverGateway::loginDriver("login","password1",usr);
    cout<<endl;

    DriverGateway::showOrderHistory(driver4);
    cout<<endl;

    //Passenger gateway
    usr = passenger;
    Passenger * passenger1;
    cout<<endl;
    passenger1 = PassengerGateway::loginPassenger("log","password",usr);
    PassengerGateway::changeDefaultPayment(passenger1,2);
    passenger1 = PassengerGateway::loginPassenger("login","pass",usr);
    PassengerGateway::showMyPayments(passenger1);
    passenger1 = PassengerGateway::loginPassenger("login3","password2",usr);
    cout<<endl;

    //PassengerGateway::createPaymentMethod(passenger1,"my second",3000,true);
    //PassengerGateway::createPaymentMethod(passenger1,"may",10000,true);
    PassengerGateway::showMyPayments(passenger1);
    PassengerGateway::changeDefaultPayment(passenger1,2);
    PassengerGateway::showMyPayments(passenger1);
    PassengerGateway::changeDefaultPayment(passenger1,1);
    PassengerGateway::showMyPayments(passenger1);
    PassengerGateway::changeDefaultPayment(passenger1,1);
    PassengerGateway::showMyPayments(passenger1);

    PassengerGateway::showRidesThatICanTake(passenger1,0,5,Economy);
    cout<<endl;
    int numberOfQuestion = PassengerGateway::askForADriver(passenger1,0,5,Economy,1);
    DriverGateway::lookForAvailableRides(driver4, false);
    PassengerGateway::orderTheRide(passenger1,0,5,Economy,1, numberOfQuestion);
    numberOfQuestion = PassengerGateway::askForADriver(passenger1,0,5,Economy,1);
    DriverGateway::answerToAQuestion(driver4, true);
    PassengerGateway::orderTheRide(passenger1,0,5,Economy,1, numberOfQuestion);
    PassengerGateway::orderTheRide(passenger1,0,5,Economy,1,numberOfQuestion,2);
    cout<<endl;
    Gateways::timeChanger(1);
    PassengerGateway::showOrderHistory(passenger1);
    PassengerGateway::showMyRideCoordinates(passenger1);
    cout<<endl;
    Gateways::timeChanger(1);
    PassengerGateway::showOrderHistory(passenger1);
    PassengerGateway::showMyRideCoordinates(passenger1);
    cout<<endl;
    Gateways::timeChanger(3);
    PassengerGateway::showOrderHistory(passenger1);
    PassengerGateway::showMyRideCoordinates(passenger1);
    cout<<endl;
    Gateways::timeChanger(5);
    PassengerGateway::showOrderHistory(passenger1);
    PassengerGateway::showMyRideCoordinates(passenger1);
    cout<<endl;
    Gateways::timeChanger(20);
    PassengerGateway::showOrderHistory(passenger1);
    PassengerGateway::showMyRideCoordinates(passenger1);
    cout<<endl;
    Gateways::timeChanger(20);
    PassengerGateway::showOrderHistory(passenger1);
    PassengerGateway::showMyRideCoordinates(passenger1);
    cout<<endl;

    PassengerGateway::showPinnedAddresses(passenger1);
    PassengerGateway::createPinnedAddress(passenger1,"home",5);
    PassengerGateway::showPinnedAddresses(passenger1);
    PassengerGateway::updatePinnedAddress(passenger1,1,"work",0);
    PassengerGateway::showPinnedAddresses(passenger1);
    PassengerGateway::updatePinnedAddress(passenger1,1,"-1",-1,true);
    PassengerGateway::showPinnedAddresses(passenger1);

    DriverGateway::parkRightInFrontOfTheEntrance(driver4);
    DriverGateway::parkRightInFrontOfTheEntrance(driver5);


    //Need for making Drivers "notWorking" in DRIVERS dataBase
    Gateways::endOfProgram();
    return 0;
}