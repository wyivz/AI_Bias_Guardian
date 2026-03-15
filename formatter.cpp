#include "formatter.h"

static std::string replaceAll(std::string s, const std::string &from, const std::string &to) {
    if (from.empty())
        return s;
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.length(), to);
        pos += to.length();
    }
    return s;
}

std::string highlight_bias(const std::string &text, const AuditResult &result) {
    std::string out = text;

    const std::string start = "\x1b[1;31m"; // bold red
    const std::string end = "\x1b[0m";

    for (const auto &b : result.biases) {
        if (b.span.empty())
            continue;
        out = replaceAll(out, b.span, start + b.span + end);
    }

    return out;
}
