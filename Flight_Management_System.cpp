#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Flight structure
struct Flight {
    int flightNumber;
    string destination;
    int availableSeats;
    bool isDeleted;

    Flight() : flightNumber(0), destination(""), availableSeats(0), isDeleted(false) {}

    Flight(int number, string dest, int seats)
        : flightNumber(number), destination(dest), availableSeats(seats), isDeleted(false) {}
};

// Node structure for BST
struct FlightNode {
    Flight flightData;
    FlightNode* left;
    FlightNode* right;

    FlightNode(Flight flight)
        : flightData(flight), left(NULL), right(NULL) {}
};

class FlightManagementSystem {
private:
    FlightNode* root;

public:
    FlightManagementSystem() : root(NULL) {}

    void insertFlight(Flight flight) {
        root = insertFlightRec(root, flight);
    }

    FlightNode* insertFlightRec(FlightNode* node, Flight flight) {
        if (node == NULL) {
            return new FlightNode(flight);
        }

        if (flight.flightNumber < node->flightData.flightNumber) {
            node->left = insertFlightRec(node->left, flight);
        } else if (flight.flightNumber > node->flightData.flightNumber) {
            node->right = insertFlightRec(node->right, flight);
        } else {
            // If same flight number exists (even if deleted), update it
            node->flightData = flight;
        }

        return node;
    }

    Flight* searchFlight(int flightNumber) {
        FlightNode* node = searchFlightRec(root, flightNumber);
        return node ? &node->flightData : NULL;
    }

    FlightNode* searchFlightRec(FlightNode* node, int flightNumber) {
        if (node == NULL || node->flightData.flightNumber == flightNumber) {
            return node;
        }
        if (flightNumber < node->flightData.flightNumber)
            return searchFlightRec(node->left, flightNumber);
        return searchFlightRec(node->right, flightNumber);
    }

    void markFlightDeleted(int flightNumber) {
        FlightNode* node = searchFlightRec(root, flightNumber);
        if (node) {
            node->flightData.destination = "";
            node->flightData.availableSeats = 0;
            node->flightData.isDeleted = true;
            cout << "Flight data removed.\n";
        } else {
            cout << "Flight number not found.\n";
        }
    }

    void saveData() {
        ofstream outputFile("flight_data.txt");
        if (outputFile.is_open()) {
            saveDataRec(root, outputFile);
            outputFile.close();
            cout << "Flight data saved successfully.\n";
        } else {
            cout << "Unable to open file.\n";
        }
    }

    void saveDataRec(FlightNode* node, ofstream& outputFile) {
        if (node == NULL) return;

        saveDataRec(node->left, outputFile);
        outputFile << node->flightData.flightNumber << " "
                   << node->flightData.destination << " "
                   << node->flightData.availableSeats << " "
                   << node->flightData.isDeleted << endl;
        saveDataRec(node->right, outputFile);
    }

    void loadData() {
        ifstream inputFile("flight_data.txt");
        if (inputFile.is_open()) {
            int flightNumber, availableSeats;
            string destination;
            bool isDeleted;
            while (inputFile >> flightNumber >> destination >> availableSeats >> isDeleted) {
                Flight flight(flightNumber, destination, availableSeats);
                flight.isDeleted = isDeleted;
                insertFlight(flight);
            }
            inputFile.close();
            cout << "Flight data loaded successfully.\n";
        } else {
            cout << "Initializing flight data manually.\n";
            initializeFlights();
        }
    }

    void initializeFlights() {
        for (int i = 1; i <= 5; ++i) {
            string destination;
            int seats;
            cout << "Enter destination for Flight " << i << ": ";
            cin >> destination;
            cout << "Enter available seats for Flight " << i << ": ";
            cin >> seats;
            Flight flight(i, destination, seats);
            insertFlight(flight);
        }
    }

    void displayAvailableFlights() {
        cout << "\nAvailable Flights:\n";
        displayRec(root);
    }

    void displayRec(FlightNode* node) {
        if (node == NULL) return;

        displayRec(node->left);
        if (!node->flightData.isDeleted) {
            cout << "Flight " << node->flightData.flightNumber << ": "
                 << node->flightData.destination << " - Seats: "
                 << node->flightData.availableSeats << endl;
        } else {
            cout << "Flight Unavailable (Flight Number: " << node->flightData.flightNumber << ")\n";
        }
        displayRec(node->right);
    }

    void bookTicket(int flightNumber, int numTickets) {
        Flight* flight = searchFlight(flightNumber);
        if (flight && !flight->isDeleted) {
            if (flight->availableSeats >= numTickets) {
                flight->availableSeats -= numTickets;
                cout << "Ticket(s) booked successfully.\n";
            } else {
                cout << "Not enough seats.\n";
            }
        } else {
            cout << "Flight not found.\n";
        }
    }

    void checkFlightStatus(int flightNumber) {
        Flight* flight = searchFlight(flightNumber);
        if (flight && !flight->isDeleted) {
            cout << "Flight " << flightNumber << " - Available Seats: " << flight->availableSeats << endl;
        } else {
            cout << "Flight not found.\n";
        }
    }

    void updateFlightData(int flightNumber) {
        Flight* flight = searchFlight(flightNumber);
        if (flight) {
            string destination;
            int seats;
            cout << "Enter new destination: ";
            cin >> destination;
            cout << "Enter available seats: ";
            cin >> seats;
            flight->destination = destination;
            flight->availableSeats = seats;
            flight->isDeleted = false;  // Restore if was deleted
            cout << "Flight updated.\n";
            saveData();
        } else {
            cout << "Flight not found.\n";
        }
    }

    void deleteFlightData(int flightNumber) {
        markFlightDeleted(flightNumber);
        saveData();
    }

    void displayMenu() {
        cout << "\n || Flight Management System || \n";
        cout << "\n1. Display Available Flights\n";
        cout << "2. Book a Ticket\n";
        cout << "3. Check Flight Status\n";
        cout << "4. Update Flight Data\n";
        cout << "5. Delete Flight Data\n";
        cout << "6. Exit\n";
    }

    void processMenuChoice() {
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayAvailableFlights();
                break;
            case 2: {
                int flightNumber, numTickets;
                cout << "Enter flight number: ";
                cin >> flightNumber;
                cout << "Enter number of tickets: ";
                cin >> numTickets;
                bookTicket(flightNumber, numTickets);
                break;
            }
            case 3: {
                int flightNumber;
                cout << "Enter flight number: ";
                cin >> flightNumber;
                checkFlightStatus(flightNumber);
                break;
            }
            case 4: {
                int flightNumber;
                cout << "Enter flight number to update: ";
                cin >> flightNumber;
                updateFlightData(flightNumber);
                break;
            }
            case 5: {
                int flightNumber;
                cout << "Enter flight number to delete: ";
                cin >> flightNumber;
                deleteFlightData(flightNumber);
                break;
            }
            case 6:
                cout << "Thank you for using the Flight Management System.\n";
                exit(0);
            default:
                cout << "Invalid choice.\n";
        }
    }
};

// Main function
int main() {
    FlightManagementSystem system;
    system.loadData();

    while (true) {
        system.displayMenu();
        system.processMenuChoice();
    }

    return 0;
}

