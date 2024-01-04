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

void exit_program(stock_data* stock_data_list)
{
	clear_screen();
	delete[] stock_data_list;
	exit(0);
}

void menu(stock_data *stock_data_list);

ifstream open_file(const string& file_name);

long int read_line_numbers(ifstream& file);

void read_csv_to_struct(stock_data*& data, const string& file_name);

void generate_chart(stock_data* stock_data_list);



int main() {
	stock_data* stock_data_list = nullptr;

	menu(stock_data_list);


	return 0;
}

void menu(stock_data* stock_data_list)
{
	clear_screen();
	cout <<
		" _____                 _ _      _____ _                _       \n"
		"/  __ \\               | | |    /  __ \\ |              | |      \n"
		"| /  \\/ __ _ _ __   __| | | ___| /  \\/ |__   __ _ _ __| |_ ___ \n"
		"| |    / _` | '_ \\ / _` | |/ _ \\ |   | '_ \\ / _` | '__| __/ __|\n"
		"| \\__/\\ (_| | | | | (_| | |  __/ \\__/\\ | | | (_| | |  | |\\__ \\ \n"
		" \\____/\\__,_|_| |_|\\__,_|_|\\___|\\____/_| |_|\\__,_|_|   \\__|___/\n"
		"		  Tomasz Nazar            197613            ACiR3                    \n";

	cout << "g. Generate chart" << endl;
	cout << "q. Exit" << endl;
	char choice;
	cin >> choice;
	if (choice == 'g')
	{
		generate_chart(stock_data_list);
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

void read_csv_to_struct(stock_data*& data, const string& file_name)
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
			// Data successfully read
		}
		else {
			cerr << "Error parsing line: " << line << endl;
		}
	}

	// Close the file
	file.close();
}

void generate_chart(stock_data *stock_data_list)
{
	clear_screen();
	int choice;
	cout << "1. Generate default chart" << endl;
	cout << "2. Choose your own csv file" << endl;
	cin >> choice;
	if (choice == 1)
	{
		read_csv_to_struct(stock_data_list, "intc_us_data.csv");
	}
	else if (choice == 2)
	{
		string file_name;
		cout << "Enter the file name: ";
		cin >> file_name;
		cout << "Generating chart..." << endl;
		read_csv_to_struct(stock_data_list, file_name);
	}
	else
	{
		cout << "Wrong choice!" << endl;
		generate_chart(stock_data_list);
	}
	
	
	
}