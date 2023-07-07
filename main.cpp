#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

struct Field {
    string name;
    int length;
};

struct Orders {
    char orderId[15];
    char orderDate[11];
    char shipDate[11];
    char shipMode[15];
    char customerId[9];
};

struct orderIndexEntry {
    char orderId[15];
    long offset;
};

struct Customers {
    char customerId[9];
    char customerName[21];
    char segment[12];
    char country[14];
    char city[21];
    char state[21];
    int postalCode;
    char region[8];
};

struct customersIndexRecord {
    char customerId[9];
    int offset;
};

struct Products {
    char productId[16];
    char category[16];
    char subcategory[12];
    char productName[121];
};

struct productsIndexEntry {
    char key[16];
    streampos offset;
};

struct Details {
    char orderId[15];
    char productId[16];
    double sales;
    int quantity;
    double discount;
    double profit;

};

bool compareIndexRecords(const customersIndexRecord& record1, const customersIndexRecord& record2) {
    return strcmp(record1.customerId, record2.customerId) < 0;
}
bool compareIndexEntries(const orderIndexEntry& entry1, const orderIndexEntry& entry2) {
    return strcmp(entry1.orderId, entry2.orderId) < 0;
}
bool compareProductsIndexEntries(const productsIndexEntry& entry1, const productsIndexEntry& entry2) {
    return strcmp(entry1.key, entry2.key) < 0;
}

vector<Field> readFieldsFromCSV(const string& csvFile, const string& lengthFile) {
    vector<Field> fields;
    ifstream length(lengthFile);
    if (length.is_open()) {
        string line;
        while (getline(length, line)) {
            string fieldName = line.substr(0, 14);
            string lengthStr = line.substr(14, line.find_last_of(' ') - 14);
            int fieldLength = stoi(lengthStr);
            fields.push_back({ fieldName, fieldLength });
        }
        length.close();
    }
    return fields;
}


vector<string> splitLine(const string& line, char delimiter) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    bool inQuotes = false;
    bool doubleQuotes = false;
    string tokenBuffer;
    int fieldCount = 0;

    while (getline(ss, token, delimiter)) {
        if (!inQuotes) {
            if (token.empty()) {
                tokenBuffer += token;
                tokenBuffer += delimiter;
            }
            else if (token.front() == '"') {
                tokenBuffer += token;
                tokenBuffer += delimiter;
                if (token.back() != '"') {
                    inQuotes = true;
                    doubleQuotes = false;
                }
                else {
                    tokenBuffer = tokenBuffer.substr(1, tokenBuffer.length() - 3);
                    if (fieldCount >= 15 && fieldCount <= 18) {
                        size_t quotePos = tokenBuffer.find("\"\"");
                        if (quotePos != string::npos) {
                            tokenBuffer = tokenBuffer.substr(1, quotePos - 1) + tokenBuffer.substr(quotePos + 2);
                        }
                        tokenBuffer.erase(remove_if(tokenBuffer.begin(), tokenBuffer.end(), [](char c) { return !isdigit(c); }), tokenBuffer.end());
                    }
                    tokens.push_back(tokenBuffer);
                    tokenBuffer.clear();
                }
            }
            else {
                tokens.push_back(token);
            }
        }
        else {
            tokenBuffer += token;
            tokenBuffer += delimiter;
            if (doubleQuotes) {
                tokenBuffer = tokenBuffer.substr(1, tokenBuffer.length() - 3);
                tokens.push_back(tokenBuffer);
                tokenBuffer.clear();
                inQuotes = false;
                doubleQuotes = false;
            }
            else if (token.back() == '"') {
                size_t quotePos = token.find("\"\"");
                if (quotePos != string::npos) {
                    tokenBuffer = tokenBuffer.substr(0, tokenBuffer.length() - 2);
                    tokenBuffer += token.substr(1, quotePos - 1);
                    doubleQuotes = true;
                }
                else {
                    tokenBuffer = tokenBuffer.substr(0, tokenBuffer.length() - 2);
                    if (fieldCount >= 15 && fieldCount <= 18) {
                        tokenBuffer.erase(remove_if(tokenBuffer.begin(), tokenBuffer.end(), [](char c) { return !isdigit(c); }), tokenBuffer.end());
                    }
                    tokens.push_back(tokenBuffer);
                    tokenBuffer.clear();
                    inQuotes = false;
                }
            }
        }

        ++fieldCount;
    }

    if (!tokenBuffer.empty()) {
        tokens.push_back(tokenBuffer);
    }

    return tokens;
}


string padField(const string& field, int length) {
    string paddedField = field;
    if (paddedField.length() > static_cast<size_t>(length)) {
        paddedField = paddedField.substr(0, length);
    }
    else if (paddedField.length() < static_cast<size_t>(length)) {
        paddedField.resize(length, ' ');
    }
    return paddedField;
}


void createCustomersFile(string inputFile) {
    vector<Field> fields = readFieldsFromCSV(inputFile, "length-file.txt");
    vector<string> allRecords;
    ifstream inFile(inputFile);
    ofstream outFile("customers.dat", ios::binary | ios::out);

    if (inFile.is_open() && outFile.is_open()) {
        string line;
        getline(inFile, line);
        while (getline(inFile, line)) {
            vector<string> tokens = splitLine(line, ',');
            string record;
            for (size_t i = 0; i < fields.size(); ++i) {
                if (i == 5 || i == 6 || i == 7 || i == 8 || i == 9 || i == 10 || i == 11 || i == 12) {
                    string paddedField = padField(tokens[i], fields[i].length);
                    record += paddedField;
                }
            }
            allRecords.push_back(record);
            bool exists = false;
            int recordSize = allRecords.size() - 1;
            for (int i = 0; i < recordSize; i++) {
                if (allRecords[i] == record) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                Customers customer;
                for (size_t i = 0; i < fields.size(); ++i) {
                    string paddedField = padField(tokens[i], fields[i].length);
                    if (i == 5) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(customer.customerId, paddedField.c_str(), 8);
                        customer.customerId[8] = '\0';
                    }
                    else if (i == 6) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(customer.customerName, paddedField.c_str(), 20);
                        customer.customerName[20] = '\0';
                    }
                    else if (i == 7) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(customer.segment, paddedField.c_str(), 12);
                        customer.segment[12] = '\0';
                    }
                    else if (i == 8) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(customer.country, paddedField.c_str(), 13);
                        customer.country[13] = '\0';
                    }
                    else if (i == 9) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(customer.city, paddedField.c_str(), 20);
                        customer.city[20] = '\0';
                    }
                    else if (i == 10) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(customer.state, paddedField.c_str(), 20);
                        customer.state[20] = '\0';
                    }
                    else if (i == 11) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        customer.postalCode = stoi(paddedField);
                    }
                    else if (i == 12) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(customer.region, paddedField.c_str(), 7);
                        customer.region[7] = '\0';
                        outFile.write(reinterpret_cast<char*>(&customer), sizeof(Customers));
                    }
                }

            }
        }
        inFile.close();
        outFile.close();
    }
    else {
        cout << "Error opening files." << endl;
    }
}
void createOrdersFile(string inputFile) {
    vector<Field> fields = readFieldsFromCSV(inputFile, "length-file.txt");
    vector<string> allRecords;
    ifstream inFile(inputFile);
    ofstream outFile("orders.dat", ios::binary | ios::out);

    if (inFile.is_open() && outFile.is_open()) {
        string line;
        getline(inFile, line);

        while (getline(inFile, line)) {
            vector<string> tokens = splitLine(line, ',');
            string record;
            for (size_t i = 0; i < fields.size(); ++i) {
                if (i == 1 || i == 2 || i == 3 || i == 4 || i == 5) {
                    string paddedField = padField(tokens[i], fields[i].length);
                    record += paddedField;
                }
            }
            allRecords.push_back(record);
            bool exists = false;
            int recordSize = allRecords.size() - 1;
            for (int i = 0; i < recordSize; i++) {
                if (allRecords[i] == record) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                Orders order;
                for (size_t i = 0; i < fields.size(); ++i) {
                    string paddedField = padField(tokens[i], fields[i].length);
                    if (i == 1) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(order.orderId, paddedField.c_str(), 14);
                        order.orderId[14] = '\0';
                    }
                    else if (i == 2) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(order.orderDate, paddedField.c_str(), 10);
                        order.orderDate[10] = '\0';
                    }
                    else if (i == 3) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(order.shipDate, paddedField.c_str(), 10);
                        order.shipDate[10] = '\0';
                    }
                    else if (i == 4) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(order.shipMode, paddedField.c_str(), 14);
                        order.shipMode[14] = '\0';
                    }
                    else if (i == 5) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(order.customerId, paddedField.c_str(), 8);
                        order.customerId[8] = '\0';
                        outFile.write(reinterpret_cast<char*>(&order), sizeof(Orders));

                    }
                }

            }
        }
        inFile.close();
        outFile.close();
    }
    else {
        cout << "Error opening files." << endl;
    }
}
void createDetailsFile(string inputFile) {
    vector<Field> fields = readFieldsFromCSV(inputFile, "length-file.txt");
    vector<string> allRecords;
    ifstream inFile(inputFile);
    ofstream outFile("details.dat", ios::binary | ios::out);
    if (inFile.is_open() && outFile.is_open()) {
        string line;
        getline(inFile, line);
        while (getline(inFile, line)) {
            vector<string> tokens = splitLine(line, ',');
            string record;
            for (size_t i = 0; i < fields.size(); ++i) {
                if (i == 1 || i == 13 || i == 17 || i == 18 || i == 19 || i == 20) {
                    string paddedField = padField(tokens[i], fields[i].length);
                    record += paddedField;
                }
            }
            allRecords.push_back(record);
            bool exists = false;
            int recordSize = allRecords.size() - 1;
            for (int i = 0; i < recordSize; i++) {
                if (allRecords[i] == record) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                Details details;

                for (size_t i = 0; i < fields.size(); ++i) {
                    string paddedField = padField(tokens[i], fields[i].length);
                    if (i == 1) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(details.orderId, paddedField.c_str(), 14);
                        details.orderId[14] = '\0';
                    }
                    else if (i == 13) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(details.productId, paddedField.c_str(), 15);
                        details.productId[15] = '\0';
                    }
                    else if (i == 17) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        try {
                            details.sales = stod(paddedField);
                        }
                        catch (const invalid_argument& e) {
                        }

                    }
                    else if (i == 18) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        details.quantity = stoi(paddedField);
                    }
                    else if (i == 19) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        details.discount = stod(paddedField);
                    }
                    else if (i == 20) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        details.profit = stod(paddedField);
                        outFile.write(reinterpret_cast<char*>(&details), sizeof(Details));
                    }

                }

            }
        }
        inFile.close();
        outFile.close();
    }
    else {
        cout << "Error opening files." << endl;
    }
}
void createProductsFile(string inputFile) {
    vector<Field> fields = readFieldsFromCSV(inputFile, "length-file.txt");
    vector<string> allRecords;
    ifstream inFile(inputFile);
    ofstream outFile("products.dat", ios::binary | ios::out);

    if (inFile.is_open() && outFile.is_open()) {
        string line;
        getline(inFile, line);
        while (getline(inFile, line)) {
            vector<string> tokens = splitLine(line, ',');
            string record;
            for (size_t i = 0; i < fields.size(); ++i) {
                if (i == 13 || i == 14 || i == 15 || i == 16) {
                    string paddedField = padField(tokens[i], fields[i].length);
                    record += paddedField;
                }
            }
            allRecords.push_back(record);
            bool exists = false;
            int recordSize = allRecords.size() - 1;
            for (int i = 0; i < recordSize; i++) {
                if (allRecords[i] == record) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                Products product;
                for (size_t i = 0; i < fields.size(); ++i) {
                    string paddedField = padField(tokens[i], fields[i].length);
                    if (i == 13) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(product.productId, paddedField.c_str(), 15);
                        product.productId[15] = '\0';
                    }
                    else if (i == 14) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(product.category, paddedField.c_str(), 15);
                        product.category[15] = '\0';
                    }
                    else if (i == 15) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(product.subcategory, paddedField.c_str(), 11);
                        product.subcategory[11] = '\0';
                    }
                    else if (i == 16) {
                        string paddedField = padField(tokens[i], fields[i].length);
                        strncpy(product.productName, paddedField.c_str(), 120);
                        product.productName[120] = '\0';
                        outFile.write(reinterpret_cast<char*>(&product), sizeof(Products));
                    }
                }

            }
        }
        inFile.close();
        outFile.close();
    }
    else {
        cout << "Error opening files." << endl;
    }
}

void createCustomersIndex() {
    ifstream inFile("customers.dat", ios::binary);
    ofstream outFile("customers.idx", ios::binary);

    if (!inFile) {
        cerr << "Error al abrir el archivo de entrada " << endl;
        return;
    }

    if (!outFile) {
        cerr << "Error al abrir el archivo de salida" << endl;
        return;
    }

    Customers customer;
    customersIndexRecord indexRecord;
    vector<customersIndexRecord> indexRecords;

    while (inFile.read(reinterpret_cast<char*>(&customer), sizeof(Customers))) {
        strcpy(indexRecord.customerId, customer.customerId);
        indexRecord.offset = static_cast<int>(inFile.tellg()) - static_cast<int>(sizeof(Customers));  // Obtener el desplazamiento del registro actual
        indexRecords.push_back(indexRecord);
    }

    sort(indexRecords.begin(), indexRecords.end(), compareIndexRecords);

    for (const auto& record : indexRecords) {
        outFile.write(reinterpret_cast<const char*>(&record), sizeof(customersIndexRecord));
    }


    inFile.close();
    outFile.close();
}
void createOrdersIndex() {
    ifstream ordersFile("orders.dat", ios::binary);
    ofstream indexFile("orders.idx", ios::binary);

    if (!ordersFile) {
        cerr << "Error al abrir el archivo de orders" << endl;
        return;
    }

    if (!indexFile) {
        cerr << "Error al crear el archivo de índices" << endl;
        return;
    }

    vector<orderIndexEntry> indexEntries;

    Orders order;
    long offset = 0;
    while (ordersFile.read(reinterpret_cast<char*>(&order), sizeof(Orders))) {
        orderIndexEntry entry;
        strcpy(entry.orderId, order.orderId);
        entry.offset = offset;
        indexEntries.push_back(entry);
        offset += sizeof(Orders);
    }

    sort(indexEntries.begin(), indexEntries.end(), compareIndexEntries);

    for (const orderIndexEntry& entry : indexEntries) {
        indexFile.write(reinterpret_cast<const char*>(&entry), sizeof(orderIndexEntry));
    }

    ordersFile.close();
    indexFile.close();
}
void createProductsIndex() {
    ifstream productsFile("products.dat", ios::binary);
    ofstream indexFile("products.idx", ios::binary);

    if (!productsFile) {
        cerr << "Error: No se pudo abrir el archivo de productos." << endl;
        return;
    }

    if (!indexFile) {
        cerr << "Error: No se pudo crear el archivo de índices." << endl;
        return;
    }

    Products product;
    productsIndexEntry entry;
    vector<productsIndexEntry> indexEntries;

    while (productsFile.read(reinterpret_cast<char*>(&product), sizeof(Products))) {
        strcpy(entry.key, product.productId);
        entry.offset = static_cast<std::streamoff>(productsFile.tellg()) - static_cast<std::streamoff>(sizeof(Products));
        indexEntries.push_back(entry);
    }

    sort(indexEntries.begin(), indexEntries.end(), [](const productsIndexEntry& entry1, const productsIndexEntry& entry2) {
        return strcmp(entry1.key, entry2.key) < 0;
        });

    for (const productsIndexEntry& entry : indexEntries) {
        indexFile.write(reinterpret_cast<const char*>(&entry), sizeof(productsIndexEntry));
    }

    productsFile.close();
    indexFile.close();
}


void searchOrder(const string& orderId) {
    ifstream ordersIndexFile("orders.idx", ios::binary);
    ifstream ordersFile("orders.dat", ios::binary);
    ifstream customersIndexFile("customers.idx", ios::binary);
    ifstream customersFile("customers.dat", ios::binary);
    ifstream detailsFile("details.dat", ios::binary);
    ifstream productsIndexFile("products.idx", ios::binary);
    ifstream productsFile("products.dat", ios::binary);

    if (!ordersIndexFile || !ordersFile || !customersIndexFile || !customersFile || !detailsFile || !productsIndexFile || !productsFile) {
        cerr << "Error: No se pudieron abrir todos los archivos necesarios." << endl;
        return;
    }

    orderIndexEntry orderIndexEntry;
    bool orderFound = false;

    while (ordersIndexFile.read(reinterpret_cast<char*>(&orderIndexEntry), sizeof(orderIndexEntry))) {
        if (strcmp(orderIndexEntry.orderId, orderId.c_str()) == 0) {
            orderFound = true;
            break;
        }
    }

    if (!orderFound) {
        cout << "Orden no encontrada." << endl;
        return;
    }

    ordersFile.seekg(orderIndexEntry.offset);
    Orders order;
    ordersFile.read(reinterpret_cast<char*>(&order), sizeof(Orders));

    cout << "ORDER" << endl;
    cout << "    ID:          " << order.orderId << endl;
    cout << "    Date:        " << order.orderDate << endl;
    cout << "    Shipped:     " << order.shipDate << endl;
    cout << "    Ship Mode:   " << order.shipMode << endl;

    customersIndexRecord customerIndexRecord;
    customersIndexFile.seekg(0, ios::end);
    int indexFileSize = customersIndexFile.tellg();
    customersIndexFile.seekg(0, ios::beg);

    bool customerFound = false;

    while (customersIndexFile.tellg() < indexFileSize) {
        customersIndexFile.read(reinterpret_cast<char*>(&customerIndexRecord), sizeof(customerIndexRecord));

        if (strcmp(customerIndexRecord.customerId, order.customerId) == 0) {
            customerFound = true;
            break;
        }
    }

    if (!customerFound) {
        cout << "Cliente no encontrado." << endl;
        return;
    }

    customersFile.seekg(customerIndexRecord.offset);
    Customers customer;
    customersFile.read(reinterpret_cast<char*>(&customer), sizeof(Customers));

    cout << "CUSTOMER" << endl;
    cout << "    ID:          " << customer.customerId << endl;
    cout << "    Name:        " << customer.customerName << endl;
    cout << "    Segment:     " << customer.segment << endl;
    cout << "    Country:     " << customer.country << endl;
    cout << "    City:        " << customer.city << endl;
    cout << "    State:       " << customer.state << endl;
    cout << "    Postal Code: " << customer.postalCode << endl;
    cout << "    Region:      " << customer.region << endl;

    vector<Details> orderDetails;
    Details detail;

    while (detailsFile.read(reinterpret_cast<char*>(&detail), sizeof(Details))) {
        if (strcmp(detail.orderId, orderId.c_str()) == 0) {
            orderDetails.push_back(detail);
        }
    }

    cout << "ITEMS" << endl;
    int totalItems = 0;
    for (const Details& orderDetail : orderDetails) {
        productsIndexEntry productIndexEntry;
        productsIndexFile.seekg(0, ios::end);
        int productIndexFileSize = productsIndexFile.tellg();
        productsIndexFile.seekg(0, ios::beg);

        bool productFound = false;

        while (productsIndexFile.tellg() < productIndexFileSize) {
            productsIndexFile.read(reinterpret_cast<char*>(&productIndexEntry), sizeof(productIndexEntry));

            if (strcmp(productIndexEntry.key, orderDetail.productId) == 0) {
                productFound = true;
                break;
            }
        }

        if (!productFound) {
            cout << "Producto no encontrado para el detalle de orden: " << orderDetail.productId << endl;
            continue;
        }

        productsFile.seekg(productIndexEntry.offset);
        Products product;
        productsFile.read(reinterpret_cast<char*>(&product), sizeof(Products));

        cout << "    " << product.productId << " " << product.category << " " << product.subcategory << " " << product.productName << " " << orderDetail.sales << " " << orderDetail.quantity << " " << orderDetail.discount << " " << orderDetail.profit << endl;
        totalItems++;
    }


    double totalAmount = 0.0;

    for (const Details& orderDetail : orderDetails) {
        totalAmount += (orderDetail.sales * orderDetail.quantity);
    }

    cout << "ORDER SUMMARY" << endl;
    cout << "    Items: " << totalItems << endl;
    cout << "    Total: " << totalAmount << endl;

    ordersIndexFile.close();
    ordersFile.close();
    customersIndexFile.close();
    customersFile.close();
    detailsFile.close();
    productsIndexFile.close();
    productsFile.close();
}
void searchCustomer(const string& customerId) {
    ifstream customersIndexFile("customers.idx", ios::binary);
    ifstream customersFile("customers.dat", ios::binary);

    if (!customersIndexFile || !customersFile) {
        cerr << "Error: No se pudieron abrir todos los archivos necesarios." << endl;
        return;
    }

    customersIndexRecord customerIndexRecord;
    bool customerFound = false;

    while (customersIndexFile.read(reinterpret_cast<char*>(&customerIndexRecord), sizeof(customerIndexRecord))) {
        if (strcmp(customerIndexRecord.customerId, customerId.c_str()) == 0) {
            customerFound = true;
            break;
        }
    }

    if (!customerFound) {
        cout << "Cliente no encontrado." << endl;
        return;
    }

    customersFile.seekg(customerIndexRecord.offset);
    Customers customer;
    customersFile.read(reinterpret_cast<char*>(&customer), sizeof(Customers));

    cout << "CUSTOMER" << endl;
    cout << "    ID:          " << customer.customerId << endl;
    cout << "    Name:        " << customer.customerName << endl;
    cout << "    Segment:     " << customer.segment << endl;
    cout << "    Country:     " << customer.country << endl;
    cout << "    City:        " << customer.city << endl;
    cout << "    State:       " << customer.state << endl;
    cout << "    Postal Code: " << customer.postalCode << endl;
    cout << "    Region:      " << customer.region << endl;

    customersIndexFile.close();
    customersFile.close();
}
void searchProduct(const string& productId) {
    ifstream productsIndexFile("products.idx", ios::binary);
    ifstream productsFile("products.dat", ios::binary);

    if (!productsIndexFile || !productsFile) {
        cerr << "Error: No se pudieron abrir todos los archivos necesarios." << endl;
        return;
    }

    productsIndexEntry productIndexEntry;
    bool productFound = false;

    while (productsIndexFile.read(reinterpret_cast<char*>(&productIndexEntry), sizeof(productIndexEntry))) {
        if (strcmp(productIndexEntry.key, productId.c_str()) == 0) {
            productFound = true;
            break;
        }
    }

    if (!productFound) {
        cout << "Producto no encontrado." << endl;
        return;
    }

    productsFile.seekg(productIndexEntry.offset);
    Products product;
    productsFile.read(reinterpret_cast<char*>(&product), sizeof(Products));

    cout << "PRODUCT" << endl;
    cout << "    ID:          " << product.productId << endl;
    cout << "    Category:    " << product.category << endl;
    cout << "    Subcategory: " << product.subcategory << endl;
    cout << "    Name:        " << product.productName << endl;

    productsIndexFile.close();
    productsFile.close();
}

void splitData(const string& texto, string& antesDelIgual, string& despuesDelIgual) {
    size_t posicionIgual = texto.find("=");

    if (posicionIgual != std::string::npos) {
        antesDelIgual = texto.substr(0, posicionIgual);
        despuesDelIgual = texto.substr(posicionIgual + 1);
    }
    else {
        antesDelIgual = "";
        despuesDelIgual = "";
    }
}
int main(int argc, char** argv) {
    if (argc == 3 && string(argv[2]) == "-normalize") {
        createCustomersFile(string(argv[1]));
        createDetailsFile(string(argv[1]));
        createOrdersFile(string(argv[1]));
        createProductsFile(string(argv[1]));
    }
    else if (argc == 3 && string(argv[2]) == "-create-indices") {
        createCustomersIndex();
        createOrdersIndex();
        createProductsIndex();
    }
    else if (argc == 3 && string(argv[1]) == "-search") {
        string beforeEqual, afterEqual;
        splitData(argv[2], beforeEqual, afterEqual);
        if (beforeEqual == "order") {
            searchOrder(afterEqual);
        }
        else if (beforeEqual == "customer") {
            searchCustomer(afterEqual);
        }
        else if (beforeEqual == "product") {
            searchProduct(afterEqual);
        }
    }
    else {
        cerr << "Argumentos invalidos" << endl;
    }
}