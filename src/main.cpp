#include <iostream>
#include <vector>
#include <utility>
#include <exception>
#include "../include/ConverterJSON.h"
#include "../include/SearchEngine.h"
#include "../include/InvertedIndex.h"

int main()
{
    try {
        ConverterJSON converter;
        std::cout << "Starting search engine" << std::endl;

        InvertedIndex index;
        index.UpdateDocumentBase(converter.GetTextDocuments());

        SearchEngine server(index);

        auto requests = converter.GetRequests();
        auto results = server.search(requests);

        std::vector<std::vector<std::pair<int, float>>> answers;
        int max_responses = converter.GetResponsesLimit();

        for (const auto& result : results) {
            std::vector<std::pair<int, float>> answer;
            for (size_t i = 0; i < result.size() && i < max_responses; ++i) {
                answer.emplace_back(result[i].doc_id, result[i].rank);
            }
            answers.push_back(answer);
        }

        converter.putAnswers(answers);
        std::cout << "Search completed. Results saved to answers.json" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
