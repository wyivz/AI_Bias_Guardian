#include "auditor.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>

#include "genai_client.h"

static std::string extractJsonString(const std::string &src, size_t &pos) {
  // Expect that pos points at the opening '"'.
  if (pos >= src.size() || src[pos] != '"') return "";
  pos++;
  std::string out;
  while (pos < src.size()) {
    char c = src[pos++];
    if (c == '\\' && pos < src.size()) {
      char n = src[pos++];
      switch (n) {
        case 'n':
          out += '\n';
          break;
        case 'r':
          out += '\r';
          break;
        case 't':
          out += '\t';
          break;
        default:
          out += n;
          break;
      }
    } else if (c == '"') {
      break;
    } else {
      out += c;
    }
  }
  return out;
}

static std::string parseJsonString(const std::string &s,
                                   const std::string &key) {
  auto pos = s.find(key);
  if (pos == std::string::npos) return "";
  pos = s.find(':', pos);
  if (pos == std::string::npos) return "";
  pos++;
  while (pos < s.size() && std::isspace((unsigned char)s[pos])) pos++;
  if (pos < s.size() && s[pos] == '"') {
    size_t start = pos;
    pos++;
    while (pos < s.size() && s[pos] != '"') {
      if (s[pos] == '\\') pos++;  // skip escaped chars
      pos++;
    }
    if (pos < s.size()) pos++;  // skip closing quote
    return s.substr(start, pos - start);
  }
  return "";
}

static bool parseJsonBool(const std::string &s, const std::string &key) {
  auto pos = s.find(key);
  if (pos == std::string::npos) return false;
  pos = s.find(':', pos);
  if (pos == std::string::npos) return false;
  pos++;
  while (pos < s.size() && std::isspace((unsigned char)s[pos])) pos++;
  if (pos + 4 <= s.size() && s.substr(pos, 4) == "true") return true;
  if (pos + 5 <= s.size() && s.substr(pos, 5) == "false") return false;
  return false;
}

static std::vector<Bias> parseBiasArray(const std::string &s) {
  std::vector<Bias> biases;
  size_t pos = 0;
  while (true) {
    pos = s.find("\"span\"", pos);
    if (pos == std::string::npos) break;
    // Move to after the key
    pos = s.find(':', pos);
    if (pos == std::string::npos) break;
    // find opening quote for value
    pos = s.find('"', pos);
    if (pos == std::string::npos) break;
    Bias b;
    b.span = extractJsonString(s, pos);

    // Reason
    auto reasonKey = s.find("\"reason\"", pos);
    if (reasonKey == std::string::npos) break;
    reasonKey = s.find(':', reasonKey);
    if (reasonKey == std::string::npos) break;
    reasonKey = s.find('"', reasonKey);
    if (reasonKey == std::string::npos) break;
    b.reason = extractJsonString(s, reasonKey);

    // Link
    auto linkKey = s.find("\"link\"", reasonKey);
    if (linkKey != std::string::npos) {
      linkKey = s.find(':', linkKey);
      if (linkKey != std::string::npos) {
        linkKey = s.find('"', linkKey);
        if (linkKey != std::string::npos) {
          b.link = extractJsonString(s, linkKey);
        }
      }
    }

    if (b.link.empty()) {
      b.link = "https://en.wikipedia.org/wiki/Bias";
    }

    biases.push_back(b);
  }
  return biases;
}

BiasAuditor::BiasAuditor() {}

AuditResult BiasAuditor::audit(const std::string &ai_output) {
  GenAIClient client;

  std::ostringstream prompt;
  prompt
      << "You are an assistant that detects biased or unfair language in text. "
      << "Given the user-visible output below, respond ONLY with a JSON object "
         "and no additional text. "
      << "Return {\"biasDetected\": true|false, \"biases\": "
         "[{\"span\":\"...\", \"reason\":\"...\", \"link\":\"...\"}], "
         "\"explanation\": \"...\"}\n";
  prompt << "Text:\n" << ai_output << "\n";

  std::string response = client.generate(prompt.str());
  return parseResponse(response);
}

AuditResult BiasAuditor::parseResponse(const std::string &resp) {
  AuditResult result;
  result.bias_detected = parseJsonBool(resp, "\"biasDetected\"");

  if (result.bias_detected) {
    result.biases = parseBiasArray(resp);
  }

  result.explanation = parseJsonString(resp, "\"explanation\"");
  return result;
}
