#include "Indexacion.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::vector;
using std::to_string;
using std::setw;

string handleQuotes(stringstream& ss) {
    string field;
    bool inQuotes = false;
    string quotedField;
    while (getline(ss, field, ',')) {
        field = field.substr(field.find_first_not_of(' '));
        field = field.substr(0, field.find_last_not_of(' ') + 1);
        if (!inQuotes && !field.empty() && field.front() == '"') {
            inQuotes = true;
            if (field.length() > 1 && field[field.length() - 2] == '"') {
                quotedField = field.substr(1, field.length() - 3);
                inQuotes = false;
            } else {
                quotedField = field.substr(1);
            }
        } else if (inQuotes) {
            quotedField += "," + field;
            if (field.back() == '"' && !field.empty() && field[field.length() - 2] != '"') {
                inQuotes = false;
                return quotedField.substr(0, quotedField.length() - 1);
            }
        } else {
            return field;
        }
    }
    if (inQuotes) {
        string remainingData;
        stringstream ss2(quotedField);
        while (getline(ss2, remainingData, ',')) {
            // Hacer algo con cada elemento de remainingData
        }
        quotedField = quotedField + '/';
        return quotedField;
    }
    return field;
}

int getNumberOfRecords(string fileName) {
    int counter = 0;
    ifstream inputFile(fileName, std::ios::binary);
    string line;
    while (getline(inputFile, line, '\n')) {
        counter++;
    }
    inputFile.close();
    return counter;
}

int getRecordLength(string fileName) {
    int numberOfRecords = getNumberOfRecords(fileName);
    int recordLength = 0;
    ifstream inputFile(fileName, std::ios::binary);
    inputFile.seekg(0, std::ios::end);
    int fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);
    recordLength = fileSize / numberOfRecords;
    inputFile.close();
    return recordLength;
}

int getFieldSize(string lengthFileName, int position) {
    ifstream inputFile(lengthFileName, std::ios::binary);
    string line, field;
    string dataName, dataType, dataLength;
    //int dataLength;
    for (int i = 0; i < 21; i++) {
        getline(inputFile, line, '\n');
        stringstream lineSS(line);
        if (i == position) {
            string data1, data2, data3, data4;
            string result;
            lineSS >> data1 >> data2 >> data3;
            dataName = data1;
            dataLength = data2;
            dataType = data3;
            if (lineSS >> data4) {
                dataLength = data3;
            }
             else {
                dataLength = data2;
            }
            if (dataType.front() == '1') {
                data3 = dataType.substr(0, dataType.length() - 4);
                dataLength = data3;
            }
        }
    }
    return stoi(dataLength);
}

void writeNormalized(string field, int fieldLength, int section, bool writeLineBreak) {
    switch (section) {
        case 0: {
            ofstream fileOutput("orders.dat", std::ios::app | std::ios::out | std::ios::binary);
            fileOutput.write(field.c_str(), fieldLength);
            fileOutput.write(" ", 1);
            if (writeLineBreak == true) {
                fileOutput.write("\n", 1);
                fileOutput.close();
            }
        }
        break;
        case 1: {
            ofstream fileOutput("details.dat", std::ios::app | std::ios::out | std::ios::binary);
            fileOutput.write(field.c_str(), fieldLength);
            fileOutput.write(" ", 1);
            if (writeLineBreak == true) {
                fileOutput.write("\n", 1);
                fileOutput.close();
            }
        }
        break;
        case 2: {
            ofstream fileOutput("customers.dat", std::ios::app | std::ios::out | std::ios::binary);
            fileOutput.write(field.c_str(), fieldLength);
            fileOutput.write(" ", 1);
            if (writeLineBreak == true) {
                fileOutput.write("\n", 1);
                fileOutput.close();
            }
        }
        break;
        case 3: {
            ofstream fileOutput("products.dat", std::ios::app | std::ios::out | std::ios::binary);
            fileOutput.write(field.c_str(), fieldLength);
            fileOutput.write(" ", 1);
            if (writeLineBreak == true) {
                fileOutput.write("\n", 1);
                fileOutput.close();
            }
        }
        break;
    }
}

void formatField(std::string& field, int length) {
    field.resize(length, ' ');
}

void writeIndexFile (int recordLength, string fileName, int fileType, int numberOfRecords) {
    int offset = 0;
    ifstream fileInput(fileName, std::ios::binary);
    if (!fileInput) {
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return;
    }
    switch (fileType) {
        case 0: {
            ofstream fileOutput("customers.idx");
            string line, id;
            for (int i = 0; i < numberOfRecords; i++) {
                int offset = recordLength * i;
                string offsesStr = to_string(offset);
                getline(fileInput, line, '\n');
                id = line.substr(0, 8);
                cout << id << " " << offset << endl;
                fileOutput.write(id.c_str(), 8);
                fileOutput.write(" ",1);
                fileOutput.write(offsesStr.c_str(), offsesStr.length());
                fileOutput.write("\n",1);
            }
            break;
        }
        case 1: {
            ofstream fileOutput("orders.idx");
            string line, id;
            for (int i = 0; i < numberOfRecords; i++) {
                int offset = recordLength * i;
                string offsesStr = to_string(offset);
                getline(fileInput, line, '\n');
                id = line.substr(0, 14);
                cout << id << " " << offset << endl;
                fileOutput.write(id.c_str(), 14);
                fileOutput.write(" ",1);
                fileOutput.write(offsesStr.c_str(), offsesStr.length());
                fileOutput.write("\n",1);
            }
            break;
        }
        case 2: {
            ofstream fileOutput("products.idx");
            string line, id;
            for (int i = 0; i < numberOfRecords; i++) {
                int offset = recordLength * i;
                string offsesStr = to_string(offset);
                getline(fileInput, line, '\n');
                id = line.substr(0, 15);
                cout << id << " " << offset << endl;
                fileOutput.write(id.c_str(), 15);
                fileOutput.write(" ",1);
                fileOutput.write(offsesStr.c_str(), offsesStr.length());
                fileOutput.write("\n",1);
            }
            break;
        }
    }
}

double getData (int fileType, int offset, bool visited, int index) {
    double total;
    string line;
    switch(fileType) {
        case 0: {
            ifstream inputFile("orders.dat");
            inputFile.seekg(offset);
            string id(14, '\0');
            inputFile.read(&id[0], 14);
            offset = offset + 15;
            inputFile.seekg(offset);
            string date(10, '\0');
            inputFile.read(&date[0], 10);
            offset = offset + 11;
            inputFile.seekg(offset);
            string shipped(10, '\0');
            inputFile.read(&shipped[0], 10);
            offset = offset + 11;
            inputFile.seekg(offset);
            string shipMode(14, '\0');
            inputFile.read(&shipMode[0], 14);
            cout << "ORDER" << endl;
            cout << "    " << "ID:        " << id << endl;
            cout << "    " << "Date:      " << date << endl;
            cout << "    " << "Shipped:   " << shipped << endl;
            cout << "    " << "Ship Mode: " << shipMode << endl;
            cout << endl;
            break;
        }
        case 1: {
            ifstream inputFile("customers.dat");
            inputFile.seekg(offset);
            string id(8, '\0');
            inputFile.read(&id[0], 8);
            offset = offset + 9;

            inputFile.seekg(offset);
            string name(20, '\0');
            inputFile.read(&name[0], 20);
            offset = offset + 21;

            inputFile.seekg(offset);
            string segment(11, '\0');
            inputFile.read(&segment[0], 11);
            offset = offset + 12;

            inputFile.seekg(offset);
            string country(13, '\0');
            inputFile.read(&country[0], 13);
            offset = offset + 14;

            inputFile.seekg(offset);
            string city(20, '\0');
            inputFile.read(&city[0], 20);
            offset = offset + 21;

            inputFile.seekg(offset);
            string state(20, '\0');
            inputFile.read(&state[0], 20);
            offset = offset + 21;

            inputFile.seekg(offset);
            string postal(5, '\0');
            inputFile.read(&postal[0], 5);
            offset = offset + 6;

            inputFile.seekg(offset);
            string region(7, '\0');
            inputFile.read(&region[0], 7);
            cout << "CUSTOMER" << endl;
            cout << "    " << "ID:          " << id << endl;
            cout << "    " << "Name:        " << name << endl;
            cout << "    " << "Segment:     " << segment << endl;
            cout << "    " << "Country:     " << country << endl;
            cout << "    " << "City:        " << city << endl;
            cout << "    " << "State:       " << state << endl;
            cout << "    " << "Postal Code: " << postal << endl;
            cout << "    " << "Region:      " << region << endl;
            cout << endl;
            break;
        }
        case 2: {
            int offsetDetails = 63 * (index + 1) + 31;
            ifstream inputFile("products.dat");
            ifstream inputDetails("details.dat");
            inputFile.seekg(offset);
            string id(15, '\0');
            inputFile.read(&id[0], 15);
            offset = offset + 16;

            inputFile.seekg(offset);
            string category(15, '\0');
            inputFile.read(&category[0], 15);
            offset = offset + 16;

            inputFile.seekg(offset);
            string subCategory(11, '\0');
            inputFile.read(&subCategory[0], 11);
            offset = offset + 12;

            inputFile.seekg(offset);
            string productName(120, '\0');
            inputFile.read(&productName[0], 120);

            inputDetails.seekg(offsetDetails);
            string sales(10, '\0');
            inputDetails.read(&sales[0], 10);
            offsetDetails = offsetDetails + 11;

            inputDetails.seekg(offsetDetails);
            string quantity(3, '\0');
            inputDetails.read(&quantity[0], 3);
            offsetDetails = offsetDetails + 4;

            inputDetails.seekg(offsetDetails);
            string discount(4, '\0');
            inputDetails.read(&discount[0], 4);
            offsetDetails = offsetDetails + 5;

            inputDetails.seekg(offsetDetails);
            string profit(10, '\0');
            inputDetails.read(&profit[0], 10);

            formatField(id, 16);
            formatField(category, 16);
            formatField(subCategory, 12);
            formatField(productName, 121);
            formatField(sales, 10);
            formatField(quantity, 3);
            formatField(discount, 4);
            formatField(profit, 10);
            if (visited == false) {
                cout << "ITEMS" << endl;
            }
            cout << "    " << id << category << subCategory << productName << sales << quantity << discount << profit << endl;
            total = stoi(quantity) * stod(sales);
            break;
        }
        case 3: {
            ifstream inputFile("products.dat");
            inputFile.seekg(offset);
            string id(15, '\0');
            inputFile.read(&id[0], 15);
            offset = offset + 16;

            inputFile.seekg(offset);
            string category(15, '\0');
            inputFile.read(&category[0], 15);
            offset = offset + 16;

            inputFile.seekg(offset);
            string subCategory(11, '\0');
            inputFile.read(&subCategory[0], 11);
            offset = offset + 12;

            inputFile.seekg(offset);
            string productName(120, '\0');
            inputFile.read(&productName[0], 120);
            formatField(id, 16);
            formatField(category, 16);
            formatField(subCategory, 12);
            formatField(productName, 121);
            cout << "Product" << endl;
            cout << "    " << "ID:           " << id << endl;
            cout << "    " << "Category:     " << category << endl;
            cout << "    " << "Sub-category: " << subCategory << endl;
            cout << "    " << "Name:         " << productName << endl;
            cout << endl;
            break;
        }
    }
    return total;
}

vector<Index*> getIndexList(int archivo) {
    vector<Index*> indexList;
    int index = 1;
    bool isFirstLine = true;
    string line, id, offset;
    switch (archivo) {
        case 0: {
            ifstream fileInput("orders.idx");
            while (getline(fileInput, line, '\n')) {
                if (isFirstLine == true) {
                    isFirstLine =false;
                    continue;
                }
                stringstream ss(line);
                ss >> id >> offset;
                indexList.push_back(new Index(index, id, stoi(offset)));
            }
            break;
        }
        case 1: {
            ifstream fileInput("customers.idx");
            while (getline(fileInput, line, '\n')) {
                if (isFirstLine == true) {
                    isFirstLine =false;
                    continue;
                }
                stringstream ss(line);
                ss >> id >> offset;
                indexList.push_back(new Index(index, id, stoi(offset)));
            }
            break;
        }
        case 2: {
            ifstream fileInput("products.idx");
            while (getline(fileInput, line, '\n')) {
                if (isFirstLine == true) {
                    isFirstLine =false;
                    continue;
                }
                stringstream ss(line);
                ss >> id >> offset;
                indexList.push_back(new Index(index, id, stoi(offset)));
            }
            break;
        }
    }
    return indexList;  
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Error: Debe ingresar el nombre del archivo y la bandera de la accion que desea realizar." << endl;
    }
    string fileName = argv[1];
    string flag = argv[2];
    if (flag == "-normalize") {
        ifstream fileInput(fileName, std::ios::in | std::ios::binary);
        if (fileInput.fail()) {
            cerr << "Error: No se pudo abrir el archivo." << endl;
            return 0;
        }
        //-----------------------------------
        int numberOfRecords = getNumberOfRecords(fileName);
        //Normalize the file
        for (int i = 0; i < numberOfRecords; i++) {
            bool isInput = false;
            string line;
            getline(fileInput, line, '\n');
            stringstream ss(line);
            for (int i = 0; i < 21; i++) {
                string field = handleQuotes(ss);
                int fieldLength = getFieldSize("length-file.txt", i);
                if (field.back() == '/') {
                    isInput = true;
                    int isLast = 0;
                    vector<string> quoteData;
                    string remainingData = field;
                    remainingData = remainingData.substr(0, remainingData.length() - 1);
                    string nextData;
                    stringstream ssQuotedField(remainingData);
                    while(getline(ssQuotedField, field, ',')) {
                        if (field.back() != '"') {
                            quoteData.push_back(field);
                            field = "";
                        } else if (field.back() == '"') {
                            quoteData.push_back(field);
                            field = "";
                        } else if (field.back() == ' '){
                            field = field;
                            i++;
                        }
                    }
                    for (size_t i = 0; i < quoteData.size() - 4; i++) {
                        field = field + quoteData[i] + ",";
                    }
                    field = field.substr(0, field.length() - 2);
                    formatField(field, fieldLength);
                    writeNormalized(field, fieldLength, 3, true);
                    //Remaining data
                    field = "";
                    for (size_t i = quoteData.size() - 4; i < quoteData.size(); i++) {
                        field = field + quoteData[i] + ",";
                    }
                    field = field.substr(0, field.length() - 1);
                    stringstream remainingSS(field);
                    while(getline(remainingSS, nextData, ',')) {
                        fieldLength = getFieldSize("length-file.txt", i + 1);
                        formatField(nextData, fieldLength);
                        if (isLast == 3) {
                            writeNormalized(nextData, fieldLength, 1, true);
                        } else {
                            writeNormalized(nextData, fieldLength, 1, false);
                        }
                        i++;
                        isLast++;
                    }
                } else {
                    if (isInput == false) {
                        formatField(field, fieldLength);
                        switch (i) {
                            case 1: 
                            writeNormalized(field, fieldLength, 0, false);
                            writeNormalized(field, fieldLength, 1, false);
                            break;
                            case 2:
                            writeNormalized(field, fieldLength, 0, false);
                            break;
                            case 3:
                            writeNormalized(field, fieldLength, 0, false);
                            break;
                            case 4:
                            writeNormalized(field, fieldLength, 0, false);
                            break;
                            case 5:
                            writeNormalized(field, fieldLength, 0, true);
                            writeNormalized(field, fieldLength, 2, false);
                            break;
                            case 6:
                            writeNormalized(field, fieldLength, 2, false);
                            break;
                            case 7:
                            writeNormalized(field, fieldLength, 2, false);
                            break;
                            case 8:
                            writeNormalized(field, fieldLength, 2, false);
                            break;
                            case 9:
                            writeNormalized(field, fieldLength, 2, false);
                            break;
                            case 10:
                            writeNormalized(field, fieldLength, 2, false);
                            break;
                            case 11:
                            writeNormalized(field, fieldLength, 2, false);
                            break;
                            case 12:
                            writeNormalized(field, fieldLength, 2, true);
                            break;
                            case 13:
                            writeNormalized(field, fieldLength, 1, false);
                            writeNormalized(field, fieldLength, 3, false);
                            break;
                            case 14:
                            writeNormalized(field, fieldLength, 3, false);
                            break;
                            case 15:
                            writeNormalized(field, fieldLength, 3, false);
                            break;
                            case 16:
                            writeNormalized(field, fieldLength, 3, true);
                            break;
                            case 17:
                            writeNormalized(field, fieldLength, 1, false);
                            break;
                            case 18:
                            writeNormalized(field, fieldLength, 1, false);
                            break;
                            case 19:
                            writeNormalized(field, fieldLength, 1, false);
                            break;
                            case 20:
                            writeNormalized(field, fieldLength, 1, true);
                            break;
                        }
                    }
                }
            }
        }

        //Create the index files with its byte offset
        int ordersLength, productsLength, costumersLength;
        costumersLength = getRecordLength("customers.dat");
        ordersLength = getRecordLength("orders.dat");
        productsLength = getRecordLength("products.dat");
        writeIndexFile(costumersLength, "customers.dat", 0, numberOfRecords);
        writeIndexFile(ordersLength, "orders.dat", 1, numberOfRecords);
        writeIndexFile(productsLength, "products.dat", 2, numberOfRecords);
        
    } else if (fileName == "-search") {
        string orderId;
        bool visited = false, found = false, itemVisited = false;
        int index = 1, quantity = 1;
        double total = 0;
        string line, id;
        vector<Index*> indexOrders = getIndexList(0);
        vector<Index*> indexCostumers = getIndexList(1);
        vector<Index*> indexProducts = getIndexList(2);
        if (flag.substr(0, 6) == "order=") {
            orderId = flag.substr(6, flag.length());
            for (int i = 0; i < indexOrders.size(); i++) {
                if (indexOrders[i]->getId() == orderId) {
                    index = i;
                    int offsetOrders = indexOrders[i]->getOffset();
                    int offsetCustomers = indexCostumers[i]->getOffset();
                    int offsetProducts = indexProducts[i]->getOffset();
                    if (visited == false) {
                        visited = true;
                        getData(0, offsetOrders, true, i);
                        getData(1, offsetCustomers, true, i);
                        
                    }
                    if (itemVisited == true) {
                        quantity++;
                    }
                    if (itemVisited == false) {
                        itemVisited = true;
                        total = total + getData(2, offsetProducts, false, i);
                    } else {
                        total = total + getData(2, offsetProducts, true, i);
                    }
                    found = true;
                }
            }
            cout << "\nORDER SUMMARY" << endl;
            cout << "    Items: " << quantity << endl;
            cout << "    Total: " << total << endl;
            if (found == false) {
                cout << "No se encontro la orden." << endl;
            }
        } else if (flag.substr(0, 9) == "customer=") {
            orderId = flag.substr(9, flag.length());
            for (int i = 0; i < indexOrders.size(); i++) {
                if (indexCostumers[i]->getId() == orderId) {
                    index = i;
                    int offsetCustomers = indexCostumers[i]->getOffset();
                    if (visited == false) {
                        visited = true;
                        getData(1, offsetCustomers, true, i);
                    }
                    found = true;
                }
            }
            if (found == false) {
                cout << "No se encontro el cliente." << endl;
            }
        } else if (flag.substr(0, 8) == "product=") {
            orderId = flag.substr(8, flag.length());
            for (int i = 0; i < indexOrders.size(); i++) {
                if (indexProducts[i]->getId() == orderId) {
                    index = i;
                    int offsetProducts = indexProducts[i]->getOffset();
                    getData(3, offsetProducts, true, i);
                    found = true;
                }
            }
            if (found == false) {
                cout << "No se pudo encontrar el producto." << endl;
            }
        } else {
            cerr << "Error! Accion erronea" << endl;
        }
        
    } else {
        cerr << "Error: Bandera incorrecta.\nDebe ser -normalize o -" << endl;
    }
    return 0;
}
//g++
//./a.out ./data/superstore.csv -normalize
//./a.out -search order=CA-2015-149587
