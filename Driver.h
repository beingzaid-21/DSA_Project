#ifndef DRIVER_H
#define DRIVER_H

/**
 * @enum DriverStatus
 * @brief Represents the current status of a driver
 *
 * Note: These values should align with TripState for consistency
 */
enum DriverStatus
{
    DRIVER_AVAILABLE, ///< Driver is available for new trips
    DRIVER_ASSIGNED,  ///< Driver is assigned to a trip
    DRIVER_ON_TRIP,   ///< Driver is currently on a trip
    DRIVER_OFFLINE    ///< Driver is offline/unavailable
};

/**
 * @class Driver
 * @brief Represents a driver in the ride-sharing system
 *
 * Each driver has a unique ID, current location, zone, and availability status.
 */
class Driver
{
private:
    int id;              ///< Unique driver ID
    int currentLocation; ///< Current node/location ID where driver is
    int zoneId;          ///< Zone ID where driver operates
    DriverStatus status; ///< Current availability status

public:
    /**
     * @brief Default constructor
     */
    Driver();

    /**
     * @brief Parameterized constructor
     * @param driverId Unique ID for the driver
     * @param locationId Initial location node ID
     * @param zone Zone ID for the driver
     */
    Driver(int driverId, int locationId, int zone);

    /**
     * @brief Destructor
     */
    ~Driver();

    /**
     * @brief Gets the driver's ID
     * @return Driver ID
     */
    int getId() const;

    /**
     * @brief Gets the driver's current location
     * @return Current node/location ID
     */
    int getCurrentLocation() const;

    /**
     * @brief Sets the driver's current location
     * @param locationId New location node ID
     */
    void setCurrentLocation(int locationId);

    /**
     * @brief Gets the driver's zone
     * @return Zone ID
     */
    int getZoneId() const;

    /**
     * @brief Sets the driver's zone
     * @param zoneId New zone ID
     */
    void setZoneId(int zoneId);

    /**
     * @brief Gets the driver's current status
     * @return Current DriverStatus
     */
    DriverStatus getStatus() const;

    /**
     * @brief Sets the driver's status
     * @param newStatus New status to set
     */
    void setStatus(DriverStatus newStatus);

    /**
     * @brief Checks if the driver is available for a new trip
     * @return true if driver is DRIVER_AVAILABLE, false otherwise
     */
    bool isAvailable() const;

    /**
     * @brief Prints driver information
     *
     * Useful for debugging and display purposes
     */
    void printInfo() const;

    /**
     * @brief Converts status enum to string
     * @param status DriverStatus to convert
     * @return String representation of status
     */
    static const char *statusToString(DriverStatus status);
};

#endif // DRIVER_H