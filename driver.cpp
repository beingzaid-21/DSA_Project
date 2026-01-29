#include "Driver.h"
#include <iostream>

using namespace std;

// ==================== Driver Implementation ====================

Driver::Driver() : id(-1), currentLocation(-1), zoneId(-1), status(DRIVER_OFFLINE)
{
    // Default constructor creates an invalid driver
}

Driver::Driver(int driverId, int locationId, int zone)
    : id(driverId), currentLocation(locationId), zoneId(zone), status(DRIVER_AVAILABLE)
{

    // Validate input
    if (driverId < 0)
    {
        cout << "Warning: Driver ID should be non-negative!" << endl;
    }

    if (locationId < 0)
    {
        cout << "Warning: Location ID should be non-negative!" << endl;
    }

    if (zone < 0)
    {
        cout << "Warning: Zone ID should be non-negative!" << endl;
    }

    cout << "Driver " << id << " created at location "
         << currentLocation << " in zone " << zoneId << endl;
}

Driver::~Driver()
{
    // Simple destructor - no dynamic memory to clean up
    cout << "Driver " << id << " destroyed." << endl;
}

int Driver::getId() const
{
    return id;
}

int Driver::getCurrentLocation() const
{
    return currentLocation;
}

void Driver::setCurrentLocation(int locationId)
{
    if (locationId < 0)
    {
        cout << "Error: Cannot set negative location ID!" << endl;
        return;
    }

    int oldLocation = currentLocation;
    currentLocation = locationId;

    cout << "Driver " << id << " moved from location "
         << oldLocation << " to " << currentLocation << endl;
}

int Driver::getZoneId() const
{
    return zoneId;
}

void Driver::setZoneId(int zoneId)
{
    if (zoneId < 0)
    {
        cout << "Error: Cannot set negative zone ID!" << endl;
        return;
    }

    int oldZone = this->zoneId;
    this->zoneId = zoneId;

    cout << "Driver " << id << " changed zone from "
         << oldZone << " to " << zoneId << endl;
}

DriverStatus Driver::getStatus() const
{
    return status;
}

void Driver::setStatus(DriverStatus newStatus)
{
    DriverStatus oldStatus = status;
    status = newStatus;

    cout << "Driver " << id << " status changed from "
         << statusToString(oldStatus) << " to "
         << statusToString(newStatus) << endl;
}

bool Driver::isAvailable() const
{
    return status == DRIVER_AVAILABLE;
}

void Driver::printInfo() const
{
    cout << "\n=== Driver Information ===" << endl;
    cout << "Driver ID: " << id << endl;
    cout << "Current Location: " << currentLocation << endl;
    cout << "Zone: " << zoneId << endl;
    cout << "Status: " << statusToString(status) << endl;
    cout << "Available: " << (isAvailable() ? "Yes" : "No") << endl;
    cout << "==========================" << endl;
}

const char *Driver::statusToString(DriverStatus status)
{
    switch (status)
    {
    case DRIVER_AVAILABLE:
        return "AVAILABLE";
    case DRIVER_ASSIGNED:
        return "ASSIGNED";
    case DRIVER_ON_TRIP:
        return "ON_TRIP";
    case DRIVER_OFFLINE:
        return "OFFLINE";
    default:
        return "UNKNOWN";
    }
}