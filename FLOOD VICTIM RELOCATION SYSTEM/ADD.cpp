#include<iostream>
#include<fstream>
#include<string>
using namespace std;

//array
string ppsname[100];
string ppsbil[100];

void victim();
void admin();
void addpps();
void searchVictim(); 
//void summary();

int main()
{
	int x;
	cout << "Please choose your desire action" << endl;
	cout << "1.Flood Victim" << endl << "2.Admin" << endl;
	cin >> x;
	switch (x)
	{
	case 1:
		victim();
		break;
	case 2:
		admin();
		break;
	}

	return 0;
};

void victim()
{
	ifstream readMaster("all_location.txt");
	if (!readMaster.is_open())
	{
		cout << "No location is available yet. Please wait for further notice" << endl;
		return;
	}

	string name;
	int capacity, current;
	int count = 0;

	cout << "\n--- Available Relocation Centers ---\n";
	while (readMaster >> name >> capacity >> current)
	{
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
	cout << "Please choose your desire Temporary relocation location";
	cin >> choice;
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
	while (masterIn >> tempNames[totalLines] >> tempCap[totalLines] >> tempCurrent[totalLines]) {
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
		masterOut << tempNames[i] << " " << tempCap[i] << " " << tempCurrent[i] << endl;
	}
	masterOut.close();

	cout << "\nYou have successfully register. Please head to " << ppsname[index] << " for further instruction" << endl;

};

void admin()
{
	string user, password;
	cout << "\n--- ADMIN LOGIN ---" << endl;
	cout << "Please enter your user id: ";
	cin >> user;
	cout << "Please enter your password: ";
	cin >> password;

	if (user == "ali" && password == "ali")
	{
		int x = 0;
		// added a do..while loop here so Admin will not get kicked out after one action
		do {
			cout << "\nWelcome " << user << ". Menu:" << endl;
			cout << "1. Add new relocation center" << endl;
			cout << "2. Search for a Victim" << endl; // <--- YOUR PART
			cout << "3. Logout" << endl;
			cout << "Enter choice: ";
			cin >> x;

			switch (x)
			{
			case 1:
				addpps();
				break;
			case 2:
				searchVictim(); 
				break;
			case 3:
				cout << "Logging out..." << endl;
				break;
			default:
				cout << "Invalid option." << endl;
			}
		} while (x != 3);
	}
	else
		cout << "Invalid login credentials." << endl;
}

void addpps()
{
	string ppsName;
	int capacity, current = 0;
	cout << "Please enter the name of the new temporary relocation location(use '_' for spaces):";
	cin >> ppsName;
	cout << "Enter the maximum capacity of the new temporary relocation location:";
	cin >> capacity;
	cout << endl;

	ofstream indfile(ppsName + ".txt");
	indfile.close();

	ofstream masterList("all_location.txt", ios::app);
	if (masterList.is_open())
	{
		masterList << ppsName << " " << capacity << " " << current << endl;
		cout << endl;
		cout << "Registration succcessfull! " << ppsName << " is now active. \n";
	}
	else {
		cout << "Error: Could not open master list file." << endl;
	}
}
;

void searchVictim()
{
	string target;
	cout << "\n--- SEARCHING VICTIM ---" << endl;
	cout << "Enter Victim IC Number (or first name): ";
	cin >> target;

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
