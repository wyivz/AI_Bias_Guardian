#pragma once

#include <string>
#include <vector>

struct Bias {
  std::string span;
  std::string reason;
  std::string link;
};

struct AuditResult {
  bool bias_detected = false;
  std::vector<Bias> biases;
  std::string explanation;
};

class BiasAuditor {
 public:
  BiasAuditor();

  AuditResult audit(const std::string &ai_output);

 private:
  AuditResult parseResponse(const std::string &resp);
};
