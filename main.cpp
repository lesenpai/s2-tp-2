#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <algorithm>
using namespace std;

void do_main();
shared_ptr<double> getRandomArr(int& len);
shared_ptr<double> parseInput(string& input, int& len, string& path);

void printArr(shared_ptr<double>& arr, int len);
bool createArray(shared_ptr<double>& arr, int& len, string& path_file);
void menu(shared_ptr<double>& arr, int len);
void push_back(shared_ptr<double>& arr, int& len);
void deleteElements(shared_ptr<double>& arr, int& len, int from, int to);
void swapElements(shared_ptr<double>& arr, int& len, int i1, int i2);
void sortArr(shared_ptr<double>& arr, int& len, string& order);
double average(const shared_ptr<double>& arr, int& len);
double maxDiff(const shared_ptr<double>& arr, int& len);
double standardDeviation(shared_ptr<double>& arr, int& len);

double fRand(double fMin, double fMax);
bool isNumber(const string& s);
void SetTextColor(const int k);
void getWord(string& str);
string trimLeft(const string& str);
string trimRight(const string& str);
string trim(const string& str);

const int COLOR_STD = 7; // 7/15
const int COLOR_ERR = 12; // 4/12
const int COLOR_LOG = 11; // 11/30/31
const int COLOR_INPUT = 10; // 2/10
const int COLOR_QUEST = 14;

int main() {
	setlocale(LC_ALL, "ru");

	do_main();
	system("pause");
}

void do_main() {
	shared_ptr<double> arr;
	string path = "user_array.txt";
	int length = 0;
	string input;

	while (true) {
		cout << "Начать/выйти/cls - (go/q/cls): ";
		SetTextColor(COLOR_INPUT);
		getline(cin, input);
		SetTextColor(COLOR_STD);
		if (input == "go") {
			// create array: input / default
			if (createArray(arr, length, path)) {
				menu(arr, length);
			}
		} else if (input == "q") break;
		else if (input == "cls") system("cls");
		else {
			SetTextColor(COLOR_ERR);
			cout << "Ошибка! Неправильный ввод\n";
			SetTextColor(COLOR_STD);
			continue;
		}
	}
}

void SetTextColor(const int k) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k);
}

void getWord(string& str) {
	cin >> str;
	cin.ignore(INT_MAX, '\n');
}

string trimRight(const string& str) {
	string s = str;
	string::size_type pos = s.find_last_not_of(' ');
	return s.erase(pos + 1);
}

string trimLeft(const string& str) {
	string s = str;
	string::size_type pos = s.find_first_not_of(' ');
	return s.erase(0, pos);
}

string trim(const string& str) {
	return trimRight(trimLeft(str));
}

shared_ptr<double> parseInput(string& input, int& len, string& path) {

	shared_ptr<double> arr = NULL;
	string word;
	vector<string> words;
	ofstream fwrite;
	ifstream fread;

	// string -> file.txt
	fwrite.open(path);
	if (fwrite.is_open()) {
		fwrite << input;
	} else {
		SetTextColor(COLOR_ERR);
		cout << "Ошибка открытия " << path << "!" << endl;
		SetTextColor(COLOR_STD);
	}
	fwrite.close();

	// file.txt -> array
	fread.open(path);
	if (fread.is_open()) {
		while (!fread.eof()) {
			fread >> word;
			if (isNumber(word)) {
				words.push_back(word);
			}
		}

		if (words.size()) {
			len = words.size();
			arr = (shared_ptr<double>)(new double[len]());
			for (int i = 0; i < len; i++) {
				arr.get()[i] = stod(words[i]);
			}
		}
	} else {
		SetTextColor(COLOR_ERR);
		cout << "Ошибка открытия " << path << "!" << endl;
		SetTextColor(COLOR_STD);
	}

	return arr;
}

shared_ptr<double> getRandomArr(int& len) {
	shared_ptr<double> arr = (shared_ptr<double>)(new double[len]());

	for (int i = 0; i < len; i++) {
		arr.get()[i] = round(fRand(-50, 50) * 100) / 100;
	}

	return arr;
}

bool isNumber(const string& str) {
	string s = trim(str);
	try {
		string stod_s = to_string(stod(s));
		stod_s = stod_s.substr(0, s.size());
		return stod_s == s;
	} catch (const exception&) {
		return false;
	}
}

void printArr(shared_ptr<double>& arr, const int len) {
	for (int i = 0; i < len; i++) {
		cout << arr.get()[i] << " ";
	}
	cout << endl;
}

bool createArray(shared_ptr<double>& arr, int& len, string& path) {
	string input;

	while (true) {
		cout << "[Ввести/сгенерировать массив]/назад (y/n/q): ";
		SetTextColor(COLOR_INPUT);
		getWord(input);
		SetTextColor(COLOR_STD);

		if (input == "y") {
			cout << "Введите массив (разделитель - пробел):\n> ";
			SetTextColor(COLOR_INPUT);
			getline(cin, input);
			SetTextColor(COLOR_STD);

			if (input == "") {
				SetTextColor(COLOR_ERR);
				cout << "Ошибка! Пустая строка\n";
				SetTextColor(COLOR_STD);
				continue;
			}

			// "нормальный" user ввод
			if (parseInput(input, len, path) != NULL) {
				arr = parseInput(input, len, path);
				SetTextColor(COLOR_LOG);
				cout << "Массив: ";
				printArr(arr, len);
				SetTextColor(COLOR_STD);
				return true;
			} else {
				SetTextColor(COLOR_ERR);
				cout << "Ошибка! Неудалось получить ни одного числа из вашего ввода!\n";
				SetTextColor(COLOR_STD);
				continue;
			}
		}

		else if (input == "n") {
			cout << "Введите размер массива: ";
			SetTextColor(COLOR_INPUT);
			getWord(input);
			SetTextColor(COLOR_STD);
			if (isNumber(input) && stoi(input) > 0) {
				len = stoi(input);
				arr = getRandomArr(len);
				SetTextColor(COLOR_LOG);
				cout << "Массив: ";
				printArr(arr, len);
				SetTextColor(COLOR_STD);
				return true;
			} else {
				SetTextColor(COLOR_ERR);
				cout << "Ошибка! Неправильный ввод\n";
				SetTextColor(COLOR_STD);
				return false;
			}
		} else if (input == "q") {
			return false;
		} else {
			SetTextColor(COLOR_ERR);
			cout << "Ошибка! Неправильный ввод\n";
			SetTextColor(COLOR_STD);
			continue;
		}
	}
}

double fRand(double fMin, double fMax) {
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void menu(shared_ptr<double>& arr, int len) {
	string input;
	while (true) {
		const double AVERAGE = average(arr, len);
		const double STANDARD_DEVIATION = standardDeviation(arr, len);
		const double MAX_DIFF = maxDiff(arr, len);
		const double AVERAGE_DIFF = AVERAGE - STANDARD_DEVIATION;

		SetTextColor(COLOR_QUEST);
		cout << "Что вы хотите сделать?\n";
		SetTextColor(COLOR_STD);
		cout << "\tПоказать массив - 0 | "
			<< "Добавить элемент(-ы) - 1\n"
			<< "\tУдалить элемент(-ы) - 2 | "
			<< "Поменять элементы местами - 3\n"
			<< "\tСортировать - 4 | "
			<< "Среднее арифметическое - 5\n"
			<< "\tМаксимальная разность - 6 | "
			<< "Среднеквадратическое отклонение - 7\n"
			<< "\tРазница средних - 8\n"
			<< "\tВыход - q | "
			<< "очистить экран - cls\n"
			<< "> ";
		SetTextColor(COLOR_INPUT);
		getWord(input);
		SetTextColor(COLOR_STD);

		if (input == "0") {
			SetTextColor(COLOR_LOG);
			cout << "Массив: ";
			printArr(arr, len);
			SetTextColor(COLOR_STD);
		} else if (input == "1") {
			push_back(arr, len);
			SetTextColor(COLOR_LOG);
			cout << "Массив: ";
			printArr(arr, len);
			SetTextColor(COLOR_STD);
		} else if (input == "2") {
			string s_from;
			int from, to;
			bool ok_from = false, ok_to = false;

			cout << "Удалить элемент(-ы),\n\tначиная с: ";
			SetTextColor(COLOR_INPUT);
			getWord(input);
			SetTextColor(COLOR_STD);
			ok_from = (isNumber(input) && stoi(input) > 0 && stoi(input) <= len);
			s_from = input;
			cout << "\tпо: ";
			SetTextColor(COLOR_INPUT);
			getWord(input);
			SetTextColor(COLOR_STD);
			ok_to = (isNumber(input) && stoi(input) >= stoi(s_from) && stoi(input) > 0 && stoi(input) <= len);

			if (ok_from && ok_to) {
				from = stoi(s_from) - 1;
				to = stoi(input) - 1;
				deleteElements(arr, len, from, to);
				SetTextColor(COLOR_LOG);
				cout << "Массив: ";
				printArr(arr, len);
				SetTextColor(COLOR_STD);
			} else {
				SetTextColor(COLOR_ERR);
				cout << "Ошибка! Неправильный ввод\n";
				SetTextColor(COLOR_STD);
			}
		} else if (input == "3") {
			string s_i1, s_i2;

			cout << "1-й элемент: ";
			SetTextColor(COLOR_INPUT);
			getWord(s_i1);
			SetTextColor(COLOR_STD);
			cout << "2-й элемент: ";
			SetTextColor(COLOR_INPUT);
			getWord(s_i2);
			SetTextColor(COLOR_STD);
			if ((isNumber(s_i1) && stoi(s_i1) >= 0 && stoi(s_i1) < len)
				&& (isNumber(s_i2) && stoi(s_i2) >= 0 && stoi(s_i2) < len)) {
				swapElements(arr, len, stoi(s_i1), stoi(s_i2));
				SetTextColor(COLOR_LOG);
				cout << "Массив: ";
				printArr(arr, len);
				SetTextColor(COLOR_STD);
			} else {
				SetTextColor(COLOR_ERR);
				cout << "Ошибка! Неправильный ввод\n";
				SetTextColor(COLOR_STD);
			}
		} else if (input == "4") {
			while (true) {
				cout << "Порядок сортировки (up/down): ";
				SetTextColor(COLOR_INPUT);
				getWord(input);
				SetTextColor(COLOR_STD);

				if (input == "up") {
					sortArr(arr, len, input);
					SetTextColor(COLOR_LOG);
					cout << "Массив: ";
					printArr(arr, len);
					SetTextColor(COLOR_STD);
					break;
				} else if (input == "down") {
					sortArr(arr, len, input);
					SetTextColor(COLOR_LOG);
					cout << "Массив: ";
					printArr(arr, len);
					SetTextColor(COLOR_STD);
					break;
				} else {
					SetTextColor(COLOR_ERR);
					cout << "Ошибка! Не правильный ввод\n";
					SetTextColor(COLOR_STD);
					continue;
				}
			}
		} else if (input == "5") {
			SetTextColor(COLOR_LOG);
			cout << "Среднее арифметическое: " << AVERAGE << endl;
			SetTextColor(COLOR_STD);
		} else if (input == "6") {
			SetTextColor(COLOR_LOG);
			cout << "Максимальная разность: " << MAX_DIFF << endl;
			SetTextColor(COLOR_STD);
		} else if (input == "7") {
			SetTextColor(COLOR_LOG);
			cout << "Среднеквадратическое отклонение: " << STANDARD_DEVIATION << endl;
			SetTextColor(COLOR_STD);
		} else if (input == "8") {
			SetTextColor(COLOR_LOG);
			cout << "Разница средних: " << AVERAGE_DIFF << endl;
			SetTextColor(COLOR_STD);
		} else if (input == "q") {
			SetTextColor(COLOR_LOG);
			cout << "Вышли из menu\n";
			SetTextColor(COLOR_STD);
			break;
		}
		else if (input == "cls") system("cls");
		else {
			SetTextColor(COLOR_ERR);
			cout << "Ошибка! Неправильный ввод\n";
			SetTextColor(COLOR_STD);
			continue;
		}
	}
}

void push_back(shared_ptr<double>& arr, int& len) {
	string input;
	string path_tmp = "temp.txt";
	fstream file;
	shared_ptr<double> prev = NULL;
	shared_ptr<double> toAdd = NULL;
	int toAdd_len = 0;

	cout << "Введите элементы, которые хотите добавить:\n> ";
	SetTextColor(COLOR_INPUT);
	getline(cin, input);
	SetTextColor(COLOR_STD);
	toAdd = parseInput(input, toAdd_len, path_tmp);
	if (toAdd == NULL) {
		SetTextColor(COLOR_ERR);
		cout << "Ошибка! Неудалось считать ни один элемент!\n";
		SetTextColor(COLOR_STD);
		return;
	} else {
		prev = (shared_ptr<double>)(new double[len]());
		for (int i = 0; i < len; i++) {
			prev.get()[i] = arr.get()[i];
		}
	}

	arr = NULL;
	arr = (shared_ptr<double>)(new double[len + toAdd_len]());
	for (int i = 0; i < len; i++) {
		arr.get()[i] = prev.get()[i];
	}
	for (int i = len, j = 0; i < len + toAdd_len; i++, j++) {
		arr.get()[i] = toAdd.get()[j];
	}
	len += toAdd_len;
}

void deleteElements(shared_ptr<double>& arr, int& len, int from, int to) {
	int new_len = len - (to - from + 1);
	shared_ptr<double> newArr = (shared_ptr<double>)(new double[new_len]());

	int i = 0;
	for (; i < from; i++) {
		newArr.get()[i] = arr.get()[i];
	}
	for (int j = i, i = to + 1; i < len; j++, i++) {
		newArr.get()[j] = arr.get()[i];
	}

	arr = NULL;
	arr = newArr;
	len = new_len;
}

void swapElements(shared_ptr<double>& arr, int& len, int i1, int i2) {
	swap(arr.get()[i1], arr.get()[i2]);
}

void sortArr(shared_ptr<double>& arr, int& len, string& order) {
	// вставками
	if (order == "up") {
		for (int i = 1; i < len; i++)
			for (int j = i; j > 0 && arr.get()[j - 1] > arr.get()[j]; j--)
				swap(arr.get()[j - 1], arr.get()[j]);
	} else if (order == "down") {
		for (int i = 1; i < len; i++)
			for (int j = i; j > 0 && arr.get()[j - 1] < arr.get()[j]; j--)
				swap(arr.get()[j - 1], arr.get()[j]);
	}
}

double average(const shared_ptr<double>& arr, int& len) {
	double ret = 0;
	for (int i = 0; i < len; i++) {
		ret += arr.get()[i];
	}
	return ret / len;
}

double maxDiff(const shared_ptr<double>& arr, int& len) {
	double min = arr.get()[0], max = arr.get()[0];

	for (int i = 0; i < len; i++) {
		if (arr.get()[i] > max) max = arr.get()[i];
		if (arr.get()[i] < min) min = arr.get()[i];
	}

	return max - min;
}

double standardDeviation(shared_ptr<double>& arr, int& len) {
	double a_sum = 0, a;
	// a
	for (int i = 0; i < len; i++) {
		a_sum += arr.get()[i];
	}
	a = a_sum / len;
	// (sigma)
	a_sum = 0;
	for (int i = 0; i < len; i++) {
		a_sum += pow(arr.get()[i] - a, 2);
	}
	return sqrt(a_sum / len);
}

/* made by LP */