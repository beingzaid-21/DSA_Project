#ifndef RIDER_H
#define RIDER_H

/**
 * @class Rider
 * @brief Represents a rider/customer in the ride-sharing system
 *
 * Each rider has a unique ID, pickup location, and dropoff location.
 * Riders request trips from the system.
 */
class Rider
{
private:
    int id;              ///< Unique rider ID
    int pickupLocation;  ///< Node ID where rider wants to be picked up
    int dropoffLocation; ///< Node ID where rider wants to go
    bool hasActiveTrip;  ///< Whether rider currently has an active trip

public:
    /**
     * @brief Default constructor
     */
    Rider();

    /**
     * @brief Parameterized constructor
     * @param riderId Unique ID for the rider
     * @param pickup Node ID for pickup location
     * @param dropoff Node ID for dropoff location
     */
    Rider(int riderId, int pickup, int dropoff);

    /**
     * @brief Destructor
     */
    ~Rider();

    /**
     * @brief Gets the rider's ID
     * @return Rider ID
     */
    int getId() const;

    /**
     * @brief Gets the pickup location
     * @return Pickup node ID
     */
    int getPickupLocation() const;

    /**
     * @brief Sets the pickup location
     * @param location New pickup node ID
     */
    void setPickupLocation(int location);

    /**
     * @brief Gets the dropoff location
     * @return Dropoff node ID
     */
    int getDropoffLocation() const;

    /**
     * @brief Sets the dropoff location
     * @param location New dropoff node ID
     */
    void setDropoffLocation(int location);

    /**
     * @brief Checks if the rider has an active trip
     * @return true if rider has active trip, false otherwise
     */
    bool hasActiveTripStatus() const;

    /**
     * @brief Sets the active trip status
     * @param active true if trip is active, false if completed/cancelled
     */
    void setActiveTripStatus(bool active);

    /**
     * @brief Updates both pickup and dropoff locations
     * @param pickup New pickup location
     * @param dropoff New dropoff location
     */
    void updateTripRequest(int pickup, int dropoff);

    /**
     * @brief Validates the trip request
     * @return true if pickup and dropoff are valid (different, non-negative)
     */
    bool validateTripRequest() const;

    /**
     * @brief Prints rider information
     *
     * Useful for debugging and display purposes
     */
    void printInfo() const;

    /**
     * @brief Calculates trip distance (straight-line estimation)
     * @note This is a simple estimation. Real distance would use graph algorithms.
     * @return Estimated distance between pickup and dropoff (absolute difference)
     */
    int estimateTripDistance() const;
};

#endif // RIDER_H