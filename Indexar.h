#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED
#include <string>
using std::string;

class Index {
    private:
    int index;
    string id;
    int offset;
    public:
    Index(int, string, int);
    void setIndex(int index);
    int getIndex();
    void setId(string id);
    string getId();
    void serOffset(int offset);
    int getOffset();
};

#endif