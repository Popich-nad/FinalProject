#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <vector>
#include <string>
#include "../include/InvertedIndex.h"

struct RelativeIndex {
    size_t doc_id;
    float rank;
};

class SearchEngine {
private:
    InvertedIndex _index;

public:
    SearchEngine(InvertedIndex& idx) : _index(idx) {}

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
};

#endif // SEARCHENGINE_H