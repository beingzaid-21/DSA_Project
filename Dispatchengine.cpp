#include "DispatchEngine.h"
#include <iostream>
#include <climits>

using namespace std;

// Initialize static constants
const int DispatchEngine::DEFAULT_SAME_ZONE_BONUS = -10;   // Bonus for same zone
const int DispatchEngine::DEFAULT_CROSS_ZONE_PENALTY = 20; // Penalty for different zone

// Initial capacities
const int INITIAL_DRIVER_CAPACITY = 10;
const int INITIAL_TRIP_CAPACITY = 10;
const int INITIAL_RIDER_CAPACITY = 10;

// ==================== DispatchEngine Implementation ====================

DispatchEngine::DispatchEngine(City *cityPtr)
    : city(cityPtr), driverCount(0), tripCount(0), riderCount(0), nextTripId(1000)
{

    if (cityPtr == nullptr)
    {
        cout << "Warning: DispatchEngine created with null city pointer!" << endl;
    }

    // Initialize drivers array
    driverCapacity = INITIAL_DRIVER_CAPACITY;
    drivers = new Driver *[driverCapacity];
    for (int i = 0; i < driverCapacity; i++)
    {
        drivers[i] = nullptr;
    }

    // Initialize trips array
    tripCapacity = INITIAL_TRIP_CAPACITY;
    trips = new Trip *[tripCapacity];
    for (int i = 0; i < tripCapacity; i++)
    {
        trips[i] = nullptr;
    }

    // Initialize riders array
    riderCapacity = INITIAL_RIDER_CAPACITY;
    riders = new Rider *[riderCapacity];
    for (int i = 0; i < riderCapacity; i++)
    {
        riders[i] = nullptr;
    }

    cout << "DispatchEngine initialized successfully! Next trip ID: " << nextTripId << endl;
}

DispatchEngine::~DispatchEngine()
{
    // Note: We don't delete drivers, riders, or trips here as they're managed elsewhere
    delete[] drivers;
    delete[] trips;
    delete[] riders;
    cout << "DispatchEngine destroyed." << endl;
}

bool DispatchEngine::assignDriverToTrip(int tripId, int driverId)
{
    Trip *trip = findTripById(tripId);
    Driver *driver = findDriverById(driverId);

    if (!validateAssignment(trip, driver))
    {
        return false;
    }

    // Assign driver to trip
    if (trip->assignDriver(driverId))
    {
        // Update driver status
        driver->setStatus(DRIVER_ASSIGNED);
        cout << "Driver " << driverId << " successfully assigned to trip " << tripId << endl;
        return true;
    }

    return false;
}

bool DispatchEngine::startTrip(int tripId)
{
    Trip *trip = findTripById(tripId);

    if (trip == nullptr)
    {
        cout << "Error: Trip " << tripId << " not found!" << endl;
        return false;
    }

    // Start the trip
    if (trip->startTrip())
    {
        // Update driver status
        Driver *driver = findDriverById(trip->getDriverId());
        if (driver != nullptr)
        {
            driver->setStatus(DRIVER_ON_TRIP);
        }
        return true;
    }

    return false;
}

bool DispatchEngine::completeTrip(int tripId)
{
    Trip *trip = findTripById(tripId);

    if (trip == nullptr)
    {
        cout << "Error: Trip " << tripId << " not found!" << endl;
        return false;
    }

    // Complete the trip
    if (trip->completeTrip())
    {
        // Update driver status
        Driver *driver = findDriverById(trip->getDriverId());
        if (driver != nullptr)
        {
            driver->setStatus(DRIVER_AVAILABLE);
            // Update driver location to dropoff
            driver->setCurrentLocation(trip->getDropoffLocation());
        }

        // Update rider status
        Rider *rider = findRiderById(trip->getRiderId());
        if (rider != nullptr)
        {
            rider->setActiveTripStatus(false);
        }

        return true;
    }

    return false;
}

bool DispatchEngine::cancelTrip(int tripId)
{
    Trip *trip = findTripById(tripId);

    if (trip == nullptr)
    {
        cout << "Error: Trip " << tripId << " not found!" << endl;
        return false;
    }

    // Cancel the trip
    if (trip->cancelTrip())
    {
        // Update driver status if trip was assigned or ongoing
        if (trip->getDriverId() != -1)
        {
            Driver *driver = findDriverById(trip->getDriverId());
            if (driver != nullptr)
            {
                driver->setStatus(DRIVER_AVAILABLE);
            }
        }

        // Update rider status
        Rider *rider = findRiderById(trip->getRiderId());
        if (rider != nullptr)
        {
            rider->setActiveTripStatus(false);
        }

        return true;
    }

    return false;
}

// ==================== Utility Functions ====================

void DispatchEngine::resizeDrivers()
{
    driverCapacity *= 2;
    Driver **newArr = new Driver *[driverCapacity];
    for (int i = 0; i < driverCount; i++)
        newArr[i] = drivers[i];
    delete[] drivers;
    drivers = newArr;
}

void DispatchEngine::resizeTrips()
{
    tripCapacity *= 2;
    Trip **newArr = new Trip *[tripCapacity];
    for (int i = 0; i < tripCount; i++)
        newArr[i] = trips[i];
    delete[] trips;
    trips = newArr;
}

void DispatchEngine::resizeRiders()
{
    riderCapacity *= 2;
    Rider **newArr = new Rider *[riderCapacity];
    for (int i = 0; i < riderCount; i++)
        newArr[i] = riders[i];
    delete[] riders;
    riders = newArr;
}

bool DispatchEngine::validateAssignment(Trip *trip, Driver *driver) const
{
    return trip && driver && driver->isAvailable();
}

// ==================== Driver ====================

bool DispatchEngine::registerDriver(Driver *driver)
{
    if (!driver)
        return false;
    if (driverCount == driverCapacity)
        resizeDrivers();
    drivers[driverCount++] = driver;
    return true;
}

bool DispatchEngine::removeDriver(int driverId)
{
    for (int i = 0; i < driverCount; i++)
    {
        if (drivers[i]->getId() == driverId)
        {
            drivers[i] = drivers[--driverCount];
            return true;
        }
    }
    return false;
}

Driver *DispatchEngine::findDriverById(int driverId) const
{
    for (int i = 0; i < driverCount; i++)
        if (drivers[i]->getId() == driverId)
            return drivers[i];
    return nullptr;
}

// ==================== Rider ====================

Rider *DispatchEngine::findRiderById(int riderId) const
{
    for (int i = 0; i < riderCount; i++)
        if (riders[i]->getId() == riderId)
            return riders[i];
    return nullptr;
}

// ==================== Trip ====================

bool DispatchEngine::createTrip(Trip *trip)
{
    if (!trip)
        return false;
    if (tripCount == tripCapacity)
        resizeTrips();
    trips[tripCount++] = trip;
    return true;
}

Trip *DispatchEngine::findTripById(int tripId) const
{
    for (int i = 0; i < tripCount; i++)
        if (trips[i]->getId() == tripId)
            return trips[i];
    return nullptr;
}

Trip *DispatchEngine::handleTripRequest(const Rider &rider, int distance)
{
    Trip *trip = new Trip(nextTripId++, rider.getId(),
                          rider.getPickupLocation(),
                          rider.getDropoffLocation(),
                          distance);
    createTrip(trip);
    return trip;
}

// ==================== Queries ====================

int DispatchEngine::getAvailableDriverCount() const
{
    int c = 0;
    for (int i = 0; i < driverCount; i++)
        if (drivers[i]->isAvailable())
            c++;
    return c;
}

int DispatchEngine::getTotalDriverCount() const { return driverCount; }
int DispatchEngine::getActiveTripCount() const { return tripCount; }
int DispatchEngine::getTotalTripCount() const { return tripCount; }

// ==================== Printing ====================

void DispatchEngine::printStatus() const
{
    cout << "\n=== Dispatch Engine Status ===" << endl;
    cout << "Total Drivers: " << driverCount << endl;
    cout << "Available Drivers: " << getAvailableDriverCount() << endl;
    cout << "Active Trips: " << tripCount << endl;
    cout << "Next Trip ID: " << nextTripId << endl;
    cout << "================================\n"
         << endl;
}

void DispatchEngine::printAvailableDrivers() const
{
    cout << "\n=== Available Drivers ===" << endl;
    bool found = false;
    for (int i = 0; i < driverCount; i++)
    {
        if (drivers[i]->isAvailable())
        {
            drivers[i]->printInfo(); // Changed from print() to printInfo()
            found = true;
        }
    }
    if (!found)
    {
        cout << "No available drivers at the moment." << endl;
    }
    cout << "=========================\n"
         << endl;
}

void DispatchEngine::printActiveTrips() const
{
    cout << "\n=== Active Trips ===" << endl;
    if (tripCount == 0)
    {
        cout << "No active trips at the moment." << endl;
    }
    else
    {
        for (int i = 0; i < tripCount; i++)
        {
            trips[i]->printInfo(); // Changed from print() to printInfo()
        }
    }
    cout << "====================\n"
         << endl;
}

int DispatchEngine::calculateDispatchScore(
    Driver *driver,
    int riderLocation,
    int sameZoneBonus,
    int crossZonePenalty) const
{
    int distance = city->getShortestDistance(
        driver->getCurrentLocation(),
        riderLocation);

    if (distance == -1)
        return INT_MAX;

    int driverZone = driver->getZoneId();
    int riderZone = city->getZone(riderLocation);

    if (driverZone == riderZone)
        distance += sameZoneBonus;
    else
        distance += crossZonePenalty;

    return distance;
}

Driver *DispatchEngine::findBestDriver(int riderPickupLocation)
{
    Driver *bestDriver = nullptr;
    int bestScore = INT_MAX;

    for (int i = 0; i < driverCount; i++)
    {
        if (!drivers[i]->isAvailable())
            continue;

        int score = calculateDispatchScore(
            drivers[i],
            riderPickupLocation,
            DEFAULT_SAME_ZONE_BONUS,
            DEFAULT_CROSS_ZONE_PENALTY);

        if (score < bestScore)
        {
            bestScore = score;
            bestDriver = drivers[i];
        }
    }
    return bestDriver;
}
Trip* DispatchEngine::requestTrip(const Rider& rider)
{
    int distance = city->getShortestDistance(
        rider.getPickupLocation(),
        rider.getDropoffLocation()
    );

    if (distance == -1)
        return nullptr;

    Trip* trip = handleTripRequest(rider, distance);

    Driver* bestDriver = findBestDriver(rider.getPickupLocation());
    if (!bestDriver)
        return trip;

    assignDriverToTrip(trip->getId(), bestDriver->getId());
    startTrip(trip->getId());

    return trip;
}
