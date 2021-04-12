//
// Created by Eremov Artur B20-02 a.eremov@innopolis.university on 03/03/2021.
//

#include <bits/stdc++.h>
#include <ctime>

using namespace std;


//Enumerator with all access levels
enum AccessLevel{ no_level = 0, green = 1, yellow = 2, red = 3, special = 4, all = 5, blue = 6};

//Enumerator with all room types
enum RoomType{ classRoom = 0, lectureRoom = 1, conferenceRoom = 3, cabinet = 4};


//Simple room class with type and number
class Room{
private:
    RoomType type;
    int number;
    inline static bool emergency;
public:
    Room(RoomType type, int number){
        this->type = type;
        this->number = number;
    }
    static void changeEmergencyState(bool emergencyState)
    {
        emergency = emergencyState;
    }
    int getRoomNumber() {return number;}
    int getAccessLevel() {return type;}
    bool getEmergencyStatus() {return emergency;}

    // Own operator < for inserting to the set<Room>
    friend bool operator<(const Room &obj1, const Room &obj2)
    {
        return obj1.number<obj2.number;
    }
    // Own operator == for checking matching rooms
    friend bool operator==(const Room &obj1, const Room &obj2)
    {
        return obj1.number==obj2.number;
    }
};

void changeEmergency(bool emergencyState)
{
    Room::changeEmergencyState(emergencyState);
    if(emergencyState)
        cout<<"Emergency: all rooms opened"<<endl;
    else
        cout<<"All fine: all rooms closed"<<endl;
}

// Room for checking for the no room
Room nullptrRoom(classRoom, -1);

//Parent class user
class User{
protected:
    string name;
    string surname;
    bool haveOwnCabinet;
    Room ownCabinet = nullptrRoom;
    AccessLevel level;
    set<Room> accessedRooms;
public:
    User(string name, string surname, AccessLevel level = no_level, bool haveOwnCabinet = false, Room cabinet = nullptrRoom)
    {
        this->name = name;
        this->surname = surname;
        this->level = level;
        this->haveOwnCabinet = haveOwnCabinet;
        this->ownCabinet = cabinet;
    }
    User()
    {
        this->name = "";
        this->surname = "";
        this->level = no_level;
        this->haveOwnCabinet = false;
        this->ownCabinet = nullptrRoom;
    }

    //This function check - can someone go to this room number or not
    bool goToTheRoom(Room room)
    {
        // If level of access = 4(special) then this cabinet can be for one person
        // number of such cabinet placed in the user variable
        for(auto it : accessedRooms)
        {
            if(it.getRoomNumber() == room.getRoomNumber())
            {
                cout << name << " " << surname << " is going to the room " << room.getRoomNumber()  << "." <<endl;
                return true;
            }
        }
        if(room.getEmergencyStatus())
        {
            cout << name << " " << surname << " is going to the room " << room.getRoomNumber()  << "." <<endl;
            return true;
        }
        else if(level == 6)
        {
            if(room.getAccessLevel() == 3 || room.getAccessLevel() == 1)
            {
                if(room.getRoomNumber() > 99 && room.getRoomNumber() < 200)
                {
                    cout << name << " " << surname << " is going to the room " << room.getRoomNumber()  << "." << endl;
                    return true;
                }
            }
        }
        else if(room.getAccessLevel() == 4)
        {
            if (haveOwnCabinet) {
                if(ownCabinet == room || level == 5) {
                    cout << name << " " << surname << " is going to the room " << room.getRoomNumber()  << "." << endl;
                    return true;
                }
            }
        }
        // checking level of access of the user
        else if( room.getAccessLevel() <= level)
        {
            cout << name << " " << surname << " is going to the room " << room.getRoomNumber()  << "." <<endl;
            return true;
        }

        cout<<name<<" "<<surname<<" have no access to the room "<<room.getRoomNumber() <<"."<<endl;
        return false;
    }
    //Simple protection of using this function. Only for admins with mark "Level upgrader"
    void changeAccessLevel(string secretWord, AccessLevel level)
    {
        if(secretWord == "Level upgrader") {
            this->level = level;
            cout<<"Level access of "<<name<<" "<<surname<<" upgraded successfully to the "<<level<<"."<<endl;
        }
    }
    void addAccessedRoom(string secretWord, Room sampleRoom)
    {
        if(secretWord == "Room upgrader") {
            accessedRooms.insert(sampleRoom);
            cout<<"Room "<<sampleRoom.getRoomNumber()<< " added for a "<<name<<" "<<surname<<endl;
        }
    }
    //getters to private variables
    string getName(){return name;}
    string getSurname(){return surname;}

    // Own operator < for inserting to the set<User>
    friend bool operator<(const User &obj1, const User &obj2)
    {
        return obj1.level < obj2.level;
    }
};


class Guest : public User{
private:
    string competition;
public:
    Guest(string name, string surname, string competition) : User(name, surname,static_cast<AccessLevel>(6)){
       this->competition = competition;
    }
    void doSomeCompetitionActions() {cout<<name<<" "<<surname<<" is doing some actions related to the: "<<competition<<"."<<endl;}
};

// class student with random gpa and scholarship
// (p.s.) gpa - average of the all grades of all courses
class Student : public User{
private:
    float gpa;
    int scholarship;
public:
    float showGPA(){ cout<<"GPA of "<<name <<" "<<surname<<" is "<<gpa<<endl; return gpa;}
    int showStipa(){ cout<<"Stipa of "<<name<<" "<<surname<<" is "<<scholarship<<"."<<endl; return scholarship;}
    Student(string name, string surname) : User(name, surname,static_cast<AccessLevel>(0)){
        srand (time(NULL));
        gpa = rand() % 5 + 1;
        scholarship = rand() % 36000 + 1;
    }
    //Simple protection of using this function. Only for workers with mark "Scholarship upgrader"
    void changeScholarship(string worker, int amount)
    {
        if(worker == "Scholarship upgrader") {
            scholarship = amount;
            cout<<"Scholarship of "<<name<<" "<<surname<<" upgraded successfully."<<endl;
        }
    }
};

//Professor class which can tell joke and laugh
class Professor : public User{
private:
    string favoriteJoke;
    string favoriteLaughter;
public:
    Professor(string name, string surname, string favoriteJoke, string favoriteLaughter, Room cabinet):
    User(name, surname, static_cast<AccessLevel>(3), true, cabinet){
        this->favoriteJoke = favoriteJoke;
        this->favoriteLaughter = favoriteLaughter;
    }
    void tellTheJoke(){
        cout<<endl<<"\""<<favoriteJoke<<"\" - asked professor "<<surname<<endl;
        cout<<"\""<<favoriteLaughter<<"\" - laugh professor "<<surname<<endl<<endl;
    }
    // also can go to the own cabinet
    bool goToTheOwnCabinet(){ return goToTheRoom(ownCabinet); }
};


// Employee of the University
// at this moment implemented only functions for the "Scholarship upgrader" employee
class Employee : public User{
private:
    string work;
public:
    Employee(string name, string surname, string work) :
    User(name, surname, static_cast<AccessLevel>(1)){
        this->work = work;
    }
    void setAmountOfScholarship(int amount, Student * student)
    {
        if(work == "Scholarship upgrader")
        {
            // We can change the amount of the scholarship
            student->changeScholarship(work,amount);
        }
        else
            cout<<name<<" "<<surname<<" have no access to the changing scholarship of the "<<student->getName()<<" "<<student->getSurname()<<"."<<endl;
    }
    void doSomeWork() {cout<<name<<" "<<surname<<" is doing some work related to the: "<<work<<"."<<endl;}
};

// Simple class for the director
class Director : public User{
private:
    // Count of completed upgrades in the University
    int countOfUpgrades;
    set<string> upgrades;
public:
    Director(string name, string surname, Room cabinet) :
    User(name, surname, static_cast<AccessLevel>(5), true, cabinet){
        countOfUpgrades = 0;
    }
    //Director can create new upgrade of the University
    void newUpgrade(string upgrade)
    {
        cout<<endl;
        countOfUpgrades++;
        upgrades.insert(upgrade);
        cout<<"New upgrade \""<< upgrade << "\" complete!"<<endl<<"You are the best!"<<endl<<endl;
    }
    int showMyCountOfUpgrades(){cout<<"Count of your upgrades = "<<countOfUpgrades<<"."<<endl;return countOfUpgrades;}

    //Show all director upgrades
    int showMyUpgrades(){
        cout<<endl;
        if(countOfUpgrades != 0) {
            cout << "You have " << countOfUpgrades << " completed upgrades." << endl;
            for (auto it : upgrades) {
                cout << it << endl;
            }
        }
        else
            cout<<"You have no upgrades at this University."<<endl<<endl;
        cout<<endl;
        return countOfUpgrades;
    }
    // also like a Professor he can go to his room
    bool goToTheOwnCabinet(){ return goToTheRoom(ownCabinet); }
};

// Simple admin class for changing level of access
class Admin : public User{
private:
    string secretWord = "Level upgrader";
    string secretRoomAccess = "Room upgrader";
public:
    Admin(string name, string surname) :
    User(name, surname,static_cast<AccessLevel>(5),true,nullptrRoom){
    //Admins have OwnCabinetNumber = -1 because they have access to all cabinets
    }
    //change level function for changing access level of some user
    void changeLevel(AccessLevel level, User * user)
    {
        user->changeAccessLevel(secretWord, level);
    }
    //add room for some user
    void addAccessedRoom(Room sampleRoom, User * user)
    {
        user->addAccessedRoom(secretRoomAccess, sampleRoom);
    }
};