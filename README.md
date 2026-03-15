# AI Bias Auditor CLI

A command-line interface (CLI) tool that audits text for bias using generative AI. It detects biased language in user inputs and AI-generated responses, highlights problematic content, and provides options for regeneration to ensure fair and unbiased communication.

This is a project inspired by the following case study material.

## 🤖 Case Study: AI Bias
“You are what you eat” might not apply to humans, but it absolutely holds true for AI. Every AI system is enormously influenced by the data it was trained on, as well as the users it interacts with. Biases that are present in either the training data or in the human overseers will often result in those same biases reflected in the trained AI model. Moreover, AI systems are also prone to developing their own biases throughout training and production — like [xAI Grok’s infamous meltdown on Twitter (X) in 2025](https://www.npr.org/2025/07/09/nx-s1-5462609/grok-elon-musk-antisemitic-racist-content), which resulted from a catastrophic combination of xAI founder Elon Musk’s own personal biases entering Grok’s training, and a feedback loop online where Twitter users encouraged Grok to become more and more extreme.

Biases can also be much more subtle in systems that seem much more innocent. With the prevalence of AI today, AI systems are now used for [classifying images](http://gendershades.org/overview.html), [algorithmically deciding what content you’ll watch](https://pmc.ncbi.nlm.nih.gov/articles/PMC11373151/), and even [choosing the best job applicant](https://www.canada.ca/en/public-service-commission/services/appointment-framework/guides-tools-appointment-framework/ai-hiring-process.html). In these cases, the subtle biases in the AI will often reinforce stereotypes, gender inequality, discrimination, and misrepresentation. 

<mark>Your challenge is to create something that makes AI fairer and more inclusive — whether that’s a tool that detects when bias occurs, a resource that helps users understand where and how bias happens, or a way of reducing or correcting for AI bias.</mark>

Further reading:
[Biases in AI: acknowledging and addressing the inevitable ethical issues ](https://pmc.ncbi.nlm.nih.gov/articles/PMC12405166/)
[When AI Amplifies the Biases of Its Users](https://hbr.org/2026/01/when-ai-amplifies-the-biases-of-its-users)
The [Gender Shades project](http://gendershades.org/overview.html)
The [Algorithmic Justice League](https://www.ajl.org/)

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
