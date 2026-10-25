#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <Windows.h>

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
//перевірка, чи є день робочим
bool isweekday(const tm& date) {
	//0 - неділя 6 - субота
	return date.tm_mday != 0 && date.tm_wday != 6;
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char repeat;

	do {
		string inputdate;
		int workday;
		cout << "Впишіть дату (ДД.ММ.РРРР): ";
		cin >> inputdate;
		cout << "Впишіть кількість робочх днів: ";
		cin >> workday;

		//перевірка кількості робочих днів
		if (cin.fail()) {
			cout << "Помилка: невірна кількість робочих днів.";
			return 1;
		}
		else {

			//розбір дати
			int day, month, year;
			char dot1, dot2;

			stringstream ss(inputdate);

			if (!(ss >> day >> dot1 >> month >> dot2 >> year) || dot1 != '.' || dot2 != '.' || !ss.eof()) {
				cout << "Помилка: невірни формат дати.";
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
				int daysprocessed = 0;
				int direction = (workday >= 0) ? 1 : -1;

				while (daysprocessed < abs(workday)) {
					date.tm_mday += direction;
					mktime(&date);

					if (isweekday(date)) {
						daysprocessed++;
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
		cout << "Повторити програму?(y/n): ";
		cin >>repeat;

		while (repeat != 'y' && repeat != 'Y' &&
			repeat != 'n' && repeat != 'N')
		{
			cout << "Помилка: впишіть N або Y";
			cin >> repeat;
		}
	} while (repeat == 'y' || repeat == 'Y');

	return 0;
}