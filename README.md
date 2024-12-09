# JAWA (Multithreaded Web Crawler in C++)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Quick Start
```bash
# Clone repository
git clone https://github.com/aidynk22/JAWA.git
cd JAWA

# Compile (Linux/macOS)
g++ -std=c++20 web_crawler.cpp -lcurl -pthread -o crawler

# Run
./crawler
```

## Quick Links
- [Requirements](#requirements)
- [Installation Guide](#installation-guide)
- [Usage Guide](#usage-guide)
- [Troubleshooting](#troubleshooting)

## Contributors
- Jannet D.
- Anthony C.
- William Z.
- Aidyn K.

## Description
JAWA is a multithreaded web crawler implemented in C++ for an Operating Systems class project. It demonstrates the use of threads, synchronization primitives, and I/O operations to efficiently crawl web pages. The crawler starts from a given URL, fetches web pages using libcurl, parses the HTML to extract further URLs to crawl, and continues this process using multiple worker threads.

## Features
- Multithreaded crawling for improved performance
- Uses libcurl for HTTP requests
- Parses HTML using regular expressions to extract URLs
- Utilizes mutexes and condition variables for thread synchronization
- Maintains a thread-safe queue to store URLs to be crawled
- Supports a configurable number of worker threads and crawl duration

## Requirements
- C++20 compatible compiler
- libcurl library
- Active internet connection
- Minimum 1GB RAM recommended
- Terminal/Command Prompt access

## Installation Guide

### Prerequisites

#### Option 1: Direct Installation of Required Packages

##### Windows (Using MSYS2)
```bash
# 1. Download and install MSYS2 from https://www.msys2.org

# 2. Open MSYS2 terminal and update
pacman -Syu

# 3. Install required packages
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-curl

# 4. Add to PATH (if not already done)
# Add this to your PATH environment variable:
# C:\msys64\mingw64\bin
```

##### Linux (Ubuntu/Debian)
```bash
# Install required packages
sudo apt update
sudo apt install g++ libcurl4-openssl-dev
```

##### macOS
```bash
# Install required packages using Homebrew
brew install gcc curl
```

### Building and Running

1. Get the Code
```bash
# Clone the repository
git clone https://github.com/aidynk22/JAWA.git
cd JAWA
```

2. Compile
```bash
# Linux/macOS
g++ -std=c++20 web_crawler.cpp -lcurl -pthread -o crawler

# Windows (MSYS2)
g++ -std=c++20 web_crawler.cpp -lcurl -pthread -o crawler.exe
```

Example successful compilation output:
```bash
$ g++ -std=c++20 web_crawler.cpp -lcurl -pthread -o crawler
$ # No output means successful compilation
$ ls
crawler  web_crawler.cpp  README.md  LICENSE.md
```

## Usage Guide

### Running the Program
```bash
./crawler    # Linux/macOS
./crawler.exe  # Windows
```

### Program Configuration
The program will prompt for three inputs:

1. **Starting URL**
```
Enter URL to crawl: https://example.com
```
- Must start with http:// or https://
- Should be a valid, accessible URL

2. **Thread Count**
```
Enter number of threads (1-X): 4
```
- X is your CPU's thread count
- Recommended: Start with 2-4 threads
- Don't exceed available CPU threads

3. **Duration**
```
Enter crawl duration in seconds: 30
```
- Recommended: Start with 30 seconds
- Can be increased for longer crawls

### Example Output
```
Starting crawler with 4 threads for 30 seconds...

Crawled: https://example.com
Crawled: https://example.com/page1
...

Pages processed: 10 | Queue size: 25 | Time remaining: 20s
```

## Known Issues
1. Some websites may block automated crawling
2. HTTPS certificates might need manual handling on Windows
3. Very long crawl durations might consume significant memory

## Troubleshooting

### Common Issues

1. **Compilation Errors**
- curl.h not found: Install libcurl development package
- Compiler errors: Ensure C++20 compatible compiler
- Windows path issues: Verify MSYS2 PATH setup

2. **Runtime Issues**
- "Unable to connect": Check internet connection
- "Invalid URL": Verify URL format
- "Thread creation failed": Reduce thread count

## Project Structure
```
JAWA/
├── web_crawler.cpp    # Main source code
├── README.md         # Project documentation
├── LICENSE.md        # MIT license
└── .vscode/          # VS Code configuration
```

## Performance Tips
- Use SSD for better I/O performance
- Close resource-heavy applications
- Ensure stable internet connection
- Monitor system resource usage

## Support
- Open issues on GitHub: https://github.com/aidynk22/JAWA/issues
- Include error messages and system details
- Describe steps to reproduce issues

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
```
