#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

bool isvaliddate(int day, int month, int year) {
	if (year < 1 || month < 1 || month>12 || day < 1)
		return false;

	tm date = {};
	date.tm_mday = day;
	date.tm_mon = month - 1;
	date.tm_year = year - 1900;

	mktime(&date);
	return date.tm_mday == day &&
		date.tm_mon == month - 1 &&
		date.tm_year == year - 1900;
}
bool isweekday(const tm& date) {
	return date.tm_mday != 0 && date.tm_wday != 6;
}
int main() {
	char repeat;
	do {
		string inputdate;
		int workday;
		cout << "Input date (dd.mm.yyyy): ";
		cin >> inputdate;
		cout << "Input workday amount: ";
		cin >> workday;
		if (cin.fail() || workday < 0) {
			cout << "Error: Workday amount must be above zero.";
			return 1;
		}
		else {
			int day, month, year;
			char dot1, dot2;
			stringstream ss(inputdate);
			if (!(ss >> day >> dot1 >> month >> dot2 >> year) || dot1 != '.' || dot2 != '.' || !ss.eof()) {
				cout << "Error: incorrect date format.";
			}
			else if (!isvaliddate(day, month, year)) {
				cout << "Such date does not exist.";
			}
			else {
				tm date = {};
				date.tm_mday = day;
				date.tm_mon = month;
				date.tm_year = year;
				mktime(&date);
				int daysadded = 0;
				while (daysadded < workday) {
					date.tm_mday++;
					mktime(&date);
					if (isweekday(date)) {
						daysadded++;
					}
				}
				cout << "Result: " << setfill('0') << setw(2) << date.tm_mday << "." << setw(2) << date.tm_mon + 1 << "." << setw(4) << date.tm_year + 1900 << endl;
			}
		}
		cout << "Repeat the program?(y/n): ";
		cin >>repeat;
		while (repeat != 'y' && repeat != 'Y' &&
			repeat != 'n' && repeat != 'N')
		{
			cout << "Error: input N or Y";
			cin >> repeat;
		}
	} while (repeat == 'y' || repeat == 'Y');
	return 0;
}