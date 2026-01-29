#include "Rider.h"
#include <iostream>
#include <cmath>

using namespace std;

// ==================== Rider Implementation ====================

Rider::Rider() : id(-1), pickupLocation(-1),
                 dropoffLocation(-1), hasActiveTrip(false)
{
    // Default constructor creates an invalid rider
}

Rider::Rider(int riderId, int pickup, int dropoff)
    : id(riderId), pickupLocation(pickup),
      dropoffLocation(dropoff), hasActiveTrip(false)
{

    // Validate input
    if (riderId < 0)
    {
        cout << "Warning: Rider ID should be non-negative!" << endl;
    }

    if (pickup < 0)
    {
        cout << "Warning: Pickup location ID should be non-negative!" << endl;
    }

    if (dropoff < 0)
    {
        cout << "Warning: Dropoff location ID should be non-negative!" << endl;
    }

    if (pickup == dropoff)
    {
        cout << "Warning: Pickup and dropoff locations are the same!" << endl;
    }

    cout << "Rider " << id << " created with pickup at "
         << pickupLocation << " and dropoff at " << dropoffLocation << endl;
}

Rider::~Rider()
{
    // Simple destructor - no dynamic memory to clean up
    cout << "Rider " << id << " destroyed." << endl;
}

int Rider::getId() const
{
    return id;
}

int Rider::getPickupLocation() const
{
    return pickupLocation;
}

void Rider::setPickupLocation(int location)
{
    if (location < 0)
    {
        cout << "Error: Cannot set negative pickup location!" << endl;
        return;
    }

    int oldLocation = pickupLocation;
    pickupLocation = location;

    cout << "Rider " << id << " changed pickup from "
         << oldLocation << " to " << pickupLocation << endl;
}

int Rider::getDropoffLocation() const
{
    return dropoffLocation;
}

void Rider::setDropoffLocation(int location)
{
    if (location < 0)
    {
        cout << "Error: Cannot set negative dropoff location!" << endl;
        return;
    }

    int oldLocation = dropoffLocation;
    dropoffLocation = location;

    cout << "Rider " << id << " changed dropoff from "
         << oldLocation << " to " << dropoffLocation << endl;
}

bool Rider::hasActiveTripStatus() const
{
    return hasActiveTrip;
}

void Rider::setActiveTripStatus(bool active)
{
    bool oldStatus = hasActiveTrip;
    hasActiveTrip = active;

    cout << "Rider " << id << " active trip status changed from "
         << (oldStatus ? "Active" : "Inactive")
         << " to " << (hasActiveTrip ? "Active" : "Inactive") << endl;
}

void Rider::updateTripRequest(int pickup, int dropoff)
{
    setPickupLocation(pickup);
    setDropoffLocation(dropoff);

    // Reset active trip status when updating request
    if (hasActiveTrip)
    {
        setActiveTripStatus(false);
    }
}

bool Rider::validateTripRequest() const
{
    if (pickupLocation < 0 || dropoffLocation < 0)
    {
        cout << "Invalid trip: Negative location ID!" << endl;
        return false;
    }

    if (pickupLocation == dropoffLocation)
    {
        cout << "Invalid trip: Pickup and dropoff are the same location!" << endl;
        return false;
    }

    if (hasActiveTrip)
    {
        cout << "Invalid trip: Rider already has an active trip!" << endl;
        return false;
    }

    return true;
}

void Rider::printInfo() const
{
    cout << "\n=== Rider Information ===" << endl;
    cout << "Rider ID: " << id << endl;
    cout << "Pickup Location: " << pickupLocation << endl;
    cout << "Dropoff Location: " << dropoffLocation << endl;
    cout << "Active Trip: " << (hasActiveTrip ? "Yes" : "No") << endl;
    cout << "Trip Valid: " << (validateTripRequest() ? "Yes" : "No") << endl;
    cout << "Estimated Distance: " << estimateTripDistance() << " units" << endl;
    cout << "=========================" << endl;
}

int Rider::estimateTripDistance() const
{
    // Simple estimation: absolute difference between locations
    // In real implementation, this would use graph algorithms
    if (pickupLocation < 0 || dropoffLocation < 0)
    {
        return -1; // Invalid locations
    }

    return abs(dropoffLocation - pickupLocation);
}