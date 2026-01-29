#include "Trip.h"
#include <iostream>

using namespace std;

// Initialize static constants
const float Trip::BASE_FARE = 50.0f;
const float Trip::RATE_PER_KM = 10.0f;

// ==================== Trip Implementation ====================

Trip::Trip() : id(-1), riderId(-1), driverId(-1),
               pickupLocation(-1), dropoffLocation(-1),
               distance(0), state(REQUESTED), fare(0.0f)
{
    // Default constructor creates an invalid trip
}

Trip::Trip(int tripId, int rider, int pickup, int dropoff, int dist)
    : id(tripId), riderId(rider), driverId(-1),
      pickupLocation(pickup), dropoffLocation(dropoff),
      distance(dist), state(REQUESTED)
{

    // Validate input
    if (tripId < 0)
    {
        cout << "Warning: Trip ID should be non-negative!" << endl;
    }

    if (rider < 0)
    {
        cout << "Warning: Rider ID should be non-negative!" << endl;
    }

    if (pickup < 0 || dropoff < 0)
    {
        cout << "Warning: Location IDs should be non-negative!" << endl;
    }

    if (dist <= 0)
    {
        cout << "Warning: Trip distance should be positive!" << endl;
        distance = 1; // Default minimum distance
    }

    if (pickup == dropoff)
    {
        cout << "Warning: Pickup and dropoff are the same location!" << endl;
    }

    // Calculate initial fare
    calculateFare();

    cout << "Trip " << id << " created for rider " << riderId
         << " from " << pickupLocation << " to " << dropoffLocation
         << " (distance: " << distance << "km)" << endl;
}

Trip::~Trip()
{
    cout << "Trip " << id << " destroyed." << endl;
}

bool Trip::isValidTransition(TripState newState) const
{
    // Define valid state transitions
    switch (state)
    {
    case REQUESTED:
        // Can only go to ASSIGNED or CANCELLED
        return (newState == ASSIGNED || newState == CANCELLED);

    case ASSIGNED:
        // Can go to ONGOING or CANCELLED
        return (newState == ONGOING || newState == CANCELLED);

    case ONGOING:
        // Can go to COMPLETED or CANCELLED
        return (newState == COMPLETED || newState == CANCELLED);

    case COMPLETED:
        // Final state - no further transitions allowed
        return false;

    case CANCELLED:
        // Final state - no further transitions allowed
        return false;

    default:
        return false;
    }
}

void Trip::calculateFare()
{
    fare = BASE_FARE + (distance * RATE_PER_KM);
}

int Trip::getId() const
{
    return id;
}

int Trip::getRiderId() const
{
    return riderId;
}

int Trip::getDriverId() const
{
    return driverId;
}

void Trip::setDriverId(int driverId)
{
    if (driverId < 0)
    {
        cout << "Error: Cannot set negative driver ID!" << endl;
        return;
    }

    this->driverId = driverId;
    cout << "Driver " << driverId << " assigned to trip " << id << endl;
}

int Trip::getPickupLocation() const
{
    return pickupLocation;
}

int Trip::getDropoffLocation() const
{
    return dropoffLocation;
}

int Trip::getDistance() const
{
    return distance;
}

void Trip::setDistance(int dist)
{
    if (dist <= 0)
    {
        cout << "Error: Distance must be positive!" << endl;
        return;
    }

    distance = dist;
    calculateFare(); // Recalculate fare with new distance

    cout << "Trip " << id << " distance updated to " << distance
         << "km, new fare: " << fare << endl;
}

TripState Trip::getState() const
{
    return state;
}

float Trip::getFare() const
{
    return fare;
}

bool Trip::transitionTo(TripState newState)
{
    // Check if transition is valid
    if (!isValidTransition(newState))
    {
        cout << "Error: Invalid transition from " << stateToString(state)
             << " to " << stateToString(newState) << " for trip " << id << endl;
        return false;
    }

    // Perform the transition
    TripState oldState = state;
    state = newState;

    cout << "Trip " << id << " state changed from "
         << stateToString(oldState) << " to "
         << stateToString(newState) << endl;

    // Additional actions based on new state
    switch (newState)
    {
    case COMPLETED:
        cout << "Trip " << id << " completed successfully. Fare: " << fare << endl;
        break;
    case CANCELLED:
        cout << "Trip " << id << " cancelled. ";
        if (oldState == ONGOING)
        {
            cout << "Partial fare may apply." << endl;
        }
        else
        {
            cout << "No charges applied." << endl;
        }
        break;
    default:
        break;
    }

    return true;
}

bool Trip::assignDriver(int driverId)
{
    // Can only assign driver if in REQUESTED state
    if (state != REQUESTED)
    {
        cout << "Error: Cannot assign driver to trip " << id
             << " in state " << stateToString(state) << endl;
        return false;
    }

    setDriverId(driverId);
    return transitionTo(ASSIGNED);
}

bool Trip::startTrip()
{
    // Can only start trip if in ASSIGNED state
    if (state != ASSIGNED)
    {
        cout << "Error: Cannot start trip " << id
             << " in state " << stateToString(state) << endl;
        return false;
    }

    cout << "Driver " << driverId << " picked up rider " << riderId
         << " for trip " << id << endl;
    return transitionTo(ONGOING);
}

bool Trip::completeTrip()
{
    // Can only complete trip if in ONGOING state
    if (state != ONGOING)
    {
        cout << "Error: Cannot complete trip " << id
             << " in state " << stateToString(state) << endl;
        return false;
    }

    cout << "Driver " << driverId << " dropped off rider " << riderId
         << " for trip " << id << endl;
    return transitionTo(COMPLETED);
}

bool Trip::cancelTrip()
{
    // Can cancel from REQUESTED, ASSIGNED, or ONGOING states
    if (state == COMPLETED || state == CANCELLED)
    {
        cout << "Error: Cannot cancel trip " << id
             << " in final state " << stateToString(state) << endl;
        return false;
    }

    return transitionTo(CANCELLED);
}

bool Trip::isFinalState() const
{
    return (state == COMPLETED || state == CANCELLED);
}

bool Trip::isActive() const
{
    return (state == REQUESTED || state == ASSIGNED || state == ONGOING);
}

const char *Trip::stateToString(TripState state)
{
    switch (state)
    {
    case REQUESTED:
        return "REQUESTED";
    case ASSIGNED:
        return "ASSIGNED";
    case ONGOING:
        return "ONGOING";
    case COMPLETED:
        return "COMPLETED";
    case CANCELLED:
        return "CANCELLED";
    default:
        return "UNKNOWN";
    }
}

void Trip::printInfo() const
{
    cout << "\n=== Trip Information ===" << endl;
    cout << "Trip ID: " << id << endl;
    cout << "Rider ID: " << riderId << endl;
    cout << "Driver ID: " << (driverId == -1 ? "Not assigned" : to_string(driverId)) << endl;
    cout << "Pickup: " << pickupLocation << endl;
    cout << "Dropoff: " << dropoffLocation << endl;
    cout << "Distance: " << distance << "km" << endl;
    cout << "State: " << stateToString(state) << endl;
    cout << "Fare: " << fare << endl;
    cout << "Active: " << (isActive() ? "Yes" : "No") << endl;
    cout << "Final State: " << (isFinalState() ? "Yes" : "No") << endl;
    cout << "========================" << endl;
}

void Trip::printStateDiagram()
{
    cout << "\n=== Trip State Transition Diagram ===" << endl;
    cout << "REQUESTED  -> ASSIGNED   (when driver assigned)" << endl;
    cout << "REQUESTED  -> CANCELLED  (rider cancels before assignment)" << endl;
    cout << "ASSIGNED   -> ONGOING    (driver picks up rider)" << endl;
    cout << "ASSIGNED   -> CANCELLED  (rider/driver cancels after assignment)" << endl;
    cout << "ONGOING    -> COMPLETED  (driver drops off rider)" << endl;
    cout << "ONGOING    -> CANCELLED  (trip cancelled during ride)" << endl;
    cout << "COMPLETED  -> [NO TRANSITIONS] (final state)" << endl;
    cout << "CANCELLED  -> [NO TRANSITIONS] (final state)" << endl;
    cout << "=====================================" << endl;
}