Version of GNU GCC C++ should be 17 or higher

The main program provided in the main.cpp file

To run the code you should compile all files by instruction in the CMakeLists.txt

This code represent simple connectives between drivers and passengers in Taxi. Programm have 2 basic global gateways PassengerGateway and DriverGatevay(different mobile apps)

The city in this program represent the 1 dimentional line(just a simple line with coordinates)
Variables from and to are positive integers and driver can move through positive integers in a line

DataBases are just files. 7 files. addresses - for pinned addresses, cars - for all cars, drivers - for all drivers,
orders - for all accepted orders of rides, passengers - for all passengers, payments - for all payments methods,
questions - for asking a driver(can you take my order and move me through this ride) 

Program have class InitialCreationOfUsers for initial creation of users and cars.

Drivers has name, surname, rating, order history, one car, status(is working or not,
in ride or not). He can login(loginDriver), see order history(showOrderHistory), see a car, update status(automaticaly in rides),
check available order(answerToAQuestion), park Right In Front Of The Entrance if he have Business car,  and answer to request by accepting or declining it.

• A Passenger has name, surname, rating, order history, payment methods, pinned
addresses. He can(with PassengerGate)
– login(loginPassenger), see order history, see(showMyPayments) and update(updatePaymentMethod) payment methods,
update default payment methods(changeDefaultPayment), see(showPinnedAddresses)
, create(createPinnedAddress) and update(updatePinnedAddress) pinned addresses.
– select any two addresses (from, to) and carType to check the time
and price of the ride.(showRidesThatICanTake)
- ask for a ride to a driver(driver can answer to this request) (askForADriver)
– order the ride if he agree with conditions (there is possibility to
change default payment method for this particular ride). Money will be taken automaticaly. (orderTheRide)
– ask for the current coordinates of the car during the ride. (showMyRideCoordinates)
– see the ride in the order history after the end of the ride. (showOrderHistory)

There are 4 car types: Economy, Comfort,
ComfortPlus, Business.
A Car has model, carType, current coordinates, the color and the number. If the type is Comfort it has count of freeBottleOfwater. 
If the type is Business it has the method parkRightInFrontOfTheEntrance which declared in the DriverGateway.

In the main.cpp file in the main function I provided some basic actions.(if you change some information in the DataBase files, please after changing put one empty line at the end of file)
