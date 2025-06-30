#include "../include/SearchEngine.h"
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex& other) const {
        return doc_id == other.doc_id && rank == other.rank;
    }
};

class MockInvertedIndex : public InvertedIndex {
public:
    std::map<std::string, std::vector<Entry>> freq_dictionary;

    void AddDocument(size_t doc_id, std::istream& content) override {
        std::string doc((std::istreambuf_iterator<char>(content)),
            std::istreambuf_iterator<char>());

        std::map<std::string, size_t> word_counts;
        size_t start = 0;
        size_t end = doc.find(' ');

        while (end != std::string::npos) {
            std::string word = doc.substr(start, end - start);
            if (!word.empty()) word_counts[word]++;
            start = end + 1;
            end = doc.find(' ', start);
        }

        std::string last_word = doc.substr(start);
        if (!last_word.empty()) word_counts[last_word]++;

        for (const auto& [word, count] : word_counts) {
            freq_dictionary[word].push_back({ doc_id, count });
        }
    }

    std::vector<Entry> GetWordCount(const std::string& word) override {
        if (freq_dictionary.count(word)) {
            return freq_dictionary.at(word);
        }
        return {};
    }
};

class SearchEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        MockInvertedIndex idx;
        std::vector<std::string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock",
            "london bridge is falling down"
        };

        for (size_t i = 0; i < docs.size(); ++i) {
            std::istringstream iss(docs[i]);
            idx.AddDocument(i, iss);
        }

        engine = std::make_unique<SearchEngine>(idx);
    }

    std::unique_ptr<SearchEngine> engine;
};

TEST_F(SearchEngineTest, EmptyQueryReturnsEmptyResult) {
    std::vector<std::string> queries = { "" };
    auto results = engine->search(queries);
    EXPECT_TRUE(results.empty());
}

TEST_F(SearchEngineTest, SingleWordQueryReturnsCorrectResults) {
    std::vector<std::string> queries = { "london" };
    auto results = engine->search(queries);

    ASSERT_EQ(results.size(), 1);
    ASSERT_EQ(results[0].size(), 2);

    EXPECT_EQ(results[0][0].doc_id, 0);
    EXPECT_EQ(results[0][1].doc_id, 2);
}

TEST_F(SearchEngineTest, MultiWordQueryReturnsCorrectResults) {
    std::vector<std::string> queries = { "great london" };
    auto results = engine->search(queries);

    ASSERT_EQ(results.size(), 1);
    ASSERT_EQ(results[0].size(), 1);
    EXPECT_EQ(results[0][0].doc_id, 0);
}

TEST_F(SearchEngineTest, NonExistentWordReturnsEmptyResult) {
    std::vector<std::string> queries = { "nonexistentword" };
    auto results = engine->search(queries);

    ASSERT_EQ(results.size(), 1);
    EXPECT_TRUE(results[0].empty());
}

TEST_F(SearchEngineTest, MultipleQueriesProcessedCorrectly) {
    std::vector<std::string> queries = { "london", "the", "nonexistent" };
    auto results = engine->search(queries);

    ASSERT_EQ(results.size(), 3);
    EXPECT_EQ(results[0].size(), 2);
    EXPECT_EQ(results[1].size(), 2);
    EXPECT_TRUE(results[2].empty());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}