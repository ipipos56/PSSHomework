// Artur Eremov B20-02 a.eremov@innopolis.university

/*
 * This code represent simple connectives between university stuff and students
 * In the main function you can find examples of implementing
 */

#include "classes.cpp"

int main()
{
    cout<<endl;

    //Creating students
    Student studentEremov("Artur","Eremov");
    Student studentAbramov("Matvey","Abramov");
    Student studentKiselyov("Ivan","Kiselyov");
    Student studentSmirnov("Konstantine","Smirnov");
    Student studentZhuravlev("Alexander","Zhuravlev");
    Student studentBatyrgariev("Alexander","Batyrgariev");
    Student studentDenislamov("Artur","Denislamov");
    Student studentSakovets("Tasha","Sakovets");
    Student studentGizutdinova("Dzhemilya","Gizutdinova");
    Student studentEvdokimov("Aleksndr","Evdokimov");
    Student studentPetrov("Ignat","Petrov");
    Student studentPlatonov("Nikolai","Platonov");
    Student studentNguen("Chong","Nguen");
    Student studentKrylova("Adel","Krylova");
    Student studentFedorov("Michael","Fedorov");
    Student studentLobanov("Aleksandr","Lobanov");

    //Creating employees
    Employee employeeScholarshipper("Gpa","scholarship","Scholarship upgrader");
    Employee employeeFilippova("Darya","Filippova","Organization of events");
    Employee employeeNizamova("Alisa","Nizamova","Basics of testing");
    Employee employeeNizamovaSister("Alina","Nizamova","Basics of testing");
    Employee employeeBlakunov("Andrey","Blakunov","Organization of club leaders");
    Employee employeeKhismatullina("Karina","Khismatullina","Organization of Dovuz events");
    Employee employeeCleaning("Mop","Cleaning","Cleaning of the University");
    Employee employeeMaster("Screwdriver","Master","Repairing the University");

    //Creating professors' rooms
    Room room407(cabinet,407);
    Room room405(cabinet,405);
    Room room402(cabinet,402);
    Room room409(cabinet,409);

    //Creating professors
    Professor professorZuev("Eugene","Zuev","Lecture is 50% of your work in this course", "hahahahhahah", room407);
    Professor professorKonykhov("Ivan","Konykhov","Is it clear?", "huhu", room405);
    Professor professorKhan("Adil","Khan","In the final you should know all", "hehe", room409);
    Professor professorBahrami("Reza","Bahrami","You can write final today but your grade probably will be C", "hhhh", room402);

    //Creating admins
    Admin adminFirst("Admin","Adminovich");
    Admin adminSecond("Admin2","Adminovich2");

    //Creating directors' room
    Room room202(cabinet,202);

    //Creating conference red rooms
    Room room307(conferenceRoom,307);
    Room room308(conferenceRoom,308);
    Room room309(conferenceRoom,309);

    //Creating class green rooms
    Room room318(classRoom,318);
    Room room317(classRoom,317);
    Room room316(classRoom,316);

    //Creating lecture yellow rooms
    Room room106(lectureRoom,106);
    Room room105(lectureRoom,105);
    Room room107(lectureRoom,107);
    Room room108(lectureRoom,108);

    //Creating a director
    Director director("Director","Directorov", room202);

    //Some directors' actions
    director.goToTheOwnCabinet();
    director.showMyCountOfUpgrades();
    director.newUpgrade("Students should be in the slippers whole day");
    director.showMyUpgrades();
    //Director have access to all rooms
    director.goToTheRoom(room407);
    director.newUpgrade("Students can go home at the 9th of March");
    director.showMyUpgrades();


    //professorKonykhov go to the Lecture room and have access
    professorKonykhov.goToTheRoom(room108);
    //studentEremov go to the Conference room and have no access as default
    // Students have 0(classRoom) access at default
    studentEremov.goToTheRoom(room307);

    //professorKonykhov go to the Conference room and have access as default
    // Professors have 3(conferenceRoom, lectureRoom and classRoom) access as default
    professorKonykhov.goToTheRoom(room308);

    //Students go to the Lecture rooms and have no access
    studentEremov.goToTheRoom(room108);
    studentAbramov.goToTheRoom(room108);
    studentFedorov.goToTheRoom(room108);
    studentZhuravlev.goToTheRoom(room106);
    studentKrylova.goToTheRoom(room107);
    studentPetrov.goToTheRoom(room105);
    studentSakovets.goToTheRoom(room108);
    studentGizutdinova.goToTheRoom(room108);
    studentLobanov.goToTheRoom(room108);

    //Students go to the Class rooms and have access
    studentAbramov.goToTheRoom(room317);
    studentBatyrgariev.goToTheRoom(room317);
    studentDenislamov.goToTheRoom(room318);
    studentEvdokimov.goToTheRoom(room316);
    studentKiselyov.goToTheRoom(room316);
    studentNguen.goToTheRoom(room316);
    studentPlatonov.goToTheRoom(room316);
    studentSmirnov.goToTheRoom(room318);

    //professors tells jokes and laughs
    professorKonykhov.tellTheJoke();
    professorBahrami.tellTheJoke();
    professorKhan.tellTheJoke();
    professorZuev.tellTheJoke();

    //Admins also as director have access to all rooms
    adminFirst.goToTheRoom(room108);
    //Admin change level of Access of StudentEremov to the red
    adminFirst.changeLevel(red, &studentEremov);
    //and studentEremov can open conference room
    studentEremov.goToTheRoom(room307);

    //employees do some work
    employeeBlakunov.doSomeWork();
    employeeCleaning.doSomeWork();
    employeeFilippova.doSomeWork();
    employeeKhismatullina.doSomeWork();
    employeeMaster.doSomeWork();
    employeeNizamova.doSomeWork();
    employeeNizamovaSister.doSomeWork();
    employeeScholarshipper.doSomeWork();

    //students look at their stipa and GPA)
    studentPlatonov.showStipa();
    studentAbramov.showStipa();
    studentSakovets.showStipa();
    studentEremov.showGPA();
    studentEremov.showStipa();

    //some employee want to change the scholarship but cannot do it
    employeeFilippova.setAmountOfScholarship(0,&studentEremov);

    //employeeScholarshipper change the scholarship
    employeeScholarshipper.setAmountOfScholarship(3000,&studentEremov);

    //students look at their stipa and GPA)
    studentPlatonov.showStipa();
    studentAbramov.showStipa();
    studentSakovets.showStipa();
    studentBatyrgariev.showStipa();
    studentBatyrgariev.showGPA();
    studentEremov.showGPA();
    studentEremov.showStipa();

    //professorBahrami go to own cabinet
    professorBahrami.goToTheOwnCabinet();

}