#include "genai_client.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

static std::string readPipe(const std::string &cmd) {
  std::string result;
  FILE *pipe = POPEN(cmd.c_str(), "r");
  if (!pipe) return result;

  char buffer[256];
  while (fgets(buffer, sizeof(buffer), pipe)) {
    result += buffer;
  }

  PCLOSE(pipe);
  return result;
}

GenAIClient::GenAIClient() {}

std::string GenAIClient::getApiKey() {
  const char *key = std::getenv("OPENAI_API_KEY");
  return key ? std::string(key) : std::string();
}

std::string GenAIClient::escapeJson(const std::string &s) {
  std::string out;
  out.reserve(s.size());
  for (char c : s) {
    switch (c) {
      case '"':
        out += "\\\"";
        break;
      case '\\':
        out += "\\\\";
        break;
      case '\b':
        out += "\\b";
        break;
      case '\f':
        out += "\\f";
        break;
      case '\n':
        out += "\\n";
        break;
      case '\r':
        out += "\\r";
        break;
      case '\t':
        out += "\\t";
        break;
      default:
        if (static_cast<unsigned char>(c) < 0x20) {
          char buf[8];
          snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned char>(c));
          out += buf;
        } else {
          out += c;
        }
    }
  }
  return out;
}

std::string GenAIClient::callApi(const std::string &payload) {
  std::string key = getApiKey();
  if (key.empty()) {
    return "";
  }

  char tmpName[L_tmpnam] = {0};
  if (!std::tmpnam(tmpName)) {
    return "";
  }

  {
    std::ofstream out(tmpName, std::ios::binary);
    if (!out) {
      return "";
    }
    out << payload;
  }

  std::ostringstream cmd;
  cmd << "curl -s -X POST https://api.openai.com/v1/chat/completions"
      << " -H \"Authorization: Bearer " << key << "\""
      << " -H \"Content-Type: application/json\""
      << " -d @" << tmpName;

  std::string response = readPipe(cmd.str());

  std::remove(tmpName);
  return response;
}

std::string GenAIClient::generate(const std::string &prompt) {
  std::string key = getApiKey();
  if (key.empty()) {
    return "[ERROR] OPENAI_API_KEY is not set. Set it in the environment and "
           "restart.";
  }

  std::ostringstream json;
  json
      << "{\"model\":\"gpt-4o\",\"messages\":[{\"role\":\"system\",\"content\":"
         "\"You are a helpful assistant.\"},{\"role\":\"user\",\"content\":\"";
  json << escapeJson(prompt);
  json << "\"}],\"max_tokens\":800}";

  std::string response = callApi(json.str());
  if (response.empty()) {
    return "[ERROR] Failed to reach GenAI API (check network / API key).";
  }

  // Try to locate the completion text returned by OpenAI.
  // We look for "content" in the first message of the response.
  const std::string keyContent = "\"content\"";
  auto pos = response.find(keyContent);
  if (pos == std::string::npos) return response;

  // Find the next '"' after content and then the following ':'
  pos = response.find(':', pos);
  if (pos == std::string::npos) return response;

  // Find opening quote of the string.
  pos = response.find('"', pos);
  if (pos == std::string::npos) return response;
  pos += 1;

  std::string out;
  while (pos < response.size()) {
    char c = response[pos];
    if (c == '"') break;
    if (c == '\\' && pos + 1 < response.size()) {
      char next = response[pos + 1];
      if (next == 'n') {
        out += '\n';
        pos += 2;
        continue;
      } else if (next == 'r') {
        out += '\r';
        pos += 2;
        continue;
      } else if (next == 't') {
        out += '\t';
        pos += 2;
        continue;
      } else if (next == '"' || next == '\\' || next == '/') {
        out += next;
        pos += 2;
        continue;
      }
    }
    out += c;
    pos += 1;
  }

  return out;
}
