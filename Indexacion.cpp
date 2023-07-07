#include "Indexar.h"

Index::Index(int index, string id, int offset) {
    this->index = index;
    this->id = id;
    this->offset = offset;
}

void Index::setIndex(int index) {
    this->index = index;
}

int Index::getIndex() {
    return this->index;
}

void Index::setId(string id) {
    this->id = id;
}

string Index::getId() {
    return this->id;
}

void Index::serOffset(int offset) {
    this->offset = offset;
}

int Index::getOffset() {
    return this->offset;
}