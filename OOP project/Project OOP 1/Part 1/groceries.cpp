#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "split.h"

using namespace std;

// Global vectors for customers and items
vector<struct Customer> customers;
vector<struct Item> items;

// Customer struct
struct Customer {
    int id;
    string name;
    string street;
    string city;
    string state;
    string zip;
    string phone;
    string email;
};

// Item struct
struct Item {
    int id;
    string description;
    double price;
};

// Function to read customers from file
void read_customers(const string& filename) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "Error opening customers file!" << endl;
        return;
    }

    while (getline(file, line)) {
        vector<string> fields = split(line, ',');
        if (fields.size() == 8) {
            Customer c;
            c.id = stoi(fields[0]);
            c.name = fields[1];
            c.street = fields[2];
            c.city = fields[3];
            c.state = fields[4];
            c.zip = fields[5];
            c.phone = fields[6];
            c.email = fields[7];
            customers.push_back(c);
        }
    }
    file.close();
}

// Function to read items from file
void read_items(const string& filename) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "Error opening items file!" << endl;
        return;
    }

    while (getline(file, line)) {
        vector<string> fields = split(line, ',');
        if (fields.size() == 3) {
            Item i;
            i.id = stoi(fields[0]);
            i.description = fields[1];
            i.price = stod(fields[2]);
            items.push_back(i);
        }
    }
    file.close();
}

// Function to find customer index by ID
int find_cust_idx(int cust_id) {
    for (size_t i = 0; i < customers.size(); i++) {
        if (customers[i].id == cust_id) {
            return i;
        }
    }
    return -1;
}

// Function to find item index by ID
int find_item_idx(int item_id) {
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].id == item_id) {
            return i;
        }
    }
    return -1;
}

// Function to process one customer's order
void one_customer_order() {
    cout << "There are " << customers.size() << " customers and "
        << items.size() << " items available." << endl;

    int cust_id;
    cout << "Enter customer number: ";
    cin >> cust_id;

    int cust_idx = find_cust_idx(cust_id);
    if (cust_idx == -1) {
        cout << "Customer " << cust_id << " not found!" << endl;
        return;
    }

    int item_count = 0;
    double total_cost = 0.0;
    int item_id;

    do {
        cout << "Enter item number (0 to finish): ";
        cin >> item_id;

        if (item_id != 0) {
            int item_idx = find_item_idx(item_id);
            if (item_idx != -1) {
                cout << items[item_idx].description << " - $"
                    << fixed << setprecision(2) << items[item_idx].price << endl;
                item_count++;
                total_cost += items[item_idx].price;
            }
            else {
                cout << "Item " << item_id << " not found!" << endl;
            }
        }
    } while (item_id != 0);

    cout << "Order complete: " << item_count << " items purchased" << endl;
    cout << "Total cost: $" << fixed << setprecision(2) << total_cost << endl;
}

int main() {
    read_customers("customers.txt");
    read_items("items.txt");
    one_customer_order();
    return 0;
}