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
	cout << "Please enter your user id and password" << endl;
	cin >> user >> password;
	if (user == "ali" && password == "ali")
	{
		int x = 0;
		cout << "Welcome admin " << user << ", What would you like to do today" << endl;
		cout << "1. Add new temporary relocation location" << endl << "2. Check available temporary location"<<endl;
		cin >> x;
		switch (x)
		{
		case 1:
			addpps();
			break;
		/*case 2:
			summary();
			break;*/
		}
	}
	else
		cout << "The username or password you have enter is unavailable";

}

void addpps()
{
	string ppsName;
	int capacity,current = 0;
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
		masterList << ppsName << " " << capacity <<" "<<  current << endl;
		cout << endl;
		cout << "Registration succcessfull! " << ppsName << " is now active. \n";
	}
	else {
		cout << "Error: Could not open master list file." << endl;
	}
}
;
