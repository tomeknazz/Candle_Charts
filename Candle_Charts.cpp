#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

struct stock_data
{
	char date[11];
	double open;
	double high;
	double low;
	double close;
	double volume;
};

void clear_screen();
void ascii_art_welcome();
void log_message(const string& message);
void exit_program(const stock_data* stock_data_list);
void menu(stock_data* stock_data_list);
ifstream open_file(const string& file_name);
long int read_line_numbers(ifstream& file);
long int read_csv_to_struct(stock_data*& data, const string& file_name);
void generate_chart(const stock_data* stock_data_list, long int size, int height, int records);
void generate_chart_to_file(const stock_data* stock_data_list, long int size, int height, int records, const string& filename);
double get_max(const stock_data* stock_data_list, long int size, int records);
double get_min(const stock_data* stock_data_list, long int size, int records);
char get_valid_option(const char* valid_options);

int main()
{
	log_message("Program started");
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

	cout << "\033[33m" << "Choose an option:" << "\033[0m" << endl;
	cout << "g. Generate chart" << endl;
	cout << "q. Exit" << endl;
}

void menu(stock_data* stock_data_list)
{
	ascii_art_welcome();
	const char choice = get_valid_option("gq");
	log_message("User chose option: " + string(1, choice));
	if (choice == 'g')
	{
		clear_screen();
		cout << "\033[33m" << "Choose an option:" << "\033[0m" << endl;
		cout << "1. Generate default chart (height: 50 ;width: 200) >> intc_us_data.csv >> chart.txt" << endl;
		cout << "2. Choose your own csv file and parameters" << endl;
		cout << "3. Back" << endl;

		const char choice1 = get_valid_option("123");

		if (choice1 == '1')
		{
			log_message("User chose option: " + string(1, choice1));
			log_message("Started generating chart");
			cout << endl << endl << endl;
			const long int size = read_csv_to_struct(stock_data_list, "intc_us_data.csv");
			generate_chart_to_file(stock_data_list, size, 50, 200, "chart.txt");
			generate_chart(stock_data_list, size, 50, 200);
			cout << endl << endl;
			exit_program(stock_data_list);
		}
		else if (choice1 == '2')
		{
			log_message("User chose option: " + string(1, choice1));
			log_message("Asking for specifics");
			string input_file_name, output_file_name;
			int height, width;
			cout << "Enter the csv file name: ";
			cin >> input_file_name;
			log_message("User  chose to generate chart from file: " + input_file_name);
			cout << "Enter the output file name: ";
			cin >> output_file_name;
			log_message("User chose to generate chart to file: " + output_file_name);
			cout << "Enter desired height: ";
			cin >> height;
			log_message("User chose height: " + to_string(height));
			cout << "Enter desired width: ";
			cin >> width;
			log_message("User chose width: " + to_string(width));
			log_message("Started generating chart");
			clear_screen();
			cout << "Generating chart from file: " << input_file_name << " to file: " << output_file_name << " with height: " << height << " and width: " << width << endl;
			const long int size = read_csv_to_struct(stock_data_list, input_file_name);
			generate_chart_to_file(stock_data_list, size, height, width, output_file_name);
			generate_chart(stock_data_list, size, height, width);
			exit_program(stock_data_list);
		}
		else if (choice1 == '3')
		{
			log_message("User chose option: " + string(1, choice1) + " and returned to main menu");
			menu(stock_data_list);
		}
	}
	else if (choice == 'q')
		exit_program(stock_data_list);
}

ifstream open_file(const string& file_name)
{
	ifstream file(file_name);
	if (!file.is_open())
	{
		cout << "Error opening the file.\n";
		exit(1);
	}
	log_message("File: " + file_name + " has been opened");
	return file;
}

long int read_line_numbers(ifstream& file)
{
	long int line_count = 0;
	string line;
	while (getline(file, line))
		++line_count;
	file.clear();
	file.seekg(0, ios::beg);
	log_message("Number of lines read: " + to_string(line_count));
	return line_count;
}

long int read_csv_to_struct(stock_data*& data, const string& file_name)
{
	ifstream file = open_file(file_name);
	const long int line_count = read_line_numbers(file);
	data = new stock_data[line_count];
	string header;
	getline(file, header);
	for (int count = 0; count < line_count; ++count)
	{
		string line;
		getline(file, line);
		istringstream iss(line);

		if (iss.getline(data[count].date, sizeof(data[count].date), ',') &&
			getline(iss, line, ',') && (istringstream(line) >> data[count].open) &&
			getline(iss, line, ',') && (istringstream(line) >> data[count].high) &&
			getline(iss, line, ',') && (istringstream(line) >> data[count].low) &&
			getline(iss, line, ',') && (istringstream(line) >> data[count].close) &&
			getline(iss, line, ',') && (istringstream(line) >> data[count].volume)) {
		}
	}
	log_message("File: " + file_name + " has been read to struct");
	file.close();
	log_message("File: " + file_name + " has been closed");
	return line_count;
}

double get_max(const stock_data* stock_data_list, const long int size, const int records)
{
	double max = 0;
	for (int i = size - records; i < size; i++)
		if (stock_data_list[i].high > max)
			max = stock_data_list[i].high;
	log_message("Max value established: " + to_string(max));
	return max;
}

double get_min(const stock_data* stock_data_list, const long int size, const int records)
{
	double min = 1000000000000000;
	for (int i = size - records; i < size; i++)
		if (stock_data_list[i].low < min and stock_data_list[i].low > 0)
			min = stock_data_list[i].low;
	log_message("Min value established: " + to_string(min));
	return min;
}

void generate_chart(const stock_data* stock_data_list, const long int size, const int height, const int records)
{
	const double max = get_max(stock_data_list, size, records);
	const double min = get_min(stock_data_list, size, records);
	const double scale = (max - min) / height;
	cout << setw(8) << "Value" << "^" << endl;
	cout << setw(8) << ' ' << "|" << endl;
	for (int i = 0; i < height; i++)
	{
		if (i % 10 == 0)
			cout << setw(7) << max - scale * i << " | ";
		else
			cout << setw(7) << ' ' << " | ";
		for (int j = size - records; j < size; j++)
		{
			const double point = max - scale / 2 - scale * i;
			if (point >= stock_data_list[j].low && point <= stock_data_list[j].high)
			{
				if ((point > stock_data_list[j].open && point > stock_data_list[j].close) || (point < stock_data_list[j].open && point < stock_data_list[j].close))
					cout << "|";
				else if (stock_data_list[j].close > stock_data_list[j].open)
					cout << "O";
				else if (stock_data_list[j].close < stock_data_list[j].open)
					cout << "#";
			}
			else
				cout << " ";
		}
		cout << endl;
	}
	cout << setw(7) << min << " | " << endl;
	cout << setw(8) << ' ';
	for (int i = 0; i <= records; i++)
		cout << "-";
	cout << ">" << "  " << "Day number" << endl;
	cout << setw(4) << ' ';
	cout << stock_data_list[size - records - 2].date;
	cout << setw(records / 2) << stock_data_list[size - records / 2 - 2].date;
	cout << setw(records / 2) << stock_data_list[size - 2].date << endl;
	cout << endl;
	log_message("Chart has been generated to console");
}

void generate_chart_to_file(const stock_data* stock_data_list, const long int size, const int height, const int records, const string& filename)
{
	const double max = get_max(stock_data_list, size, records);
	const double min = get_min(stock_data_list, size, records);
	const double scale = (max - min) / height;
	ofstream output_file(filename); // Open the output file stream
	if (!output_file.is_open())
	{
		ofstream create_file(filename);
		if (create_file.is_open())
			create_file.close();
		else
			return;
	}
	log_message("File: " + filename + " has been opened");
	output_file << setw(8) << "Value" << "^" << endl;
	output_file << setw(8) << ' ' << "|" << endl;
	for (int i = 0; i < height; i++)
	{
		if (i % 10 == 0)
			output_file << setw(7) << max - scale * i << " | ";
		else
			output_file << setw(7) << ' ' << " | ";
		for (int j = size - records; j < size; j++)
		{
			const double point = max - scale / 2 - scale * i;
			if (point >= stock_data_list[j].low && point <= stock_data_list[j].high)
			{
				if ((point > stock_data_list[j].open && point > stock_data_list[j].close) || (point < stock_data_list[j].open && point < stock_data_list[j].close))
					output_file << "|";
				else if (stock_data_list[j].close > stock_data_list[j].open)
					output_file << "O";
				else if (stock_data_list[j].close < stock_data_list[j].open)
					output_file << "#";
			}
			else
				output_file << " ";
		}
		output_file << endl;
	}
	output_file << setw(7) << min << " | " << endl;
	output_file << setw(8) << ' ';
	for (int i = 0; i <= records; i++)
		output_file << "-";
	output_file << ">" << "  " << "Day number" << endl;
	output_file << setw(4) << ' ';
	output_file << stock_data_list[size - records - 2].date;
	output_file << setw(records / 2) << stock_data_list[size - records / 2 - 2].date;
	output_file << setw(records / 2) << stock_data_list[size - 2].date << endl;
	output_file << endl;
	log_message("Chart has been generated to file: " + filename);
	output_file.close(); // Close the output file stream
	log_message("File: " + filename + " has been closed");
}

void log_message(const string& message) {
	ofstream log_file("user_actions.log", ios::app);
	if (!log_file.is_open())
		return;
	const time_t current_time = time(nullptr);
	tm local_time;
	char time_buffer[80];
	if (localtime_s(&local_time, &current_time) != 0)
		return;
	if (strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", &local_time) == 0)
		return;
	log_file << "[" << time_buffer << "] " << message << endl;
	log_file.close();
}

char get_valid_option(const char* valid_options) {
	char choice;
	do {
		cin >> choice;
		cin.ignore(10, '\n');
		if (strchr(valid_options, choice) == nullptr) {
			cout << "Invalid option. Please try again." << endl;
			log_message("User chose invalid option: " + string(1, choice));
		}
	} while (strchr(valid_options, choice) == nullptr);
	return choice;
}

void exit_program(const stock_data* stock_data_list)
{
	cout << "\033[33m" << "Thank you for using my program! Goodbye!" << "\033[0m" << endl;
	delete[] stock_data_list;
	log_message("Program ended");
	exit(0);
}

void clear_screen()
{
#ifdef _WIN32
	system("cls");
#else
	system("cls")
#endif
}