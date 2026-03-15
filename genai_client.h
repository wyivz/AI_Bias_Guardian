#pragma once

#include <string>

class GenAIClient {
public:
    GenAIClient();

    // Generate a response from the online GenAI API (e.g., OpenAI).
    // Requires OPENAI_API_KEY to be set in the environment.
    std::string generate(const std::string &prompt);

private:
    std::string callApi(const std::string &payload);
    std::string escapeJson(const std::string &s);
    std::string getApiKey();
};
