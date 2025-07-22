#include "C:/projects/FinalProject/include/InvertedIndex.h"
#include <gtest/gtest.h>
#include <algorithm>

TEST(InvertedIndex, updateDocumentBase) {
    InvertedIndex index;

    EXPECT_NO_THROW(index.UpdateDocumentBase({}));

    std::vector<std::string> docs1 = { "hello world" };
    index.UpdateDocumentBase(docs1);

    EXPECT_FALSE(index.GetWordCount("hello").empty());
    EXPECT_FALSE(index.GetWordCount("world").empty());

    std::vector<std::string> docs2 = {
        "first document text",
        "second document text",
        "third document with more text"
    };
    index.UpdateDocumentBase(docs2);

    EXPECT_FALSE(index.GetWordCount("document").empty());
    EXPECT_EQ(index.GetWordCount("document").size(), 3);
}

TEST(InvertedIndex, getWordCount) {
    InvertedIndex index;
    std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell",
        "london bridge is falling down"
    };
    index.UpdateDocumentBase(docs);

    std::vector<Entry> entries = index.GetWordCount("london");
    ASSERT_EQ(entries.size(), 2);
    EXPECT_EQ(entries[0].doc_id, 0);
    EXPECT_EQ(entries[1].doc_id, 2);

    EXPECT_TRUE(index.GetWordCount("London").empty());

    EXPECT_TRUE(index.GetWordCount("nonexistent").empty());

    entries = index.GetWordCount("the");
    ASSERT_EQ(entries.size(), 2);

    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
        return a.doc_id < b.doc_id;
        });

    EXPECT_EQ(entries[0].doc_id, 0);
    EXPECT_EQ(entries[0].count, 1);
    EXPECT_EQ(entries[1].doc_id, 1);
    EXPECT_EQ(entries[1].count, 2);
}

TEST(InvertedIndex, threadSafety) {
    InvertedIndex index;
    std::vector<std::string> docs;

    for (int i = 0; i < 100; ++i) {
        docs.push_back("test thread safety word");
    }

    EXPECT_NO_THROW(index.UpdateDocumentBase(docs));

    auto entries = index.GetWordCount("word");
    EXPECT_EQ(entries.size(), 100);
    for (const auto& entry : entries) {
        EXPECT_EQ(entry.count, 1);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
