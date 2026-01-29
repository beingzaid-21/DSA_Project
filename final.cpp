 #include <iostream>
#include "Citydj.h"
#include "Driver.h"
#include "Rider.h"
#include "Trip.h"
#include "DispatchEngine.h"
using namespace std;
void setupCity(City &city)
{
    city.addLocation(0);
    city.addLocation(1);
    city.addLocation(2);
    city.addLocation(3);
    city.addLocation(4);
    city.addLocation(5);

    city.addRoad(0, 1, 5);
    city.addRoad(0, 2, 10);
    city.addRoad(1, 3, 3);
    city.addRoad(2, 3, 7);
    city.addRoad(3, 4, 4);
    city.addRoad(4, 5, 6);

    city.setZone(0, 1);
    city.setZone(1, 1);
    city.setZone(2, 2);
    city.setZone(3, 2);
    city.setZone(4, 3);
    city.setZone(5, 3);
}

int main()
{
    City city;
    setupCity(city);

    DispatchEngine engine(&city);

    // Register drivers (system side)
    engine.registerDriver(new Driver(101, 0, 1));
    engine.registerDriver(new Driver(102, 2, 2));
    engine.registerDriver(new Driver(103, 4, 3));

    int choice;
    int riderIdCounter = 500;
    int pickup;
    int dropoff;
    while (true)
    {
        cout << "\n===== Ride Sharing System =====\n";
        cout << "1. Request Ride\n";
        cout << "2. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 2)
            break;

        if (choice == 1)
        {
            const int MIN_LOC = 0;
            const int MAX_LOC = 5;

            cout << "Enter pickup location (0-5): ";
            cin >> pickup;

            if (pickup < MIN_LOC || pickup > MAX_LOC)
            {
                cout << "❌ Invalid pickup location. Must be between 0 and 5.\n";
                continue;
            }

            cout << "Enter dropoff location (0-5): ";
            cin >> dropoff;

            if (dropoff < MIN_LOC || dropoff > MAX_LOC)
            {
                cout << "❌ Invalid dropoff location. Must be between 0 and 5.\n";
                continue;
            }

            Rider rider(riderIdCounter++, pickup, dropoff);

            Trip *trip = engine.requestTrip(rider);

            if (!trip)
            {
                cout << "❌ No route available.\n";
                continue;
            }

            cout << "\n🚕 Trip Created Successfully!\n";
            cout << "Trip ID: " << trip->getId() << endl;
            cout << "Fare: Rs. " << trip->getFare() << endl;
            cout << "Status: " << Trip::stateToString(trip->getState()) << endl;
        }
    }

    cout << "\nThank you for using the system!\n";
    return 0;
}
