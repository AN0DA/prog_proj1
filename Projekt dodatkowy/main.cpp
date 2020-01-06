#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char* argv[]) {
	long data[3] = { 0, 0, 0 };

	short temp;

	while (true) {
		cin >> temp;
		if (temp == 1 || temp == 2 || temp == 3) {
			data[temp - 1]++;
		}
		else if (temp == 0) {
			break;
		}
		else {
			cout << "Wrong value" << endl;
		}
	}
	int sum = data[0] + data[1] + data[2];

	system("cls");

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	double temp_data, temp_stat;

	//_setmode(_fileno(stdout), _O_U16TEXT);

	while (true) {
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		cout << "1 ";
		temp_data = data[0];
		temp_stat = (temp_data / sum) * columns;
		for (int j = 2; j < (rows / 3); j++) {
			for (int k = 3; k < temp_stat; k++) {
				_setmode(_fileno(stdout), _O_U16TEXT);
				wcout << L"\u2588";
				_setmode(_fileno(stdout), _O_TEXT);
			}
			if (j == rows / 3 - 1)
				cout << "	" << temp_data << endl << "  ";
			else
				cout << endl << "  ";
		}
		cout << endl;

		cout << "2 ";
		temp_data = data[1];
		temp_stat = (temp_data / sum) * columns;
		for (int j = 2; j < (rows / 3); j++) {
			for (int k = 3; k < temp_stat; k++) {
				_setmode(_fileno(stdout), _O_U16TEXT);
				wcout << L"\u2588";
				_setmode(_fileno(stdout), _O_TEXT);
			}
			if (j == rows / 3 - 1)
				cout << "	" << temp_data << endl << "  ";
			else
				cout << endl << "  ";
		}
		cout << endl;

		cout << "3 ";
		temp_data = data[2];
		temp_stat = (temp_data / sum) * columns;
		for (int j = 2; j < (rows / 3); j++) {
			for (int k = 3; k < temp_stat; k++) {
				_setmode(_fileno(stdout), _O_U16TEXT);
				wcout << L"\u2588";
				_setmode(_fileno(stdout), _O_TEXT);
			}
			if (j == rows / 3 - 1)
				cout << "	" << temp_data << endl << "  ";
			else
				cout << endl << "  ";
		}

		system("timeout 60");
		system("cls");
	}

	return 0;
}