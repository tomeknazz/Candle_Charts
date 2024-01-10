#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct stock_data {
	char date[11];
	double open;
	double high;
	double low;
	double close;
	double volume;
};

void clear_screen()
{
#ifdef _WIN32
	system("cls");
#else
	system("cls")
#endif
}

void exit_program(const stock_data* stock_data_list)
{
	cout << "\033[33m" << "Thank you for using my program! Goodbye!" << "\033[0m" << endl;
	delete[] stock_data_list;
	exit(0);
}

void menu(stock_data* stock_data_list);

ifstream open_file(const string& file_name);

long int read_line_numbers(ifstream& file);

long int read_csv_to_struct(stock_data*& data, const string& file_name);

void generate_chart(const stock_data* stock_data_list, long int size, int height, int records);

void generate_chart_to_file(const stock_data* stock_data_list, long int size, int height, int records, const string& filename);

void displayFileContents(const char* filename);

int main() {

	stock_data* stock_data_list = nullptr;
	menu(stock_data_list);
	return 0;
}

void ascii_art_welcome()
{
	clear_screen();
	cout <<
		" _____                 _ _      _____ _                _       \n"
		"/  __ \\               | | |    /  __ \\ |              | |      \n"
		"| /  \\/ __ _ _ __   __| | | ___| /  \\/ |__   __ _ _ __| |_ ___ \n"
		"| |    / _` | '_ \\ / _` | |/ _ \\ |   | '_ \\ / _` | '__| __/ __|\n"
		"| \\__/\\ (_| | | | | (_| | |  __/ \\__/\\ | | | (_| | |  | |\\__ \\ \n"
		" \\____/\\__,_|_| |_|\\__,_|_|\\___|\\____/_| |_|\\__,_|_|   \\__|___/\n"
		"	Tomasz Nazar            197613            ACiR3                    \n";

	cout << "\033[33m" << "Choose an option:" << "\033[0m" << endl;;
	cout << "g. Generate chart" << endl;
	cout << "q. Exit" << endl;
}

void menu(stock_data* stock_data_list)
{
	ascii_art_welcome();
	char choice;
	cin >> choice;
	while (choice != 'g' and choice != 'q')
	{
		cout << "Enter correct option" << endl;
		cin >> choice;
	}
	if (choice == 'g')
	{
		char choice1;
		clear_screen();
		cout << "\033[33m" << "Choose an option:" << "\033[0m" << endl;;
		cout << "1. Generate default chart (height: 50 ;width: 200) >> intc_us_data.csv >> chart.txt" << endl;
		cout << "2. Choose your own csv file and parameters" << endl;

		cin >> choice1;
		while (choice1 != '1' and choice1 != '2')
		{
			cout << "Enter correct option" << endl;
			cin >> choice1;
		}
		if (choice1 == '1')
		{
			cout << endl << endl << endl;
			long int size = read_csv_to_struct(stock_data_list, "intc_us_data.csv");
			generate_chart_to_file(stock_data_list, size, 50, 200, "chart.txt");
			generate_chart(stock_data_list, size, 50, 200);
			cout << endl << endl;
			exit_program(stock_data_list);

		}
		else if (choice1 == '2')
		{
			string input_file_name, output_file_name;
			int height, width;
			cout << "Enter the csv file name: ";
			cin >> input_file_name;
			cout << "Enter the output file name: ";
			cin >> output_file_name;
			cout << "Enter desired height: ";
			cin >> height;
			cout << "Enter desired width: ";
			cin >> width;
			clear_screen();
			cout<< "Generating chart from file: " << input_file_name << " to file: " << output_file_name << " with height: " << height << " and width: " << width << endl;
			const long int size = read_csv_to_struct(stock_data_list, input_file_name);
			generate_chart(stock_data_list, size, height, width);
			generate_chart_to_file(stock_data_list, size, height, width, output_file_name);
			
			exit_program(stock_data_list);
		}
	}
	else if (choice == 'q')
	{
		cout << "Exiting..." << endl;
		exit_program(stock_data_list);
	}
	else
	{
		cout << "Wrong choice!" << endl;
		menu(stock_data_list);
	}
}

ifstream open_file(const string& file_name)
{
	// Open the file
	ifstream file(file_name);
	if (!file.is_open())
	{
		cout << "Error opening the file.\n";
		exit(1);
	}
	return file;
}

long int read_line_numbers(ifstream& file)
{
	long int line_count = 0;
	// Read the file line by line
	string line;
	while (getline(file, line)) {
		++line_count;
	}

	// Reset the file position
	file.clear();
	file.seekg(0, ios::beg);

	return line_count;
}

long int read_csv_to_struct(stock_data*& data, const string& file_name)
{
	// Open the CSV file
	ifstream file = open_file(file_name);
	const long int line_count = read_line_numbers(file);
	data = new stock_data[line_count];
	// Skip the header line
	string header;
	getline(file, header);

	// Read the file line by line
	for (int count = 0; count < line_count; ++count) {
		string line;
		getline(file, line);
		istringstream iss(line);

		// Read each column from the line
		if (iss.getline(data[count].date, sizeof(data[count].date), ',') &&
			getline(iss, line, ',') && (istringstream(line) >> data[count].open) &&
			getline(iss, line, ',') && (istringstream(line) >> data[count].high) &&
			getline(iss, line, ',') && (istringstream(line) >> data[count].low) &&
			getline(iss, line, ',') && (istringstream(line) >> data[count].close) &&
			getline(iss, line, ',') && (istringstream(line) >> data[count].volume)) {
		}
	}

	// Close the file
	file.close();

	return line_count;
}

double get_max(const stock_data* stock_data_list, long int size, int records)
{
	double max = 0; // Initialize max to negative infinity
	for (int i = size - records; i < size; i++)
	{
		if (stock_data_list[i].high > max)
		{
			max = stock_data_list[i].high;
		}
	}
	return max;
}

double get_min(const stock_data* stock_data_list, long int size, int records)
{
	double min = 1000000; // Initialize min to positive infinity
	for (int i = size - records; i < size; i++)
	{
		if (stock_data_list[i].low < min and stock_data_list[i].low>0)
		{
			min = stock_data_list[i].low;
		}
	}
	return min;
}

void generate_chart(const stock_data* stock_data_list, long int size, int height, int records) {
	const double max = get_max(stock_data_list, size, records);
	const double min = get_min(stock_data_list, size, records);
	const double scale = (max - min) / height;

	for (int i = 0; i < height; i++) {
		for (int j = size - records; j < size; j++) {
			const double point = max - scale / 2 - scale * i;

			if (point >= stock_data_list[j].low && point <= stock_data_list[j].high) {
				if (point > stock_data_list[j].open && point > stock_data_list[j].close) {
					cout << "|";
				}
				else if (point < stock_data_list[j].open && point < stock_data_list[j].close) {
					cout << "|";
				}
				else if (stock_data_list[j].close > stock_data_list[j].open) {
					cout << "O";
				}
				else if (stock_data_list[j].close < stock_data_list[j].open) {
					cout << "#";
				}
			}
			else {
				cout << " ";
			}
		}
		cout << endl;
	}
}

void generate_chart_to_file(const stock_data* stock_data_list, long int size, int height, int records, const string& filename) {
	const double max = get_max(stock_data_list, size,records);
	const double min = get_min(stock_data_list, size,records);
	const double scale = (max - min) / height;

	ofstream outputFile(filename); // Open the output file stream

	if (!outputFile.is_open()) {
		cerr << "Error opening file: " << filename << endl;

		// Try to create the file
		ofstream createFile(filename);

		if (createFile.is_open()) {
			cerr << "File created successfully: " << filename << endl;
			createFile.close();
		}
		else {
			cerr << "Error creating file: " << filename << endl;
			return;
		}

		// Try opening the file again
		outputFile.open(filename);
		if (!outputFile.is_open()) {
			cerr << "Error opening file: " << filename << endl;
			return;
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = size - records; j < size; j++) {
			const double point = max - scale / 2 - scale * i;

			if (point >= stock_data_list[j].low && point <= stock_data_list[j].high) {
				if (point > stock_data_list[j].open && point > stock_data_list[j].close) {
					outputFile << "|";
				}
				else if (point < stock_data_list[j].open && point < stock_data_list[j].close) {
					outputFile << "|";
				}
				else if (stock_data_list[j].close > stock_data_list[j].open) {
					outputFile << "O";
				}
				else if (stock_data_list[j].close < stock_data_list[j].open) {
					outputFile << "#";
				}
			}
			else {
				outputFile << " ";
			}
		}
		outputFile << endl;
	}

	outputFile.close(); // Close the output file stream
}
