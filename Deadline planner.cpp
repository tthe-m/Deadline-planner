#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <set>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <locale>

using namespace std;

// перевірка, чи є дата коректною
bool isValidDate(int day, int month, int year) {
    if (year < 1 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    tm date = {};
    date.tm_mday = day;
    date.tm_mon = month - 1;
    date.tm_year = year - 1900;
    date.tm_isdst = -1;

    if (mktime(&date) == -1) {
        return false;
    }

    return date.tm_mday == day &&
        date.tm_mon == month - 1 &&
        date.tm_year == year - 1900;
}

// розбір рядка формату ДД.ММ.РРРР та перевірка коректності дати
bool parseDate(const string& text, int& day, int& month, int& year) {
    char dot1 = '\0', dot2 = '\0';
    stringstream ss(text);

    if (!(ss >> day >> dot1 >> month >> dot2 >> year)) {
        return false;
    }

    if (dot1 != '.' || dot2 != '.' || !(ss >> ws).eof()) {
        return false;
    }

    return isValidDate(day, month, year);
}

// перетворення дати у числовий ключ РРРРММДД для швидкого пошуку
int dateKey(const tm& date) {
    return (date.tm_year + 1900) * 10000 + (date.tm_mon + 1) * 100 + date.tm_mday;
}

// перевірка, чи є день буднім (пн–пт)
bool isWeekday(const tm& date) {
    // 0 - неділя, 6 - субота
    return date.tm_wday != 0 && date.tm_wday != 6;
}

// завантаження святкових днів із файлу
bool loadHolidays(const string& filename, set<int>& holidays) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    holidays.clear();
    string line;

    while (getline(file, line)) {
        stringstream check(line);
        string trimmed;
        if (!(check >> trimmed)) {
            continue;
        }

        int day = 0, month = 0, year = 0;
        if (!parseDate(line, day, month, year)) {
            continue;
        }

        tm holiday = {};
        holiday.tm_mday = day;
        holiday.tm_mon = month - 1;
        holiday.tm_year = year - 1900;
        holiday.tm_isdst = -1;

        if (mktime(&holiday) != -1) {
            holidays.insert(dateKey(holiday));
        }
    }

    return true;
}

// перевірка, чи є день робочим (будній день і не свято)
bool isWorkingDay(const tm& date, const set<int>& holidays) {
    return isWeekday(date) && holidays.find(dateKey(date)) == holidays.end();
}

// очищення стану та буфера введення після помилки
void clearCinBuffer() {
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
}

int main() {
    try {
        locale::global(locale("ukr"));
    }
    catch (const exception&) {
        // ігноруємо, якщо локаль не вдалося застосувати
    }

    set<int> holidays;
    if (!loadHolidays("holidays.txt", holidays)) {
        cout << "Помилка: не вдається відкрити holidays.txt" << endl;
        return 1;
    }

    char repeat = 'n';

    do {
        string inputDate;
        int workday = 0;

        cout << "Впишіть дату (ДД.ММ.РРРР): ";
        cin >> inputDate;

        cout << "Впишіть кількість робочих днів: ";
        if (!(cin >> workday)) {
            cout << "Помилка: невірна кількість робочих днів." << endl;
            clearCinBuffer();
        }
        else {
            int day = 0, month = 0, year = 0;

            if (!parseDate(inputDate, day, month, year)) {
                cout << "Помилка: некоректний формат або неіснуюча дата." << endl;
            }
            else {
                tm date = {};
                date.tm_mday = day;
                date.tm_mon = month - 1;
                date.tm_year = year - 1900;
                date.tm_isdst = -1;
                mktime(&date);

                int direction = (workday >= 0) ? 1 : -1;
                int remaining = abs(workday);

                while (remaining > 0) {
                    date.tm_mday += direction;
                    date.tm_isdst = -1;
                    mktime(&date);

                    if (isWorkingDay(date, holidays)) {
                        remaining--;
                    }
                }

                cout << "Результат: "
                    << setfill('0') << setw(2) << date.tm_mday << "."
                    << setw(2) << (date.tm_mon + 1) << "."
                    << setw(4) << (date.tm_year + 1900)
                    << endl;
            }
        }

        do {
            cout << "Повторити програму? (y/n): ";
            cin >> repeat;
            clearCinBuffer();

            if (repeat != 'y' && repeat != 'Y' && repeat != 'n' && repeat != 'N') {
                cout << "Помилка: впишіть y або n." << endl;
            }
        } while (repeat != 'y' && repeat != 'Y' && repeat != 'n' && repeat != 'N');

    } while (repeat == 'y' || repeat == 'Y');

    return 0;
}