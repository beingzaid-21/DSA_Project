#ifndef DISPATCHENGINE_H
#define DISPATCHENGINE_H

#include "Citydj.h"
#include "Driver.h"
#include "Rider.h"
#include "Trip.h"

/**
 * @class DispatchEngine
 * @brief Handles driver dispatch logic for ride-sharing system
 */
class DispatchEngine
{
private:
    // ===== Core Data =====
    City *city;

    // ===== Drivers =====
    Driver **drivers;
    int driverCount;
    int driverCapacity;

    // ===== Trips =====
    Trip **trips;
    int tripCount;
    int tripCapacity;

    // ===== Riders =====
    Rider **riders;    // 🔧 ADDED
    int riderCount;    // 🔧 ADDED
    int riderCapacity; // 🔧 ADDED

    // ===== Trip ID Generator =====
    int nextTripId; // 🔧 ADDED

    // ===== Internal Helpers =====
    void resizeDrivers();
    void resizeTrips();
    void resizeRiders(); // 🔧 ADDED

    /**
     * @brief Validates whether a driver can be assigned to a trip
     */
    bool validateAssignment(Trip *trip, Driver *driver) const; // 🔧 ADDED

    /**
     * @brief Calculates dispatch score
     */
    int calculateDispatchScore(Driver *driver,
                               int riderLocation,
                               int sameZoneBonus,
                               int crossZonePenalty) const;

public:
    // ===== Constants =====
    static const int DEFAULT_SAME_ZONE_BONUS;
    static const int DEFAULT_CROSS_ZONE_PENALTY;

    // ===== Constructor / Destructor =====
    DispatchEngine(City *cityPtr);
    ~DispatchEngine();

    // ===== Driver Management =====
    bool registerDriver(Driver *driver);
    bool removeDriver(int driverId);
    Driver *findDriverById(int driverId) const;

    Trip *requestTrip(const Rider &rider);

    // ===== Rider Management =====
    Rider *findRiderById(int riderId) const; // 🔧 ADDED

    // ===== Trip Management =====
    bool createTrip(Trip *trip);
    Trip *handleTripRequest(const Rider &rider, int distance);

    bool assignDriverToTrip(int tripId, int driverId);
    Driver *findBestDriver(int riderPickupLocation);

    bool startTrip(int tripId);    // 🔧 ADDED
    bool completeTrip(int tripId); // 🔧 ADDED
    bool cancelTrip(int tripId);   // 🔧 ADDED

    Trip *findTripById(int tripId) const;

    // ===== Queries =====
    int getAvailableDriverCount() const;
    int getTotalDriverCount() const;
    int getActiveTripCount() const;
    int getTotalTripCount() const;

    // ===== Debug / Display =====
    void printStatus() const;
    void printAvailableDrivers() const;
    void printActiveTrips() const;
};

#endif // DISPATCHENGINE_H