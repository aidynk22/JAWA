# Project Title: JAWA (Multithreaded Web Crawler in C++)

## Contributors
- Jannet D.
- Anthony C.
- William Z.
- Aidyn K.

## Description
JAWA is a multithreaded web crawler implemented in C++. It is designed to efficiently crawl web pages using multiple threads, fetching and parsing URLs concurrently.

## Features
- Multithreaded crawling for improved performance
- Fetches web pages using libcurl
- Parses HTML to extract URLs
- Uses mutexes and condition variables for thread synchronization

## Usage
To start the web crawler, compile the code and run the executable with the starting URL:

```sh
g++ -o web_crawler [web_crawler.cpp](http://_vscodecontentref_/1) -lcurl -pthread
./web_crawler