#include "exerciser.h"

void exercise(connection *C)
{
    cout << "Test cases on query1------------------------------------------------------------------------" << endl;
    query1(C, 0,35,40,0,0,5,0,3,10,0,2,5,0,0.2,1.2,0,0.1,0.5);
    cout << endl;
    query1(C, 1,35,40,0,0,5,0,3,10,0,2,5,0,0.2,1.2,0,0.1,0.5);
    cout << endl;
    query1(C, 1,35,40,0,0,5,0,3,10,0,2,5,0,0.2,1.2,1,0.1,0.5);
    cout << endl;
    cout << "Test cases on query2------------------------------------------------------------------------" << endl;
    query2(C, "DarkBlue");
    cout << endl;
    query2(C, "Red");
    cout << endl;
    query3(C, "Pink");
    cout << endl;
    cout << "Test cases on query3------------------------------------------------------------------------" << endl;
    query3(C, "Duke");
    cout << endl;
    query3(C, "Miami");
    cout << endl;
    query3(C, "Durham");
    cout << endl;
    cout << "Test cases on query4------------------------------------------------------------------------" << endl;
    query4(C, "NC", "DarkBlue");
    cout << endl;
    query4(C, "NC", "Red");
    cout << endl;
    cout << "Test cases on query5------------------------------------------------------------------------" << endl;
    query5(C, 10);
    cout << endl;
    query5(C, 13);
    cout << endl;
    query5(C, 100);
    cout << endl;
}
