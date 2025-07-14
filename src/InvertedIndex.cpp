#include <vector>
#include <string>
#include <map>
#include <mutex>
#include <thread>
#include <algorithm>
#include "../include/InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    freq_dictionary.clear();

    std::vector<std::thread> threads;
    std::mutex dict_mutex;

    for (size_t doc_id = 0; doc_id < docs.size(); ++doc_id) {
        threads.emplace_back([this, doc_id, &dict_mutex]() {
            std::string doc = docs[doc_id];
            std::map<std::string, size_t> word_counts;

            size_t start = 0;
            size_t end = doc.find(' ');
            while (end != std::string::npos) {
                std::string word = doc.substr(start, end - start);
                if (!word.empty()) {
                    word_counts[word]++;
                }
                start = end + 1;
                end = doc.find(' ', start);
            }
            std::string last_word = doc.substr(start);
            if (!last_word.empty()) {
                word_counts[last_word]++;
            }

            std::lock_guard<std::mutex> lock(dict_mutex);
            for (const auto& word_count : word_counts) {
                freq_dictionary[word_count.first].push_back({ doc_id, word_count.second });
            }
            });
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    }
    return {};
}
