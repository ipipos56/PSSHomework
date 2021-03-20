Version of GNU GCC C++ should be 17 or higher

The main program provided in the main.cpp and classes.cpp files 

To run the code you should compile the main.cpp and classes.cpp files by instruction in the CMakeLists.txt

This code represent simple connectives between university stuff and students. In the main function in the file main.cpp you can find examples of implementing it.
The code have different classes of University members: guest, professor, director, admin, employee and student. All users have name, surname and level of access to the rooms(no_level, blue, green, yellow, red, special, all). Rooms have different types such as: classRoom, lectureRoom, conferenceRoom, cabinet. All University members can access to the rooms for which they have access.

Students can look their schoolarship and GPA. Professors can tell the joke and laugh. Employees can do their work, and "Scholarship" employee can change the scholarship of students. Director and Admins can go to the all rooms. Director can do upgrades to the University and look through all updates and their count. Admins can change the access level of each University member and also can give access to any room. Guests can access only to lecture and conference rooms in the first floor and also can do their actions related to the competitions(olympiad, project schools or some other competitions).

University can set emergency. In case of an emergency, all users have access to all rooms.

In the main.cpp file in the main function I provided some actions of 16 students, 8 employees, 4 professors, 2 admins, 2 guests and 1 director.
