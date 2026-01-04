#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include <vector>
using namespace std;

string ppsname[100];
string ppsbil[100];

void menu();
void victim();
void adminGateway();
void adminMenu();
void adminLogin();
void adminRegistration();
void addpps();
void summary();
void viewVictims();
void searchVictim();

struct Center {
	string name;
	int capacity;
	int current;
};

int main() {
	int choice;
	do {
		menu();
		cin >> choice;

		switch (choice) {
		case 1:
			cout << endl;
			victim();
			break;
		case 2:
			cout << endl;
			adminGateway();
			break;
		case 3:
			cout << endl;
			cout << "Exiting system. Stay safe!" << endl;
			break;
		default:
			cout << endl;
			cout << "Invalid option. Please try again." << endl;
		}

	} while (choice != 3);

	return 0;
}

void menu() {
	cout << "\n======================================" << endl;
	cout << "   FLOOD RELOCATION SYSTEM (MELAKA)   " << endl;
	cout << "======================================" << endl;
	cout << "1. Register as Flood Victim" << endl;
	cout << "2. Administrator Login" << endl;
	cout << "3. Exit" << endl;
	cout << "Please choose your desired action: ";
}

void victim()
{
	ifstream readMaster("all_location.txt");
	if (!readMaster.is_open())
	{
		cout << endl;
		cout << "No location is available yet. Please wait until further notice" << endl;
		return;
	}

	string name;
	int capacity, current;
	int count = 0;

	cout << "\n--- Available Relocation Centers ---\n";
	while (getline(readMaster, name,','))
	{
		readMaster >> capacity;
		readMaster >> current;
		readMaster.ignore(); // to skip the newline character after reading integers

		ppsname[count] = name; // store name in array
		cout << count + 1 << ". " << name << " (Status: " << current << "/" << capacity << ")";
		if (current == capacity)
		{
			cout << "[FULL]";
		}
		cout << endl;
		count++;
	}
	readMaster.close();
	//choice logic
	int choice;
	cout << "Please choose your desire Temporary relocation location or 0 to return to the previous menu : ";
	cin >> choice;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(1000, '\n');
		choice = 0;
	}
	if (choice == 0)
		return;
	int index = choice - 1; // match with array numbering

	if (index < 0 || index >= count)
	{
		cout << "Invalid selection!" << endl;
		return;
	}

	//register victim detail
	string vicname, vicid;
	cout << "Enter your Full name: ";
	cin.ignore();
	getline(cin, vicname);
	cout << "Enter your IC number: ";
	cin >> vicid;

	ofstream ppsFile(ppsname[index] + ".txt", ios::app);
	ppsFile << vicid << " " << vicname << endl;
	ppsFile.close();

	// --- UPDATE MASTER LIST COUNT ---

	// 1. Read everything into temporary arrays
	string tempNames[100];
	int tempCap[100], tempCurrent[100];
	int totalLines = 0;

	ifstream masterIn("all_location.txt");
	while (getline(masterIn, tempNames[totalLines], ',')) {
		masterIn >> tempCap[totalLines];
		masterIn >> tempCurrent[totalLines];
		// 2. If this is the PPS the victim chose, increase the count
		if (totalLines == index) {
			tempCurrent[totalLines]++;
		}
		totalLines++;
	}
	masterIn.close();

	// 3. Overwrite the master file with updated data
	ofstream masterOut("all_location.txt", ios::out); // ios::out clears the file
	for (int i = 0; i < totalLines; i++) {
		masterOut << tempNames[i] << "," << tempCap[i] << " " << tempCurrent[i] << endl;

	}
	masterOut.close();

	cout << "\nYou have successfully register. Please head to " << ppsname[index] << " for further instruction" << endl;

}

void adminGateway()
{
	int choice;
	cout << "--- ADMINISTRATOR ---" << endl;
	cout << "1. Login " << endl;
	cout << "2. Register " << endl;
	cout << "3. Return to Main Menu" << endl;
	cout << endl;
	cout << "Please choose your desired action: ";
	cin >> choice;

	
	do{
		switch (choice)
		{
		case 1:
			adminLogin();
			break;
		case 2:
			adminRegistration();
			break;
		case 3:
			cout << "Returning to main menu..." << endl;
		default:
			cout << "Invalid option." << endl;
			return;
		}
	} while (choice != 3);
	
}

void adminLogin()
{
	string fileUser, filePass;
	string inputUser, inputPass;
	bool found = false;
	do {
		cout << "\n--- ADMIN LOGIN ---" << endl;
		cout << "Please enter your user id and password(enter 0 to return to the previous menu)" << endl;
		cin >> inputUser >> inputPass;
		if (inputUser == "0" || inputPass == "0")
			return;
		// 1. Open the file to READ
		ifstream readFile("admin_account.txt");

		if (readFile.is_open()) {
			// 2. Loop through the file word by word
			while (readFile >> fileUser >> filePass) {
				if (fileUser == inputUser && filePass == inputPass) {
					found = true;
					break;
				}
			}
			readFile.close();
		}
		else {
			cout << "Error: File not found. Have you registered an admin yet?" << endl;
		}

		if (found) {

			cout << "Login successful! Welcome back, Admin "<< inputUser << '!' << endl;
			cout << endl;
			adminMenu();
		}
		else {
			cout << "Wrong username or password, Please try again" << endl;
		}
	} while (true);
}

void adminRegistration()
{

	string newUser, newPass,Masterkey;
	cout << "\n--- ADMIN REGISTRATION ---" << endl;
	cout << "Please enter the Passcode you have receive from your Superior" << endl;
	cin >> Masterkey;
	if (Masterkey != "admin123")
	{
		cout << "Invalid Passcode! Returning to previous menu.\n";
		return;
	}
	else
	cout << "Please enter your desired user id : ";
	getline(cin, newUser);

	cout << "Please enter your desired password(no spaces): ";
	cin >> newPass;

	ofstream masterList("admin_account.txt", ios::app);
	if (masterList.is_open())
	{
		masterList << newUser << " " << newPass << endl;
		cout << endl;
		cout << "Registration successful! Please return to the previous menu and login using the username and password you have entered.\n";
		cout << endl;
		adminGateway();

	}
	else {
		cout << "Error: Could not open master list file." << endl;
	}
}

void adminMenu()
{
	int choice;
	do {
		cout << "\n--- ADMINISTRATOR MENU ---" << endl;
		cout << "1. Add New Temporary Relocation Location" << endl;
		cout << "2. View Summary Report" << endl;
		cout << "3. View Victim List" << endl;
		cout << "4. Search Victim by IC" << endl;
		cout << "5. Logout" << endl;
		cout << "Please choose your desired action: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			cout << endl;
			addpps();
			break;
		case 2:
			summary();
			break;
		case 3:
			viewVictims();
			break;
		case 4:
			searchVictim();
			break;
		case 5:
			cout << "Logging out..." << endl;
			main();
		default:
			cout << "Invalid option. Please try again." << endl;
		}
	} while (true);
}

void addpps()
{
	char choice;
	do{
		string ppsName;
		int capacity, current = 0;
		cout << "Please enter the name of the new temporary relocation location(use '_' for spaces):";
		cin.ignore();
		getline(cin, ppsName);
		cout << "Enter the maximum capacity of the new temporary relocation location:";
		cin >> capacity;
		cout << endl;

		ofstream indfile(ppsName + ".txt");
		indfile.close();

		ofstream masterList("all_location.txt", ios::app);
		if (masterList.is_open())
		{
			masterList << ppsName << "," << capacity << " " << current << endl;
			cout << endl;
			cout << "Registration succcessfull! " << ppsName << " is now active. ";
			cout << endl;
			cout << "Would you like to add another Temporary Relocation Center? ";


		}
		else {
			cout << "Error: Could not open master list file." << endl;
		}
		cout << "Type Y to continue and N to return to the main menu : ";
		cin >> choice;
	} while (choice == 'Y' || choice == 'y');
	return;
}

void summary()
{
	ifstream master("all_location.txt");

	if (!master.is_open())
	{
		cout << "No PPS data available." << endl;
		return;
	}

	string name;
	int capacity, current;

	int totalPPS = 0;
	int totalCapacity = 0;
	int totalVictims = 0;

	cout << "\n====== PPS SUMMARY REPORT ======\n";
	cout << left << setw(5) << "No."
		<< setw(20) << "PPS Name"
		<< setw(15) << "Occupancy"
		<< "Status" << endl;
	cout << "-------------------------------------------------------\n";

	while (getline(master,name,','))
	{
		master >> capacity;
		master >> current;
		master.ignore(); // to skip the newline character after reading integers
		totalPPS++;
		totalCapacity += capacity;
		totalVictims += current;

		int percentage = (current * 100) / capacity;

		cout << left << setw(5) << totalPPS
			<< setw(20) << name
			<< setw(15) << " " << (to_string(current) + "/" + to_string(capacity));

		// Warning logic
		if (current == capacity)
		{
			cout << "[FULL]";
		}
		else if (percentage >= 80)
		{
			cout << "[WARNING: Almost Full]";
		}
		else {
			cout << "[AVAILABLE]";
		}

		cout << endl;
	}

	master.close();

	cout << "\n-------------------------------------------------------\n";
	cout << "Total PPS Locations   : " << totalPPS << endl;
	cout << "Total Victims         : " << totalVictims << endl;
	cout << "Total Capacity        : " << totalCapacity << endl;
	cout << "Available Spaces Left : "
		<< (totalCapacity - totalVictims) << endl;
	cout << "=======================================================\n";
}

void viewVictims()
{
	char repeat;

	do {
		ifstream readMaster("all_location.txt");
		if (!readMaster.is_open())
		{
			cout << "\n[!] No PPS data available." << endl;
			return;
		}

		vector<Center> centers;
		string name;
		int cap, curr;
		int count = 0;

		cout << "\n--- SELECT PPS TO VIEW ---" << endl;
		while (readMaster >> name >> cap >> curr)
		{
			Center c;
			c.name = name;
			c.capacity = cap;
			c.current = curr;
			centers.push_back(c);
			cout << count + 1 << ". " << name << endl;
			count++;
		}
		readMaster.close();

		if (count == 0) {
			cout << "No centers found." << endl;
			return;
		}

		int choice;
		cout << "Select PPS number(or 0 to go back to the previous menu): ";
		cin >> choice;
		if (choice == 0)//go back to previous menu
			return;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			choice = 0;
		}
		int index = choice - 1;

		if (index < 0 || index >= count)
		{
			cout << "[!] Invalid selection!" << endl;
		}
		else
		{
			string filename = centers[index].name + ".txt";
			ifstream ppsFile(filename);

			if (!ppsFile.is_open())
			{
				cout << "\n------------------------------------------------------------" << endl;
				cout << " PPS: " << centers[index].name;
				cout << " [Occupancy: " << centers[index].current << "/" << centers[index].capacity << "]" << endl;
				cout << "------------------------------------------------------------" << endl;
				cout << "[!] No victims registered here yet." << endl;
			}
			else
			{
				string vicIC, vicName;

				cout << "\n------------------------------------------------------------" << endl;
				cout << " PPS: " << centers[index].name;
				cout << " [Occupancy: " << centers[index].current << "/" << centers[index].capacity << "]" << endl;
				cout << "------------------------------------------------------------" << endl;
				cout << left << setw(5) << "No." << setw(20) << "IC Number" << "Name" << endl;
				cout << "------------------------------------------------------------" << endl;

				int vCount = 0;

				while (ppsFile >> vicIC)
				{
					getline(ppsFile, vicName);

					vCount++;
					cout << left << setw(5) << vCount
						<< setw(20) << vicIC
						<< vicName << endl;
				}

				if (vCount == 0) {
					cout << "   (No victims found in this list)" << endl;
				}

				cout << "------------------------------------------------------------" << endl;
				ppsFile.close();
			}
		}


		cout << "\nDo you want to view another PPS record? (Y for yes, N to return to previous menu): ";
		cin >> repeat;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			choice = 0;
		}

	} while (repeat == 'y' || repeat == 'Y');
}

void searchVictim()
{
	string target;
	cout << "\n--- SEARCHING VICTIM ---" << endl;
	cout << "Enter Victim IC Number" << endl;
	cout << "Enter 0 to return to previous menu.";
	cout << "\nInput: ";
	cin >> target;
	if (target == "0")
		return;

	ifstream masterFile("all_location.txt");
	if (!masterFile.is_open()) {
		cout << "Error: No data available to search." << endl;
		return;
	}

	string ppsName;
	int cap, curr;
	bool found = false;

	// LOOP 1: Read the Master List to get PPS names
	while (masterFile >> ppsName >> cap >> curr)
	{
		// Construct filename 
		ifstream ppsFile(ppsName + ".txt");

		string fileIC, fileName;

		// LOOP 2: Read inside the specific PPS file
		while (ppsFile >> fileIC)
		{
			// using getline for name because full name have spaces
			getline(ppsFile, fileName);

			// CHECK MATCH: Compare IC OR check if Name contains the target string
			// adding a space to fileName check because getline grabs the leading space
			if (fileIC == target || fileName.find(target) != string::npos)
			{
				cout << "\n[RECORD FOUND]" << endl;
				cout << "Name    : " << fileName << endl;
				cout << "IC No   : " << fileIC << endl;
				cout << "Location: " << ppsName << endl;
				found = true;
			}
		}
		ppsFile.close();
	}
	masterFile.close();

	if (!found) {
		cout << "\n[RESULT] No victim found with IC/Name: " << target << endl;
	}
}
