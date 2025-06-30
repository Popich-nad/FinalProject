#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#include <vector>
#include <string>

class ConverterJSON {
private:
    std::string name;
    std::string version;
    int max_responses;
    std::vector<std::string> files;

public:
    ConverterJSON();
    std::vector<std::string> GetTextDocuments();
    int GetResponsesLimit() const;
    std::vector<std::string> GetRequests();
    void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);

    std::string GetName() const { return name; }
    std::string GetVersion() const { return version; }
};

#endif // CONVERTERJSON_H