#include <iostream>
#include <limits>
#include <string>
#include <vector>

// 如果是 Windows 系统，需要包含此头文件来开启 ANSI 颜色支持
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
// --- Windows 兼容性处理 ---
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  GetConsoleMode(hOut, &dwMode);
  SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

  GenAIClient genai;
  BiasAuditor auditor;

  cout << "\033[1;36mGenAI Bias Auditor CLI\033[0m\n";  // 青色标题
  cout << "Set OPENAI_API_KEY in your environment before running.\n";

  while (true) {
    cout << "\n\033[1;32mUser > \033[0m";  // 绿色提示符
    string user_input;
    if (!std::getline(cin, user_input)) break;

    if (user_input == "exit") break;

    // 1. 检测用户输入的 bias
    AuditResult audit_result = auditor.audit(user_input);
    float bias_score = audit_result.biases.size() * 20.0f;

    // 格式化输出分数
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

    // 2. 生成 AI 回复
    string ai_output = genai.generate(user_input);

    // 3. 检测 AI 输出的 bias
    AuditResult result = auditor.audit(ai_output);

    if (result.bias_detected) {
      cout << "\n\033[1;31m[!] Bias detected in AI response:\033[0m\n";

      // 调用高亮函数
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