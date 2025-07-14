#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "ConverterJSON.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

ConverterJSON::ConverterJSON() {
    try {
        std::ifstream config_file("config.json");
        if (!config_file.is_open()) {
            throw std::runtime_error("config file is missing");
        }

        json config;
        config_file >> config;
        config_file.close();

        if (config.empty() || !config.contains("config")) {
            throw std::runtime_error("config file is empty");
        }

        auto config_data = config["config"];
        name = config_data["name"].get<std::string>();
        version = config_data["version"].get<std::string>();
        max_responses = config_data.value("max_responses", 5);

        if (version != "0.1") {
            throw std::runtime_error("config.json has incorrect file version");
        }

        files = config["files"].get<std::vector<std::string>>();
    }
    catch (const std::exception& e) {
        throw;
    }
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> documents;
    for (const auto& file_path : files) {
        try {
            std::ifstream file(file_path);
            if (!file.is_open()) {
                std::cerr << "File not found: " << file_path << std::endl;
                continue;
            }

            std::string content((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
            documents.push_back(content);
            file.close();
        }
        catch (...) {
            std::cerr << "Error reading file: " << file_path << std::endl;
        }
    }
    return documents;
}

int ConverterJSON::GetResponsesLimit() const {
    return max_responses;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests;
    try {
        std::ifstream requests_file("requests.json");
        if (!requests_file.is_open()) {
            throw std::runtime_error("requests.json file is missing");
        }

        json requests_data;
        requests_file >> requests_data;
        requests_file.close();

        requests = requests_data["requests"].get<std::vector<std::string>>();
    }
    catch (...) {
        throw;
    }
    return requests;
}

void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    json result;
    json answers_json;

    for (size_t i = 0; i < answers.size(); ++i) {
        json answer;
        std::string request_id = "request" + std::to_string(i + 1);

        if (answers[i].empty()) {
            answer["result"] = false;
        }
        else {
            answer["result"] = true;

            if (answers[i].size() > 1) {
                json relevance;
                for (const auto& doc_pair : answers[i]) {
                    relevance.push_back({
                        {"docid", doc_pair.first},
                        {"rank", doc_pair.second}
                        });
                }
                answer["relevance"] = relevance;
            }
            else {
                answer["docid"] = answers[i][0].first;
                answer["rank"] = answers[i][0].second;
            }
        }

        answers_json[request_id] = answer;
    }

    result["answers"] = answers_json;

    std::ofstream answers_file("answers.json");
    answers_file << result.dump(4);
    answers_file.close();
}
