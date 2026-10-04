#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

int main() {
	char repeat;

	do {
		string inputdate;
		int workdays;

		cout << "Input date (dd.mm.yyyy): ";
		cin >> inputdate;

		cout << "Input workday amount: ";
		cin >> workdays;

		if (workdays < 0) {
			cout << "Error: Workday amount cannot be negative.";
			return 1;
		}

		else {
			int day, month, year;
			char dot1, dot2;

			stringstream ss(inputdate);
			if (!(ss >> day >> dot1 >> month >> dot2 >> year) ||
				dot1 != '.', dot2 != '.') {
				cout << "Error:Incorrect date format." << endl;
				return 1;
			}
			else {
				tm date = {};
				date.tm_mday = day;
				date.tm_mon = month - 1;
				date.tm_year = year - 1900;
				time_t time = mktime(&date);

				if (time == -1 || date.tm_mday != day || date.tm_mon != month - 1 || date.tm_year != year - 1900) {
					cout << "Error: Such date does not exist." << endl;
				}
				else {
					while (workdays > 0) {
						date.tm_mday++;
						mktime(&date);
						if (date.tm_wday != 0 && date.tm_wday != 6) {
							workdays--;
						}
					}

					cout << "\nResult: "
						<< setfill('0') << setw(2) << date.tm_mday << "."
						<< setw(2) << date.tm_mon + 1 << "."
						<< date.tm_year + 1900 << endl;
				}
			}
		}

		cout << "\nRun program again? (y/n): ";
		cin >> repeat;

	} while (repeat == 'y' || repeat == 'Y');

	return 0;
}