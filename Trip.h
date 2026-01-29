#ifndef TRIP_H
#define TRIP_H

/**
 * @enum TripState
 * @brief Represents the various states in a trip's lifecycle
 */
enum TripState {
    REQUESTED,     ///< Trip has been requested by rider
    ASSIGNED,      ///< Driver has been assigned to trip
    ONGOING,       ///< Trip is in progress (rider picked up)
    COMPLETED,     ///< Trip completed successfully
    CANCELLED      ///< Trip was cancelled
};

/**
 * @class Trip
 * @brief Represents a trip in the ride-sharing system with state machine
 * 
 * Manages the complete lifecycle of a trip from request to completion/cancellation.
 * Enforces valid state transitions.
 */
class Trip {
private:
    int id;                 ///< Unique trip ID
    int riderId;            ///< ID of the rider requesting the trip
    int driverId;           ///< ID of the driver assigned to the trip
    int pickupLocation;     ///< Node ID for pickup
    int dropoffLocation;    ///< Node ID for dropoff
    int distance;           ///< Total distance of the trip
    TripState state;        ///< Current state of the trip
    float fare;             ///< Calculated fare for the trip
    
    /**
     * @brief Validates a state transition
     * @param newState The desired new state
     * @return true if transition is valid, false otherwise
     */
    bool isValidTransition(TripState newState) const;
    
    /**
     * @brief Calculates the fare based on distance
     * Uses a simple formula: base fare + (distance * rate per km)
     */
    void calculateFare();
    
public:
    // Constants for fare calculation
    static const float BASE_FARE;
    static const float RATE_PER_KM;
    
    /**
     * @brief Default constructor
     */
    Trip();
    
    /**
     * @brief Parameterized constructor for new trip request
     * @param tripId Unique trip ID
     * @param rider ID of the rider
     * @param pickup Node ID for pickup location
     * @param dropoff Node ID for dropoff location
     * @param dist Total distance of the trip
     */
    Trip(int tripId, int rider, int pickup, int dropoff, int dist);
    
    /**
     * @brief Destructor
     */
    ~Trip();
    
    /**
     * @brief Gets the trip ID
     * @return Trip ID
     */
    int getId() const;
    
    /**
     * @brief Gets the rider ID
     * @return Rider ID
     */
    int getRiderId() const;
    
    /**
     * @brief Gets the driver ID
     * @return Driver ID (-1 if not assigned)
     */
    int getDriverId() const;
    
    /**
     * @brief Sets the driver ID (when driver is assigned)
     * @param driverId ID of the assigned driver
     */
    void setDriverId(int driverId);
    
    /**
     * @brief Gets the pickup location
     * @return Pickup node ID
     */
    int getPickupLocation() const;
    
    /**
     * @brief Gets the dropoff location
     * @return Dropoff node ID
     */
    int getDropoffLocation() const;
    
    /**
     * @brief Gets the trip distance
     * @return Distance in km
     */
    int getDistance() const;
    
    /**
     * @brief Sets the trip distance and recalculates fare
     * @param dist New distance
     */
    void setDistance(int dist);
    
    /**
     * @brief Gets the current trip state
     * @return Current TripState
     */
    TripState getState() const;
    
    /**
     * @brief Gets the calculated fare
     * @return Fare amount
     */
    float getFare() const;
    
    /**
     * @brief Attempts to transition to a new state
     * @param newState The desired new state
     * @return true if transition successful, false if invalid
     */
    bool transitionTo(TripState newState);
    
    /**
     * @brief Assigns a driver to the trip
     * @param driverId ID of the driver to assign
     * @return true if assignment successful, false if invalid state
     */
    bool assignDriver(int driverId);
    
    /**
     * @brief Starts the trip (rider picked up)
     * @return true if successful, false if invalid state
     */
    bool startTrip();
    
    /**
     * @brief Completes the trip (rider dropped off)
     * @return true if successful, false if invalid state
     */
    bool completeTrip();
    
    /**
     * @brief Cancels the trip
     * @return true if successful, false if invalid state
     */
    bool cancelTrip();
    
    /**
     * @brief Checks if the trip is in a final state
     * @return true if COMPLETED or CANCELLED, false otherwise
     */
    bool isFinalState() const;
    
    /**
     * @brief Checks if the trip is active
     * @return true if REQUESTED, ASSIGNED, or ONGOING, false otherwise
     */
    bool isActive() const;
    
    /**
     * @brief Converts state enum to string
     * @param state TripState to convert
     * @return String representation of state
     */
    static const char* stateToString(TripState state);
    
    /**
     * @brief Prints trip information
     * 
     * Useful for debugging and display purposes
     */
    void printInfo() const;
    
    /**
     * @brief Prints the state transition diagram
     * Static method to show valid transitions
     */
    static void printStateDiagram();
};

#endif // TRIP_H