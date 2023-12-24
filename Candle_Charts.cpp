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
	std::string line;
	while (getline(file, line)) {
		++line_count;
	}

	// Reset the file position
	file.clear();
	file.seekg(0, std::ios::beg);

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
		std::string line;
		getline(file, line);
		std::istringstream iss(line);

		// Read each column from the line
		if (iss.getline(data[count].date, sizeof(data[count].date), ',') &&
			getline(iss, line, ',') && (std::istringstream(line) >> data[count].open) &&
			getline(iss, line, ',') && (std::istringstream(line) >> data[count].high) &&
			getline(iss, line, ',') && (std::istringstream(line) >> data[count].low) &&
			getline(iss, line, ',') && (std::istringstream(line) >> data[count].close) &&
			getline(iss, line, ',') && (std::istringstream(line) >> data[count].volume)) {
			// Data successfully read
		}
		else {
			std::cerr << "Error parsing line: " << line << std::endl;
		}
	}

	// Close the file
	file.close();
}

void print_stock_data(stock_data* data, long int line_count)
{

	for (int i = 0; i < line_count - 1; ++i)
	{
		cout << "Date: " << data[i].date << ", Open: " << data[i].open
			<< ", High: " << data[i].high << ", Low: " << data[i].low
			<< ", Close: " << data[i].close << endl;
	}
}

int main() {
	string file_name;
	cout << "Enter the file name: ";
	cin >> file_name;
	ifstream file = open_file(file_name);
	// Create an array to store StockData
	stock_data* stock_data_list = nullptr;
	read_csv_to_struct(stock_data_list, file_name);
	print_stock_data(stock_data_list, read_line_numbers(file));


	delete[] stock_data_list;
	return 0;
}
