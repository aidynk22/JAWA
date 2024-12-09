# Project Title: JAWA (Multithreaded Web Crawler in C++)

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

## Inputs
The program takes the following inputs from the user:
1. Starting URL: The initial URL from which the crawling begins.
2. Number of threads: The number of worker threads to use for crawling (1 to the number of hardware threads).
3. Crawl duration: The duration in seconds for which the crawler should run.

## Outputs
The program outputs the following:
1. Real-time progress: As the crawler runs, it displays the number of pages processed, the current queue size, and the remaining time.
2. Crawled URLs: Each URL successfully crawled is printed to the console.
3. Final statistics: After the crawl completes, the total number of pages processed is displayed.

## Usage
To compile and run the web crawler:

1. Install libcurl:
   - On Windows (using MSYS2): `pacman -S mingw-w64-x86_64-curl`
   - On Ubuntu/Debian: `sudo apt-get install libcurl4-openssl-dev`

2. Compile the code (if the code is not already compiled (check the current directory for "web_crawler")):
   ```sh
   g++ -o web_crawler web_crawler.cpp -lcurl -pthread
   ```

3. Run the executable:
   ```sh
   ./web_crawler
   ```

4. Enter the starting URL, number of threads, and crawl duration when prompted.

The crawler will start running and display progress in real-time. After the specified duration, it will stop and display the final statistics.

## Code Structure
The code is organized into the following main components:
- `URLQueue`: A thread-safe queue to store URLs to be crawled.
- `WebCrawler`: The main crawler class that manages worker threads and performs the crawling.
- `main`: The entry point of the program that handles user input and starts the crawler.

The code utilizes C++20 features and requires a compatible compiler.

## Dependencies
- libcurl: For making HTTP requests.
- C++20 standard library: For threading, synchronization, and I/O operations.

```sh
g++ -o web_crawler [web_crawler.cpp](http://_vscodecontentref_/1) -lcurl -pthread
./web_crawler