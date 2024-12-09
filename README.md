# JAWA (Multithreaded Web Crawler in C++)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Quick Links
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

## Installation Guide

### Prerequisites

#### Windows (Using MSYS2)
```bash
# 1. Download and install MSYS2 from https://www.msys2.org

# 2. Open MSYS2 terminal and update
pacman -Syu

# 3. Install required packages
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-curl
pacman -S mingw-w64-x86_64-cmake
pacman -S make

# 4. Add MSYS2 to PATH (if not already done)
# Add this to your PATH environment variable:
# C:\msys64\mingw64\bin
```

#### Linux (Ubuntu/Debian)
```bash
# Update package list
sudo apt update

# Install required packages
sudo apt install g++
sudo apt install libcurl4-openssl-dev
sudo apt install cmake
sudo apt install make
```

#### macOS
```bash
# Install Homebrew if not installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install required packages
brew install gcc
brew install curl
brew install cmake
```

### Building the Project

1. Clone the repository:
```bash
git clone https://github.com/aidynk22/JAWA.git
cd JAWA
```

2. Create and build:
```bash
mkdir build
cd build
cmake ..
make
```

## Usage Guide

### Running the Program
```bash
./web_crawler
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

### Output Format
```
Starting crawler with 4 threads for 30 seconds...

Crawled: https://example.com
Crawled: https://example.com/page1
...

Pages processed: 10 | Queue size: 25 | Time remaining: 20s
```

## Troubleshooting

### Common Issues

1. **Compilation Errors**
- curl.h not found: Install libcurl development package
- Compiler errors: Ensure C++20 compatible compiler
- Build errors: Check CMAKE configuration

2. **Runtime Issues**
- "Unable to connect": Check internet connection
- "Invalid URL": Verify URL format
- "Thread creation failed": Reduce thread count

### VS Code Setup
If using VS Code:
1. Install C/C++ extension
2. Configure c_cpp_properties.json
3. Set up build tasks
4. Use C++20 standard

## Project Structure
```
JAWA/
├── web_crawler.cpp    # Main source code
├── README.md         # Project documentation
├── .vscode/          # VS Code configuration
│   └── c_cpp_properties.json
└── build/           # Compiled binaries
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
