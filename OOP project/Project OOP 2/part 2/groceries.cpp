#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include "split.h"

using namespace std;

// Global data
vector<class Customer> customers;
vector<class Item> items;
list<class Order> orders;

// Function declarations
size_t find_cust_idx(int cust_id);
size_t find_item_idx(int item_id);

// Customer class
class Customer {
public:
    int id;
    string name, street, city, state, zip, phone, email;
    Customer(int i, string n, string s, string c, string st, string z, string p, string e)
        : id(i), name(n), street(s), city(c), state(st), zip(z), phone(p), email(e) {
    }
    string print_detail() const {
        return "Customer ID #" + to_string(id) + ":\n" +
            name + ", ph. " + phone + ", email: " + email + "\n" +
            street + "\n" + city + ", " + state + " " + zip;
    }
};

// Item class
class Item {
public:
    int id;
    string description;
    double price;
    Item(int i, string d, double p) : id(i), description(d), price(p) {}
    string print_detail() const {
        return description + " @ " + to_string(price);
    }
};

// LineItem class
class LineItem {
public:
    int item_id;
    int quantity;
    LineItem(int id, int qty) : item_id(id), quantity(qty) {}
    friend bool operator<(const LineItem& item1, const LineItem& item2) {
        return item1.item_id < item2.item_id;
    }
};

// Payment abstract base class
class Payment {
protected:
    double amount;
public:
    Payment() : amount(0.0) {}
    virtual ~Payment() {}
    virtual string print_detail() const = 0;
    friend class Order;
};

// Credit class
class Credit : public Payment {
    string card_number, exp_date;
public:
    Credit(string cn, string ed) : card_number(cn), exp_date(ed) {}
    string print_detail() const override {
        return "Paid by Credit card " + card_number + ", exp. " + exp_date;
    }
};

// PayPal class
class PayPal : public Payment {
    string paypal_id;
public:
    PayPal(string pid) : paypal_id(pid) {}
    string print_detail() const override {
        return "Paid by Paypal ID: " + paypal_id;
    }
};

// WireTransfer class
class WireTransfer : public Payment {
    string bank_id, account_id;
public:
    WireTransfer(string bid, string aid) : bank_id(bid), account_id(aid) {}
    string print_detail() const override {
        return "Paid by Wire transfer from Bank ID " + bank_id + ", Account " + account_id;
    }
};

// Order class
class Order {
    int order_number, cust_id;
    string date;
    vector<LineItem> line_items;
    Payment* payment;
public:
    Order(int on, int cid, string d, vector<LineItem> li, Payment* p)
        : order_number(on), cust_id(cid), date(d), line_items(li), payment(p) {
        sort(line_items.begin(), line_items.end());
        payment->amount = 0.0;
        for (const auto& li : line_items) {
            size_t idx = find_item_idx(li.item_id);
            if (idx != static_cast<size_t>(-1)) {
                double item_total = items[idx].price * li.quantity;
                cout << "Order #" << order_number << " - Item " << li.item_id << ": "
                    << li.quantity << " @ " << items[idx].price << " = " << item_total << endl;
                payment->amount += item_total;
            }
            else {
                cout << "Order #" << order_number << " - Item " << li.item_id << " not found in items, skipping\n";
            }
        }
        cout << "Order #" << order_number << " - Total Amount: " << payment->amount << endl;
    }
    ~Order() { delete payment; }
    string print_order() const {
        stringstream ss;
        ss << fixed << setprecision(2);
        ss << "----------------------------------------\n";
        ss << "Order #" << order_number << ", Date: " << date << "\n";
        if (!payment) {
            ss << "Error: No payment information\n";
        }
        else {
            ss << "Amount: $" << payment->amount << ", " << payment->print_detail() << "\n";
        }
        size_t cust_idx = find_cust_idx(cust_id);
        if (cust_idx != static_cast<size_t>(-1)) {
            ss << customers[cust_idx].print_detail() << "\n";
        }
        ss << "\nOrder Detail:\n";
        for (const auto& li : line_items) {
            size_t item_idx = find_item_idx(li.item_id);
            if (item_idx != static_cast<size_t>(-1)) {
                ss << "Item " << li.item_id << ": \"" << items[item_idx].description
                    << "\", " << li.quantity << " @ " << items[item_idx].price << "\n";
            }
        }
        return ss.str();
    }
};

// Function to find customer index by ID
size_t find_cust_idx(int cust_id) {
    for (size_t i = 0; i < customers.size(); i++) {
        if (customers[i].id == cust_id) return i;
    }
    return static_cast<size_t>(-1);
}

// Function to find item index by ID
size_t find_item_idx(int item_id) {
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].id == item_id) return i;
    }
    return static_cast<size_t>(-1);
}

// Read customers
void read_customers(const string& filename) {
    ifstream file(filename);
    string line;
    if (!file.is_open()) {
        cout << "Error opening customers file: " << filename << endl;
        return;
    }
    while (getline(file, line)) {
        vector<string> fields = split(line, ',');
        if (fields.size() == 8 && !fields[0].empty()) {
            try {
                customers.emplace_back(stoi(fields[0]), fields[1], fields[2], fields[3],
                    fields[4], fields[5], fields[6], fields[7]);
            }
            catch (const std::exception& e) {
                cout << "Error parsing customer ID in line: " << line << endl;
            }
        }
    }
    file.close();
}

// Read items
void read_items(const string& filename) {
    ifstream file(filename);
    string line;
    if (!file.is_open()) {
        cout << "Error opening items file: " << filename << endl;
        return;
    }
    while (getline(file, line)) {
        vector<string> fields = split(line, ',');
        if (fields.size() == 3) {
            try {
                items.emplace_back(stoi(fields[0]), fields[1], stod(fields[2]));
            }
            catch (const std::exception& e) {
                cout << "Error parsing item data in line: " << line << endl;
            }
        }
    }
    file.close();
}

// Read orders
void read_orders(const string& filename) {
    ifstream file(filename);
    string line1, line2;
    if (!file.is_open()) {
        cout << "Error opening orders file: " << filename << endl;
        return;
    }
    int line_count = 0;
    int order_count = 0;
    while (getline(file, line1)) {
        line_count++;
        if (!getline(file, line2)) break;
        line_count++;
        vector<string> fields1 = split(line1, ',');
        vector<string> fields2 = split(line2, ',');
        if (fields1.size() < 3 || fields2.size() < 2) continue;

        int cust_id = stoi(fields1[0]);
        int order_num = stoi(fields1[1]);
        string date = fields1[2];
        vector<LineItem> line_items;
        cout << "Reading Order #" << order_num << " with " << (fields1.size() - 3) << " items:\n";
        for (size_t i = 3; i < fields1.size(); i++) {
            vector<string> pair = split(fields1[i], '-');
            if (pair.size() == 2) {
                int item_id = stoi(pair[0]);
                int qty = stoi(pair[1]);
                cout << "Order #" << order_num << " - Parsed Item " << item_id << ": " << qty << endl;
                line_items.emplace_back(item_id, qty);
            }
        }
        Payment* payment = nullptr;
        int payment_code = stoi(fields2[0]);
        if (payment_code == 1 && fields2.size() == 3) {
            payment = new Credit(fields2[1], fields2[2]);
        }
        else if (payment_code == 2 && fields2.size() == 2) {
            payment = new PayPal(fields2[1]);
        }
        else if (payment_code == 3 && fields2.size() == 3) {
            payment = new WireTransfer(fields2[1], fields2[2]);
        }
        if (payment) {
            orders.emplace_back(order_num, cust_id, date, line_items, payment);
            order_count++;
        }
    }
    file.close();
    cout << "Total lines in orders.txt: " << line_count << endl;
    cout << "Total orders parsed: " << order_count << endl;
}

// Main function
int main() {
    read_customers("customers.txt");
    cout << "Read " << customers.size() << " customers\n";
    read_items("items.txt");
    cout << "Read " << items.size() << " items\n";
    read_orders("orders.txt");
    cout << "Read " << orders.size() << " orders\n";
    ofstream ofs("order_report.txt");
    if (!ofs) {
        cout << "Error opening output file!" << endl;
        cin.get();
        return 1;
    }
    int written_orders = 0;
    for (auto it = orders.begin(); it != orders.end(); ++it) {
        ofs << it->print_order();
        written_orders++;
        if (!ofs.good()) {
            cout << "Error writing to file after " << written_orders << " orders\n";
            break;
        }
        if (next(it) != orders.end()) ofs << "\n";
    }
    cout << "Wrote " << written_orders << " orders to order_report.txt\n";
    ofs.close();
    cout << "Program finished. Check order_report.txt. Press Enter to exit..." << endl;
    cin.get();
    return 0;
}