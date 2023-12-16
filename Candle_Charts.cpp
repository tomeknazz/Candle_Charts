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

int main() {
	// Open the CSV file
	ifstream file("intc_us_data.csv");
	if (!file.is_open()) {
		cerr << "Error opening the file.\n";
		return 1;
	}

	// Determine the number of lines in the file
	int num_lines = 0;
	string line;
	while (getline(file, line)) {
		++num_lines;
	}

	// Close and reopen the file to reset the file position
	file.clear();
	file.seekg(0, ios::beg);

	// Dynamically allocate the array
	stock_data* stock_data_list = new stock_data[num_lines];

	// Read the file line by line
	int count = 0;
	while (count < num_lines && getline(file, line)) {
		istringstream iss(line);

		// Read each column from the line
		if (iss.getline(stock_data_list[count].date, sizeof(stock_data_list[count].date), ',') &&
			getline(iss, line, ',') && (istringstream(line) >> stock_data_list[count].open) &&
			getline(iss, line, ',') && (istringstream(line) >> stock_data_list[count].high) &&
			getline(iss, line, ',') && (istringstream(line) >> stock_data_list[count].low) &&
			getline(iss, line, ',') && (istringstream(line) >> stock_data_list[count].close) &&
			getline(iss, line, ',') && (istringstream(line) >> stock_data_list[count].volume)) {
			++count;
		}
		else {
			cerr << "Error parsing line: " << line << endl;
		}
	}

	// Close the file
	file.close();

	// Display the read data
	for (int i = 0; i < num_lines; ++i) {
		cout << "Date: " << stock_data_list[i].date << ", Open: " << stock_data_list[i].open
			<< ", High: " << stock_data_list[i].high << ", Low: " << stock_data_list[i].low
			<< ", Close: " << stock_data_list[i].close << ", Volume: " << stock_data_list[i].volume << endl;
	}

	// Don't forget to free the dynamically allocated memory
	delete[] stock_data_list;

	return 0;
}