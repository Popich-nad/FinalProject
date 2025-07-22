#include "C:/projects/FinalProject/include/ConverterJSON.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#if defined(_MSC_VER) && _MSC_VER < 1920
namespace fs = std::experimental::filesystem;
#else
namespace fs = std::filesystem;
#endif

void CreateTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

class ConverterJSONTest : public ::testing::Test {
protected:
    void SetUp() override {
        CreateTestFile("config.json", R"({
            "config": {
                "name": "TestSearchEngine",
                "version": "0.1",
                "max_responses": 3
            },
            "files": [
                "file1.txt",
                "file2.txt"
            ]
        })");

        CreateTestFile("file1.txt", "Test content of file 1");
        CreateTestFile("file2.txt", "Test content of file 2");
        CreateTestFile("requests.json", R"({
            "requests": [
                "first request",
                "second request"
            ]
        })");
    }

    void TearDown() override {
        remove("config.json");
        remove("file1.txt");
        remove("file2.txt");
        remove("requests.json");
        remove("answers.json");
    }
};

TEST_F(ConverterJSONTest, readConfig) {
    EXPECT_NO_THROW({
        ConverterJSON converter;
        EXPECT_EQ(converter.GetResponsesLimit(), 3);
        });

    remove("config.json");
    EXPECT_THROW(ConverterJSON converter, std::runtime_error);
}

TEST_F(ConverterJSONTest, readRequests) {
    ConverterJSON converter;
    auto requests = converter.GetRequests();

    ASSERT_EQ(requests.size(), 2);
    EXPECT_EQ(requests[0], "first request");
    EXPECT_EQ(requests[1], "second request");

    remove("requests.json");
    EXPECT_THROW(converter.GetRequests(), std::runtime_error);
}

TEST_F(ConverterJSONTest, getResponsesLimit) {
    ConverterJSON converter;
    EXPECT_EQ(converter.GetResponsesLimit(), 3);
}

TEST_F(ConverterJSONTest, getFiles) {
    ConverterJSON converter;
    auto documents = converter.GetTextDocuments();

    ASSERT_EQ(documents.size(), 2);
    EXPECT_EQ(documents[0], "Test content of file 1");
    EXPECT_EQ(documents[1], "Test content of file 2");

    remove("file1.txt");
    documents = converter.GetTextDocuments();
    ASSERT_EQ(documents.size(), 1);
    EXPECT_EQ(documents[0], "Test content of file 2");
}

TEST_F(ConverterJSONTest, putAnswers) {
    ConverterJSON converter;

    std::vector<std::vector<std::pair<int, float>>> answers = {
        {{1, 0.9f}, {2, 0.8f}},
        {{3, 0.7f}}
    };

    EXPECT_NO_THROW(converter.putAnswers(answers));

    std::ifstream test_file("answers.json");
    EXPECT_TRUE(test_file.good());
    test_file.close();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
