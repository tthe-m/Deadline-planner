#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <fstream>
#include <set>
#include <windows.h>

using namespace std;

//перевірка, чи є дата коректною
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

int dateKey(const tm& date) {
	return(date.tm_year + 1900) * 10000 + (date.tm_mon + 1) * 100 + date.tm_mday;
}

//перевірка, чи є день робочим
bool isweekday(const tm& date) {
	//0 - неділя 6 - субота
	return date.tm_mday != 0 && date.tm_wday != 6;
}

bool loadHolidays(const string& filename, set<int>& holidays) {
	ifstream file(filename);

	if (!file.is_open())
		return false;

	string line;

	while (getline(file, line)) {
			
		if (line.empty())
			continue;

		int day, month, year;
		char dot1, dot2;
		stringstream ss(line);
		if (!(ss >> day >> dot1 >> month >> dot2 >> year))
			continue;
			
		if (dot1 != '.' || dot2 != '.' || !ss.eof())
			continue;

		if (!isvaliddate(day, month, year));
			continue;

			tm holiday = {};
			holiday.tm_mday = day;
			holiday.tm_mon = month - 1;
			holiday.tm_year = year - 1900;

			mktime(&holiday);

			holidays.insert(dateKey(holiday));
	}
	return true;
}

bool isWorkingDay(const tm& date, const set<int>& holidays) {
	return isweekday(date) &&
		holidays.find(dateKey(date)) == holidays.end();
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char repeat;

	set<int> holidays;

	if (!loadHolidays("holidays.txt", holidays)) {
		cout << "Помилка: не виходить відкрити holidays.txt" << endl;
		return 1;
	}

	do {
		string inputdate;
		int workday;
		cout << "Вставте дату (ДД.ММ.РРРР): ";
		cin >> inputdate;
		cout << "Вставте кількість робочих днів: ";
		cin >> workday;

		//перевірка кількості робочих днів
		if (cin.fail() || workday < 0) {
			cout << "Помилка: кількість днів має бути більше нуля.";
			return 1;
		}
		else {

			//розбір дати
			int day, month, year;
			char dot1, dot2;

			stringstream ss(inputdate);

			if (!(ss >> day >> dot1 >> month >> dot2 >> year) || dot1 != '.' || dot2 != '.' || !ss.eof()) {
				cout << "Помилка: некорректний формат дати.";
			}
			//перевірка дати
			else if (!isvaliddate(day, month, year)) {
				cout << "Такої дати не існує.";
			}
			else {
				//створення дати
				tm date = {};
				date.tm_mday = day;
				date.tm_mon = month - 1;
				date.tm_year = year - 1900;

				mktime(&date);

				//додавання робочих днів
				int daysadded = 0;

				while (daysadded < workday) {
					date.tm_mday++;
					mktime(&date);

					if (isWorkingDay(date, holidays)) {
						daysadded++;
					}
				}
				//виведення результату
				cout << "Результат: "
					<< setfill('0') << setw(2) << date.tm_mday << "."
					<< setw(2) << date.tm_mon + 1 << "."
					<< setw(4) << date.tm_year + 1900
					<< endl;
			}
		}
		//запит на повторення
		cout << "Повторити?(y/n): ";
		cin >>repeat;

		while (repeat != 'y' && repeat != 'Y' &&
			repeat != 'n' && repeat != 'N')
		{
			cout << "Помилка: впишіть Y або N";
			cin >> repeat;
		}
	} while (repeat == 'y' || repeat == 'Y');

	return 0;
}