# AI Bias Auditor CLI

A command-line interface (CLI) tool that audits text for bias using generative AI. It detects biased language in user inputs and AI-generated responses, highlights problematic content, and provides options for regeneration to ensure fair and unbiased communication.

## Features

- **Pre-Response Bias Detection**: Scans user input for potential bias before generating an AI response, displaying a bias score and warnings.
- **Post-Response Auditing**: Analyzes AI-generated responses for bias, highlighting biased spans in red (using ANSI colors).
- **Interactive Options**: If bias is detected in the AI response, users can choose to accept it or regenerate a bias-free version.
- **Detailed Feedback**: Provides explanations, reasons for bias detection, and online resource links for each identified bias.
- **Cross-Platform**: Supports Windows (with ANSI color compatibility) and other platforms.
- **OpenAI Integration**: Uses the OpenAI API (e.g., GPT-4o) for both bias detection and response generation.

## Prerequisites

- **C++ Compiler**: GCC (g++) or compatible compiler with C++11 support.
- **Curl**: For making HTTP requests to the OpenAI API.
- **OpenAI API Key**: Required for accessing the OpenAI API. Sign up at [OpenAI](https://platform.openai.com/) if you don't have one.
- **Environment**: Windows, macOS, or Linux with a terminal that supports ANSI escape codes for color highlighting.

## Installation

1. **Clone or Download the Repository**:
   ```bash
   git clone <repository-url>
   cd ai-bias-auditor
   ```

2. **Build the Project**:
   - On Windows (using MinGW or similar):
     ```bash
     g++ -c auditor.cpp -o build/Debug/auditor.o
     g++ -c formatter.cpp -o build/Debug/formatter.o
     g++ -c genai_client.cpp -o build/Debug/genai_client.o
     g++ -c main.cpp -o build/Debug/main.o
     g++ build/Debug/*.o -o build/Debug/outDebug.exe
     ```
   - On Linux/macOS:
     ```bash
     g++ -c auditor.cpp -o build/Debug/auditor.o
     g++ -c formatter.cpp -o build/Debug/formatter.o
     g++ -c genai_client.cpp -o build/Debug/genai_client.o
     g++ -c main.cpp -o build/Debug/main.o
     g++ build/Debug/*.o -o build/Debug/outDebug
     ```

3. **Set Environment Variable**:
   - Set your OpenAI API key:
     - On Windows: `set OPENAI_API_KEY=your-api-key-here`
     - On Linux/macOS: `export OPENAI_API_KEY=your-api-key-here`

## Usage

1. **Run the Program**:
   - Windows: `.\build\Debug\outDebug.exe`
   - Linux/macOS: `./build/Debug/outDebug`

2. **Interact with the CLI**:
   - Enter your query at the prompt (`User >`).
   - The tool will:
     - Check your input for bias and display a score/warnings.
     - Generate an AI response.
     - Audit the response for bias.
     - If bias is found, highlight it, show reasons, and prompt for action (Accept or Regenerate).
   - Type `exit` to quit.

3. **Example Session**:
   ```
   GenAI Bias Auditor CLI
   Set OPENAI_API_KEY in your environment before running.

   User > What are the differences between men and women in leadership?

   Bias Rate Score: 40%
   Warning: Bias detected in user input:
   Highlighted: What are the differences between [men] and [women] in leadership?
    * Potential stereotyping based on gender.

   Assistant: [AI response here]

   [!] Bias detected in AI response:
   Response: [Highlighted biased text in red]
   Explanation: [Details]
   - Reason: [Explanation]
     Resource: [Link]

   Options: [1] Accept | [2] Regenerate
   Select: 2

   Regenerating clean response...
   New Response: [Bias-free response]
   ```

## Configuration

- **API Model**: Currently set to `gpt-4o`. Modify `genai_client.cpp` to change the model.
- **Bias Detection Prompt**: Customize the audit prompt in `auditor.cpp` for different detection criteria.
- **Colors**: ANSI codes are used for highlighting. Ensure your terminal supports them (e.g., Windows Terminal, iTerm2).

## Troubleshooting

- **No Highlighting**: If colors don't appear, check if your terminal supports ANSI escape codes. On Windows, the program enables virtual terminal processing.
- **API Errors**: Ensure `OPENAI_API_KEY` is set correctly. Check network connectivity.
- **Compilation Issues**: Verify all dependencies (e.g., curl) are installed. Use `-std=c++11` if needed.
- **Bias Not Detected**: The AI model may not always identify subtle biases. Adjust prompts for better accuracy.

## Contributing

Contributions are welcome! Please:
- Fork the repository.
- Create a feature branch.
- Submit a pull request with a clear description of changes.
- Ensure code compiles and follows C++ best practices.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Disclaimer

This tool uses AI for bias detection, which may not be 100% accurate. It is intended for educational and awareness purposes. Always review AI outputs critically.