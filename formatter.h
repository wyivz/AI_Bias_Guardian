#pragma once

#include <string>
#include "auditor.h"

// Highlights bias spans in the given text. If the terminal supports ANSI, it will use red.
std::string highlight_bias(const std::string &text, const AuditResult &result);
