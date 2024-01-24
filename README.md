# Dublin Public Transport System Modeling
## Task 3 Overview
This program aims to model the public transport system in Dublin city using bus stop/route data provided by Dublin Bus. Google Maps utilizes this information to suggest the optimal sequence of buses/trains for reaching a destination quickly. The modeling is based on a graph structure, where bus stops (nodes) are connected by bus routes (edges). Each edge has a weight indicating the travel time for that route.

## Implementation Details
The implementation involves loading data from two CSV files:

1. #### Bus Stops (Vertices):
   Loaded from a CSV file containing a list of all bus stops in Dublin.
Each bus stop is considered as a node in the graph.
2. #### Routes between Bus Stops (Edges):
   Loaded from a CSV file containing a list of routes between bus stops.
Each route represents an edge in the graph with a weight indicating the distance in meters between bus stops.
Dijkstra's algorithm is then employed to find the optimal route from a specified source to a given destination.

..both of which are publically available on https://data.smartdublin.ie/.

## Functions Implemented

1. #### load_edges:
   Loads edges from a CSV file containing route information.
2. #### load_vertices:
   Loads vertices (bus stops) from a CSV file.
3. #### shortest_path:
   Prints the shortest path between a specified start and end bus stop using Dijkstra's algorithm.
4. #### free_memory:
   Frees up any memory used during the process.


More detailed information available in the brief.pdf
