#include <iostream>
#include <limits>
#include <string>
#include <vector>

//Windows console color support
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

#include "auditor.h"
#include "formatter.h"
#include "genai_client.h"

using namespace std;

int main() {
// Enable ANSI escape codes on Windows for colored output
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  GetConsoleMode(hOut, &dwMode);
  SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

  GenAIClient genai;
  BiasAuditor auditor;

  cout << "\033[1;36mGenAI Bias Auditor CLI\033[0m\n";

  while (true) {
    cout << "\n\033[1;32mUser > \033[0m";
    string user_input;
    if (!std::getline(cin, user_input)) break;

    if (user_input == "exit") break;

    //Detect bias in user input
    AuditResult audit_result = auditor.audit(user_input);
    float bias_score = audit_result.biases.size() * 20.0f;

    //Initialize formatter with bias score
    Formatter formatter(bias_score);
    cout << "Bias Rate Score: " << (bias_score > 100 ? 100 : bias_score) << "%"
         << endl;

    if (audit_result.bias_detected) {
      cout << "\033[1;33mWarning: Bias detected in user input:\033[0m\n";
      string highlighted_input = highlight_bias(user_input, audit_result);
      cout << "Highlighted: " << highlighted_input << endl;
      for (auto &b : audit_result.biases) {
        cout << " \033[31m*\033[0m " << b.reason << "\n";
      }
    }

    //Generate AI response
    string ai_output = genai.generate(user_input);

    //Detect bias in AI response
    AuditResult result = auditor.audit(ai_output);

    if (result.bias_detected) {
      cout << "\n\033[1;31m[!] Bias detected in AI response:\033[0m\n";

      // Highlight the biased spans in the AI response
      string highlighted = highlight_bias(ai_output, result);
      cout << "Response: " << highlighted << endl;

      cout << "\n\033[1mExplanation:\033[0m\n";
      cout << result.explanation << "\n";
      for (auto &b : result.biases) {
        cout << " - " << b.reason << "\n";
        if (!b.link.empty())
          cout << "   Resource: \033[4;34m" << b.link << "\033[0m\n";
      }

      cout << "\nOptions: [1] Accept | [2] Regenerate\nSelect: ";

      int choice;
      if (!(cin >> choice)) break;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      if (choice == 2) {
        cout << "\n\033[33mRegenerating clean response...\033[0m\n";
        string regen_prompt =
            "Regenerate the response without any bias.\nQuestion: " +
            user_input + "\nPrevious answer: " + ai_output;
        ai_output = genai.generate(regen_prompt);
        cout << "\nNew Response:\n" << ai_output << endl;
      } else {
        cout << "\nAccepted Response:\n" << ai_output << endl;
      }
    } else {
      cout << "\033[1;34mAssistant:\033[0m " << ai_output << endl;
    }
  }

  return 0;
}