#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

const string FileName = "Clients.txt";

struct stClient
{
	string AccountNumber;
	string PinCode;
	string FullName;
	string PhoneNumber;
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

stClient currentClient;

void ShowATMmenu();
void ShowQuickWithdrawl();
void Login();

enum enATMMenu{QuickWithdraw = 1,NormalWithdraw = 2,Deposit =3 , CheckBalance = 4 , Exit = 5};

vector<string> Split(string UserString, string deilma)
{
	vector <string> ElmentString;

	short pos = 0;
	string sword;

	while ((pos = UserString.find(deilma)) != std::string::npos)
	{
		sword = UserString.substr(0, pos);

		if (sword != "") {
			ElmentString.push_back(sword);
		}
		UserString.erase(0, pos + deilma.length());
	}
	if (UserString != "") {
		ElmentString.push_back(UserString);
	}

	return ElmentString;
}

stClient ConvertLineToRecord(string Record, string Spreator = "#//#")
{
	stClient Client;
	vector<string> vString = Split(Record, "#//#");

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.FullName = vString[2];
	Client.PhoneNumber = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}

vector<stClient> LoadDataToFile(string FileName)
{
	vector <stClient> vClients;
	std::fstream myfile;

	myfile.open(FileName, std::ios::in);

	if (myfile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(myfile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		myfile.close();

	}

	return vClients;
}

string ConvertRecordToLine(stClient Client, string Spreator = "#//#")
{
	string RecordString = "";
	RecordString += Client.AccountNumber + Spreator;
	RecordString += Client.PinCode + Spreator;
	RecordString += Client.FullName + Spreator;
	RecordString += Client.PhoneNumber + Spreator;
	RecordString += to_string(Client.AccountBalance);

	return RecordString;
}

vector<stClient> SaveDataToFile(stClient Client, vector<stClient>& vClient)
{

	std::fstream myfile;
	myfile.open(FileName, std::ios::out); //overwrite

	string DataLine;

	if (myfile.is_open())
	{
		for (stClient& C : vClient)
		{
			if (C.MarkForDelete == false) {

				DataLine = ConvertRecordToLine(C);
				myfile << DataLine << endl;
			}
		}
	}
	return vClient;
}

short ReadMenuScreen()
{
	short choise = 0;
	cout << "\nPlease enter number from [1 - 5] : ";
	cin >> choise;

	return choise;
}

short ReadQuickWithdrawl()
{
	short choise = 0;

	cout << "\nPlease enter number from [1 - 9] : ";
	cin >> choise;

	while (choise < 1 || choise > 9) 
	{
		cout << "\nPlease enter number from [1 - 9] : ";
		cin >> choise;
	}
	return choise;
}
short GetQuickWithdrawl(short Number)
{
	switch (Number) {
	case 1 :
		return 20;
	case 2 : 
		return 50;
	case 3 : 
		return 100;
	case 4: 
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	}
}

bool FindClientByAccountNumberAndPinCode(stClient &CurrentClient , string AccountNumber, string PinCode)
{
	vector<stClient> vClient = LoadDataToFile(FileName);

	for (stClient& Client : vClient) 
	{
		if (Client.AccountNumber == AccountNumber && Client.PinCode == PinCode) 
		{
			CurrentClient = Client;
			return true;
		}
	}
	return false;
}

void GoBackToMenu() 
{
	cout << "\nPlease press ENTER  to go back to menu...";
	system("pause>0");
	ShowATMmenu();
}

void ShowCheckBalance()
{
	system("cls");
	cout << "\n==================================\n";
	cout << "\t  Check Balance Screen";
	cout << "\n==================================\n";
	cout << "your balance : " << currentClient.AccountBalance;
	cout << "\n==================================\n";
}
bool DepositBalanceByAccountNumber(string AcccountNumber, vector<stClient>& vClients, short DepositAmount)
{
	char Answer = 'n';

	cout << "Do you want to confirm this transaction ? y/n : ";
	cin >> Answer;

	if (tolower(Answer) == 'y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AcccountNumber) {

				C.AccountBalance += DepositAmount;

				SaveDataToFile(C, vClients);

				cout << "\nDone succhssfully. New balance : " << C.AccountBalance;

				return true;
			}
		}


	}
	return false;
}
void PerformQuickWithdrawlOption(short QuickWithdrawlNumber)
{

	if (QuickWithdrawlNumber == 9)
		return;

	short WithdrawlMoney = GetQuickWithdrawl(QuickWithdrawlNumber);

	if (WithdrawlMoney > currentClient.AccountBalance)
	{
		cout << "\nSorry, the transaction cannot be completed.\n";
		cout << "\nthe withdrawl money is exceed your balance.\n";
		cout << "Please enter any key to continue...";
		system("pause>0");
		ShowQuickWithdrawl();
	}

	vector<stClient> vClient = LoadDataToFile(FileName);
	DepositBalanceByAccountNumber(currentClient.AccountNumber, vClient, WithdrawlMoney * -1);
	currentClient.AccountBalance -= WithdrawlMoney;
}


void ShowQuickWithdrawl()
{
	system("cls");
	cout << "\n==================================\n";
	cout << "\t  Quick Withdrawl Screen";
	cout << "\n==================================\n";
	cout << "[1] 20     [2] 50 \n";
	cout << "[3] 100    [4] 200 \n";
	cout << "[5] 400    [6] 600 \n";
	cout << "[7] 800    [8] 1000\n";
	cout << "[9] Exit\n";
	cout << "\n==================================";
	cout << "\nyour balance : " << currentClient.AccountBalance;

	PerformQuickWithdrawlOption(ReadQuickWithdrawl());
}
int ReadNormalWithdrawl() 
{
	int amonut = 0;
	cout << "\nPlease enter 5's amonut : ";
	cin >> amonut;

	while (amonut % 5 != 0) 
	{
		cout << "\nPlease enter 5's amonut : ";
		cin >> amonut;
	}
	return amonut;
}
int ReadDeposit()
{
	int amonut = 0;
	cout << "\nPlease enter Deposit number: ";
	cin >> amonut;

	return amonut;
}
void PerformNormalWithdrawl()
{

	int NormalWithdrawl = ReadNormalWithdrawl();

	if (NormalWithdrawl > currentClient.AccountBalance)
	{
		cout << "\nSorry, the transaction cannot be completed.\n";
		cout << "\nthe withdrawl money is exceed your balance.\n";
		cout << "Please enter any key to continue...";
		system("pause>0");
		ShowQuickWithdrawl();
	}

	vector<stClient> vClient = LoadDataToFile(FileName);
	DepositBalanceByAccountNumber(currentClient.AccountNumber, vClient, NormalWithdrawl * -1);
	currentClient.AccountBalance -= NormalWithdrawl;
}

void ShowNormalWithdrawl()
{
	system("cls");
	cout << "\n==================================\n";
	cout << "\t  Normal Withdrawl Screen";
	cout << "\n==================================\n";
	PerformNormalWithdrawl();
}
void PerformDeposit()
{
	int DepositNumber = ReadDeposit();

	vector<stClient> vClient = LoadDataToFile(FileName);
	DepositBalanceByAccountNumber(currentClient.AccountNumber, vClient, DepositNumber);
	currentClient.AccountBalance += DepositNumber;
}

void ShowDeposit()
{
	system("cls");
	cout << "\n==================================\n";
	cout << "\t  Deposit Screen";
	cout << "\n==================================\n";
	PerformDeposit();
}
void PerformMainMenuOption(enATMMenu ATMMenu)
{
	switch (ATMMenu)
	{
	case enATMMenu::QuickWithdraw:
		system("cls");
		ShowQuickWithdrawl();
		GoBackToMenu();
		break;
	case enATMMenu::NormalWithdraw:
		system("cls");
		ShowNormalWithdrawl();
		GoBackToMenu();
		break;
	case enATMMenu::Deposit:
		system("cls");
		ShowDeposit();
		GoBackToMenu();
		break;
	case enATMMenu::CheckBalance:
		system("cls");
		ShowCheckBalance();
		GoBackToMenu();
		break;
	case enATMMenu::Exit:
		system("cls");
		Login();
		break;
	}
}
void ShowATMmenu()
{
	system("cls");
	cout << "\n==================================\n";
	cout << "\t  ATM Menu Screen";
	cout << "\n==================================\n";
	cout << "[1] Quick Withdraw\n";
	cout << "[2] Normal Withdraw\n";
	cout << "[3] Deposit\n";
	cout << "[4] Check Balance\n";
	cout << "[5] Exit";
	cout << "\n==================================";

	PerformMainMenuOption((enATMMenu)ReadMenuScreen());
}

bool LoadDateInLogin(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPinCode(currentClient, AccountNumber, PinCode))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFailed = false;
	string AccountNumber, PinCode;
	do {
		cout << "\n----------------------------------\n";
		cout << "\t  Login Screen";
		cout << "\n----------------------------------\n";

		cout << "\nPlease enter your account number : ";
		cin >> AccountNumber;

		cout << "Please enter your Pin Code : ";
		cin >> PinCode;

		if (LoginFailed)
			cout << "\nInvalid username or password";

		LoginFailed = !LoadDateInLogin(AccountNumber, PinCode);

	} while (LoginFailed);

	ShowATMmenu();
}

int main() {

	Login();

	system("pause>0");
	return 0;
}
