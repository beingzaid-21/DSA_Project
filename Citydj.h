#ifndef CITY_H
#define CITY_H

/**
 * @class City
 * @brief Represents a city as a weighted graph where nodes are locations and edges are roads with distances.
 *
 * This class provides graph operations with weighted edges, zone support, and shortest path finding.
 * It uses dynamic arrays instead of STL containers.
 */
class City
{
private:
    /**
     * @struct Road
     * @brief Represents a road connection with distance
     */
    struct Road
    {
        int toNodeId; ///< Destination node ID
        int distance; ///< Distance/weight of the road

        Road();                 ///< Default constructor
        Road(int to, int dist); ///< Parameterized constructor
    };

    /**
     * @struct Node
     * @brief Represents a location (node) in the city
     */
    struct Node
    {
        int id;        ///< Unique identifier for the location
        int zoneId;    ///< Zone ID this location belongs to
        Road *roads;   ///< Dynamic array of roads
        int roadCount; ///< Number of roads
        int capacity;  ///< Current capacity of roads array

        Node();                             ///< Default constructor
        Node(int nodeId);                   ///< Parameterized constructor
        ~Node();                            ///< Destructor
        void addRoad(int to, int distance); ///< Add a road with distance
        bool hasRoadTo(int nodeId) const;   ///< Check if road exists to a node
        int getRoadIndex(int nodeId) const; ///< Get index of road to a node
    };

    Node **nodes;  ///< Array of pointers to nodes
    int nodeCount; ///< Current number of nodes
    int capacity;  ///< Current capacity of nodes array

    /**
     * @brief Resizes the nodes array when more capacity is needed
     */
    void resizeNodes();

    /**
     * @brief Finds a node by ID
     * @param id The node ID to find
     * @return Index of the node in the nodes array, or -1 if not found
     */
    int findNode(int id) const;

    /**
     * @brief Finds a node by ID and returns the Node object
     * @param id The node ID to find
     * @return Pointer to the Node, or nullptr if not found
     */
    Node *getNode(int id) const;

public:
    /**
     * @struct ShortestPathResult
     * @brief Stores the result of Dijkstra's algorithm
     */
    struct ShortestPathResult
    {
        int *distances;    ///< Shortest distances from source to all nodes
        int *predecessors; ///< Predecessor nodes for path reconstruction
        int nodeCount;     ///< Number of nodes in the result

        ShortestPathResult();          ///< Default constructor
        ShortestPathResult(int count); ///< Parameterized constructor
        ~ShortestPathResult();         ///< Destructor

        /**
         * @brief Gets the shortest distance to a specific node
         * @param nodeId The destination node ID
         * @return Shortest distance, or -1 if node not found/infinite
         */
        int getDistanceTo(int nodeId) const;

        /**
         * @brief Gets the path from source to destination
         * @param destination Destination node ID
         * @param pathArray Pre-allocated array to store the path
         * @return Number of nodes in the path, or -1 if no path exists
         */
        int getPathTo(int destination, int *pathArray) const;

        /**
         * @brief Prints all shortest distances from source
         */
        void printDistances() const;
    };

    /**
     * @brief Default constructor
     */
    City();

    /**
     * @brief Destructor
     */
    ~City();

    /**
     * @brief Adds a new location (node) to the city
     * @param id Unique identifier for the location
     * @return true if added successfully, false if already exists
     */
    bool addLocation(int id);

    /**
     * @brief Adds a road (edge) between two locations with distance
     * @param from Source location ID
     * @param to Destination location ID
     * @param distance Distance/weight of the road
     * @return true if road added successfully, false if invalid nodes
     */
    bool addRoad(int from, int to, int distance);

    /**
     * @brief Sets the zone for a location
     * @param nodeId The location ID
     * @param zoneId The zone ID to assign
     * @return true if zone set successfully, false if location doesn't exist
     */
    bool setZone(int nodeId, int zoneId);

    /**
     * @brief Gets the zone ID for a location
     * @param nodeId The location ID
     * @return Zone ID if location exists, -1 if location doesn't exist
     */
    int getZone(int nodeId) const;

    /**
     * @brief Gets all locations in a specific zone
     * @param zoneId The zone ID to query
     * @param resultArray Array to store location IDs (must be pre-allocated with sufficient size)
     * @return Number of locations found in the zone
     */
    int getLocationsInZone(int zoneId, int *resultArray) const;

    /**
     * @brief Gets the total number of locations in the city
     * @return Number of nodes
     */
    int getNodeCount() const;

    /**
     * @brief Checks if a location exists
     * @param id Location ID to check
     * @return true if location exists
     */
    bool locationExists(int id) const;

    /**
     * @brief Gets the distance between two locations
     * @param from Source location ID
     * @param to Destination location ID
     * @return Distance if road exists, -1 if no road or invalid nodes
     */
    int getDistance(int from, int to) const;

    /**
     * @brief Dijkstra's shortest path algorithm
     * @param source Source node ID
     * @return ShortestPathResult object containing distances and paths from source
     */
    ShortestPathResult dijkstra(int source) const;

    /**
     * @brief Gets the shortest distance between two specific nodes
     * @param source Source node ID
     * @param destination Destination node ID
     * @return Shortest distance, or -1 if no path exists
     */
    int getShortestDistance(int source, int destination) const;

    /**
     * @brief Gets the shortest path between two nodes
     * @param source Source node ID
     * @param destination Destination node ID
     * @param pathArray Pre-allocated array to store the path
     * @return Number of nodes in the path, or -1 if no path exists
     */
    int getShortestPath(int source, int destination, int *pathArray) const;

    /**
     * @brief Prints all locations and their connections with distances and zones
     *
     * Useful for debugging and verification
     */
    void printGraph() const;

    /**
     * @brief Prints zone information for all locations
     */
    void printZones() const;
};

#endif // CITY_H