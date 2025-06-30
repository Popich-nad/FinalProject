#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>
#include "../include/SearchEngine.h"

std::vector<std::vector<RelativeIndex>> SearchEngine::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> results;

    for (const auto& query : queries_input) {
        std::vector<std::string> words;
        size_t start = 0;
        size_t end = query.find(' ');

        while (end != std::string::npos) {
            std::string word = query.substr(start, end - start);
            if (!word.empty() &&
                std::find(words.begin(), words.end(), word) == words.end()) {
                words.push_back(word);
            }
            start = end + 1;
            end = query.find(' ', start);
        }

        std::string last_word = query.substr(start);
        if (!last_word.empty() &&
            std::find(words.begin(), words.end(), last_word) == words.end()) {
            words.push_back(last_word);
        }

        if (words.empty()) {
            results.emplace_back();
            continue;
        }

        std::sort(words.begin(), words.end(), [this](const std::string& a, const std::string& b) {
            size_t count_a = 0, count_b = 0;
            auto entries_a = this->_index.GetWordCount(a);
            auto entries_b = this->_index.GetWordCount(b);

            for (const auto& entry : entries_a) count_a += entry.count;
            for (const auto& entry : entries_b) count_b += entry.count;

            return count_a < count_b;
            });

        std::vector<size_t> matching_docs;
        for (size_t i = 0; i < words.size(); ++i) {
            auto entries = this->_index.GetWordCount(words[i]);
            std::vector<size_t> doc_ids;
            for (const auto& entry : entries) {
                doc_ids.push_back(entry.doc_id);
            }

            if (i == 0) {
                matching_docs = doc_ids;
            }
            else {
                std::vector<size_t> intersection;
                std::sort(matching_docs.begin(), matching_docs.end());
                std::sort(doc_ids.begin(), doc_ids.end());
                std::set_intersection(
                    matching_docs.begin(), matching_docs.end(),
                    doc_ids.begin(), doc_ids.end(),
                    std::back_inserter(intersection));
                matching_docs = intersection;
            }

            if (matching_docs.empty()) break;
        }

        if (matching_docs.empty()) {
            results.emplace_back();
            continue;
        }

        std::map<size_t, float> absolute_relevance;
        float max_relevance = 0.0f;

        for (const auto& word : words) {
            auto entries = this->_index.GetWordCount(word);
            for (const auto& entry : entries) {
                if (std::find(matching_docs.begin(), matching_docs.end(), entry.doc_id) != matching_docs.end()) {
                    absolute_relevance[entry.doc_id] += static_cast<float>(entry.count);
                    if (absolute_relevance[entry.doc_id] > max_relevance) {
                        max_relevance = absolute_relevance[entry.doc_id];
                    }
                }
            }
        }

        std::vector<RelativeIndex> relevance;
        for (const auto& pair : absolute_relevance) {
            if (max_relevance > 0) {
                relevance.push_back({ pair.first, pair.second / max_relevance });
            }
            else {
                relevance.push_back({ pair.first, 0.0f });
            }
        }

        std::sort(relevance.begin(), relevance.end(),
            [](const RelativeIndex& a, const RelativeIndex& b) {
                return a.rank > b.rank;
            });

        results.push_back(relevance);
    }

    return results;
}