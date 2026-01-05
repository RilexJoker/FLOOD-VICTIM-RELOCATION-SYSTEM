#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>

using namespace std;

struct Center {
    string name;
    int capacity;
    int current;
};

Center ppsList[100];
int totalPPS = 0;

void menu();
void victim();
void adminGateway();
void adminMenu();
bool adminLogin();
void adminRegistration();
void addpps();
void summary();
void viewVictims();
void searchVictim();

int main() {
    int choice;
    do {
        menu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
        case 1: victim(); break;
        case 2: adminGateway(); break;
        case 3: cout << "Exiting system. Stay safe!" << endl; break;
        default: cout << "Invalid option." << endl;
        }
    } while (choice != 3);

    return 0;
}

void menu() {
    cout << "\n======================================" << endl;
    cout << "     FLOOD RELOCATION SYSTEM (MELAKA)  " << endl;
    cout << "======================================" << endl;
    cout << "1. Register as Flood Victim" << endl;
    cout << "2. Administrator Login" << endl;
    cout << "3. Exit" << endl;
    cout << "Choice: ";
}

// Logic: Load file using '|' delimiter to support spaces
void victim() {
    ifstream readMaster("all_location.txt");
    if (!readMaster.is_open()) {
        cout << "No locations found." << endl;
        return;
    }

    totalPPS = 0;
    while (totalPPS < 100 && getline(readMaster, ppsList[totalPPS].name, '|')) {
        readMaster >> ppsList[totalPPS].capacity;
        readMaster >> ppsList[totalPPS].current;
        readMaster.ignore(); // Skip newline
        totalPPS++;
    }
    readMaster.close();

    if (totalPPS == 0) {
        cout << "No PPS registered yet." << endl;
        return;
    }

    cout << "\n--- Available Centers ---\n";
    for (int i = 0; i < totalPPS; i++) {
        cout << i + 1 << ". " << ppsList[i].name
            << " [" << ppsList[i].current << "/" << ppsList[i].capacity << "]";
        if (ppsList[i].current >= ppsList[i].capacity) cout << " [FULL]";
        cout << endl;
    }

    int choice;
    cout << "Please select a Temporary Relocation Center(0 to back): ";
    cin >> choice;
    if (choice <= 0 || choice > totalPPS) return;

    int idx = choice - 1;
    if (ppsList[idx].current >= ppsList[idx].capacity) {
        cout << "Center is full!" << endl;
        return;
    }

    string vicname, vicid;
    cout << "Please enter your full Name: ";
    cin.ignore();
    getline(cin, vicname);
    cout << "PLease enter your IC Number: ";
    cin >> vicid;

    ofstream ppsFile(ppsList[idx].name + ".txt", ios::app);
    ppsFile << vicid << " " << vicname << endl;
    ppsFile.close();

    ppsList[idx].current++;
    ofstream masterOut("all_location.txt");
    for (int i = 0; i < totalPPS; i++) {
        masterOut << ppsList[i].name << "|" << ppsList[i].capacity << " " << ppsList[i].current << endl;
    }
    masterOut.close();
    cout << "Registration Successful. Please head to " << ppsList[idx].name <<" For further instruction." << endl;
}

void adminGateway() {
    int choice;
    bool loggedIn = false;

    do {
        cout << "\n--- ADMINISTRATOR GATEWAY ---" << endl;
        cout << "1. Login\n2. Register\n3. Back to Main Menu\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            loggedIn = adminLogin(); // This calls your bool function
            if (loggedIn) {
                adminMenu(); // Go to menu if successful
                break;       // Exit this loop after logging out of adminMenu
            }
            // If loggedIn is false, the loop naturally repeats
        }
        else if (choice == 2) {
            adminRegistration();
        }
        else if (choice != 3) {
            cout << "Invalid option." << endl;
        }

    } while (choice != 3);
}

bool adminLogin() {
    string u, p, iu, ip;
    cout << "\n--- LOGIN ---" << endl;
    cout << "Please enter your username and password" << endl;
    cout << "Username: "; cin >> iu;
    cout << "Password: "; cin >> ip;

    ifstream read("admin_account.txt");
    while (read >> u >> p) {
        if (u == iu && p == ip) {
            return true;
        }
    }
    read.close();

    // If we reach here, it means no match was found
    cout << "\n[!] Wrong username or password. Please try again." << endl;
    return false;
}

void adminRegistration() {
    string key, user, pass;
    cout << "Please enter the passcode you have receive from your supervisor." << endl;
    cout << "Passcode: "; cin >> key;
    if (key != "admin123") return;
    cout << "Please enter a username: "; cin >> user;
    cout << "Please enter a password: "; cin >> pass;
    ofstream write("admin_account.txt", ios::app);
    write << user << " " << pass << endl;
    cout << "Registration Successful. You may now login through the main menu.";
}

void adminMenu() {
    int choice;
    do {
        cout << "\n--- ADMIN MENU ---\n1. Add new Temporary Relocation Center\n2. Summary\n3. View Victims List\n4. Search\n5. Logout\nChoice: ";
        cin >> choice;
        switch (choice) {
        case 1: addpps(); break;
        case 2: summary(); break;
        case 3: viewVictims(); break;
        case 4: searchVictim(); break;
        }
    } while (choice != 5);
}

void addpps() {
    char choice = 0;
    do{
        string ppsName;
        int capacity;
        cin.ignore(1000, '\n');
		cout << "\n--- ADD NEW TEMPORARY RELOCATION CENTER ---" << endl;
		cout << "- enter 0 at any time to return to the previous menu. -\n" << endl;
        cout << "Please enter the new temporary relocation center name : ";
        getline(cin, ppsName);
        if (ppsName == "0")
            return;
        cout << "enter the maximum occupation capacity for the new temporary relocation center: ";
        cin >> capacity;
        if (capacity == 0)
            return;

        ofstream masterList("all_location.txt", ios::app);
        masterList << ppsName << "|" << capacity << " 0" << endl;
        masterList.close();

        ofstream indfile(ppsName + ".txt"); // Create victim file
        indfile.close();
        cout << "New temporary relocation center have been successfully added.";
        cout << "Press Y to continue adding Temporary relocation center, N to return to the previous menu.\n";

    } while (choice == 'Y' || choice == 'y');
    cout << endl;
    return;
}

void summary() {
    ifstream master("all_location.txt");

    if (!master.is_open()) {
        cout << "Error: Could not open all_location.txt" << endl;
        return;
    }

    string name;
    int capacity, current;
    int totalPPS = 0, totalCapacity = 0, totalVictims = 0;

    // Header
    cout << "\n==================== PPS SUMMARY REPORT ====================\n";
    cout << left << setw(5) << "No."
        << setw(25) << "PPS Name"
        << setw(15) << "Occupancy"
        << "Status" << endl;
    cout << string(60, '-') << endl;

    // Combined Logic: Use getline to handle spaces in names
    while (getline(master, name, '|')) {
        if (!(master >> capacity >> current)) break;
        master.ignore(); // Clean up the newline after numbers

        totalPPS++;
        totalCapacity += capacity;
        totalVictims += current;

        // Calculate percentage for status logic
        double percentage = (capacity > 0) ? (static_cast<double>(current) / capacity) * 100 : 0;
        string occString = to_string(current) + "/" + to_string(capacity);

        // Print Row
        cout << left << setw(5) << totalPPS
            << setw(25) << name
            << setw(15) << occString;

        // Status Logic
        if (current >= capacity) {
            cout << "[FULL]";
        }
        else if (percentage >= 80) {
            cout << "[WARNING: Almost Full]";
        }
        else {
            cout << "[AVAILABLE]";
        }
        cout << endl;
    }

    master.close();

    // Footer with Totals
    cout << string(60, '-') << endl;
    cout << "Total PPS Locations   : " << totalPPS << endl;
    cout << "Total Victims         : " << totalVictims << endl;
    cout << "Total Capacity        : " << totalCapacity << endl;
    cout << "Available Spaces Left : " << (totalCapacity - totalVictims) << endl;
    cout << "============================================================\n";
}

void viewVictims() {
    ifstream readMaster("all_location.txt");
    if (!readMaster.is_open()) {
        cout << "\n[Error: Could not load PPS list]\n";
        return;
    }

    string names[100];
    int count = 0;
    string tempName;

    cout << "\n========================================";
    cout << "\n       SELECT PPS TO VIEW VICTIMS       ";
    cout << "\n========================================\n";

    while (count < 100 && getline(readMaster, tempName, '|')) {
        int d1, d2;
        if (readMaster >> d1 >> d2) {
            readMaster.ignore();
            names[count] = tempName;
            cout << left << setw(4) << to_string(count + 1) + "."
                 << names[count] << endl;
            count++;
        }
    }
    readMaster.close();

    if (count == 0) {
        cout << "No PPS locations found.\n";
        return;
    }

    char userChoice;
    do {
        int selection;
        cout << "\nEnter PPS number to view victims: ";

        while (!(cin >> selection) || selection < 1 || selection > count) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid selection. Please enter a number between 1 and " << count << ": ";
        }

        cout << "\n----------------------------------------";
        cout << "\n[Displaying victims for: " << names[selection - 1] << "]"; 
        cout << "\n----------------------------------------\n";

        cout << "\nPress 'y' to see other PPS or any key to back to previous menu: ";
        cin >> userChoice;

    } while (userChoice == 'y' || userChoice == 'Y');
}

}

void searchVictim() {
    string target;
    cout << "Please enter the name or IC number of the person you wish to search for : "; cin >> target;
    ifstream master("all_location.txt");
    string pps; int c, cu;
    bool found = false;

    // FIXED: Must use getline with '|' here too
    while (getline(master, pps, '|')) {
        master >> c >> cu;
        master.ignore();
        ifstream file(pps + ".txt");
        string ic, vname;
        while (file >> ic >> ws && getline(file, vname)) {
            if (ic == target || vname.find(target) != string::npos) {
                cout << "Match: " << vname << " can be found at " << pps << endl;
                found = true;
            }
        }
        file.close();
    }
    if (!found) cout << "THe person you are searching for is not in our system.\n";
}
