/******************************************************************************
 * Simple Web Crawler
 * A basic multi-threaded web crawler that:
 * - Crawls web pages starting from a given URL
 * - Extracts and follows links from HTML content
 * - Uses multiple threads for parallel crawling
 * - Avoids duplicate URLs
 * - Respects basic politeness delays
 ******************************************************************************/

//=============================================================================
// Includes
//=============================================================================
// Standard Library
#include <iostream>     // For I/O operations
#include <string>       // For string handling
#include <queue>        // For URL queue
#include <unordered_set>// For duplicate URL detection
#include <thread>       // For multi-threading
#include <mutex>        // For thread synchronization
#include <condition_variable> // For thread signaling
#include <regex>        // For HTML parsing

// External Libraries
#include <curl/curl.h>    // For HTTP requests

//=============================================================================
// Thread-Safe URL Queue
//=============================================================================
/**
 * URLQueue: A thread-safe queue that stores URLs and prevents duplicates
 * 
 * Features:
 * - Thread-safe push and pop operations
 * - Automatic duplicate URL detection
 * - Blocking pop operation that waits for new URLs
 * - Graceful shutdown support
 */
class URLQueue {
    std::queue<std::string> urls;          // Queue of URLs to crawl
    std::unordered_set<std::string> seen;  // Set of URLs already seen
    std::mutex mtx;                        // Mutex for thread safety
    std::condition_variable cv;            // For blocking pop operation
    bool done = false;                     // Shutdown flag

public:
    // Add a URL to the queue if not seen before
    void push(const std::string& url) {
        std::lock_guard<std::mutex> lock(mtx);
        if (seen.find(url) == seen.end()) {
            urls.push(url);
            seen.insert(url);
            cv.notify_one();  // Wake up one waiting thread
        }
    }

    // Get and remove the next URL from the queue
    bool pop(std::string& url) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !urls.empty() || done; });
        if (urls.empty() && done) return false;
        url = urls.front();
        urls.pop();
        return true;
    }

    // Signal shutdown to all waiting threads
    void finish() {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
        cv.notify_all();
    }

    // Get current queue size
    size_t size() const {
        std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mtx));
        return urls.size();
    }
};

//=============================================================================
// Web Crawler Implementation
//=============================================================================
/**
 * WebCrawler: Main crawler implementation that manages multiple worker threads
 * 
 * Features:
 * - Multi-threaded crawling
 * - HTML link extraction
 * - Progress tracking
 * - Graceful shutdown
 */
class WebCrawler {
    URLQueue queue;                        // Thread-safe URL queue
    std::vector<std::thread> workers;      // Worker threads
    std::atomic<bool> running{false};      // Running state
    std::atomic<size_t> pagesProcessed{0}; // Progress counter
    std::mutex printMutex;                 // Mutex for console output
    const int threadCount;                 // Number of worker threads
    const std::string userAgent = "SimpleCrawler/1.0";

    // CURL write callback
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
        userp->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    // Extract links from HTML content
    std::vector<std::string> extractLinks(const std::string& html, const std::string& baseUrl) {
        std::vector<std::string> links;
        std::regex linkRegex(R"(<a[^>]+href=["']([^"']+)["'])");
        
        auto linksBegin = std::sregex_iterator(html.begin(), html.end(), linkRegex);
        auto linksEnd = std::sregex_iterator();

        for (auto i = linksBegin; i != linksEnd; ++i) {
            std::string link = (*i)[1];
            if (link.starts_with("http")) {
                links.push_back(link);
            }
            else if (link.starts_with("/")) {
                // Handle absolute paths
                size_t pos = baseUrl.find("/", 8); // Skip "https://"
                std::string domain = pos != std::string::npos ? baseUrl.substr(0, pos) : baseUrl;
                links.push_back(domain + link);
            }
        }
        return links;
    }

    // Crawl a single page
    void crawlPage(const std::string& url) {
        CURL* curl = curl_easy_init();
        if (!curl) return;

        std::string buffer;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            {
                std::lock_guard<std::mutex> lock(printMutex);
                std::cout << "Crawled: " << url << std::endl;
            }
            pagesProcessed++;

            auto links = extractLinks(buffer, url);
            for (const auto& link : links) {
                queue.push(link);
            }
        }

        curl_easy_cleanup(curl);
    }

    // Worker thread function
    void worker() {
        std::string url;
        while (running && queue.pop(url)) {
            try {
                crawlPage(url);
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Politeness delay
            } catch (const std::exception& e) {
                std::cerr << "Error crawling " << url << ": " << e.what() << std::endl;
            }
        }
    }

public:
    // Initialize crawler with specified number of threads
    explicit WebCrawler(int threads) : threadCount(threads) {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    // Clean up resources
    ~WebCrawler() {
        stop();
        curl_global_cleanup();
    }

    // Start crawling from the given URL
    void start(const std::string& startUrl) {
        running = true;
        queue.push(startUrl);

        for (int i = 0; i < threadCount; ++i) {
            workers.emplace_back(&WebCrawler::worker, this);
        }
    }

    // Stop all crawling
    void stop() {
        running = false;
        queue.finish();
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        workers.clear();
    }

    // Get statistics
    size_t getPagesProcessed() const { return pagesProcessed; }
    size_t getQueueSize() const { return queue.size(); }
};

//=============================================================================
// Main Program
//=============================================================================
int main() {
    try {
        // Get user input
        std::string url;
        std::cout << "Enter URL to crawl: ";
        std::getline(std::cin, url);
        
        int threadCount;
        std::cout << "Enter number of threads (1-" 
                  << std::thread::hardware_concurrency() << "): ";
        std::cin >> threadCount;
        threadCount = std::clamp(threadCount, 1, 
                               (int)std::thread::hardware_concurrency());

        int seconds;
        std::cout << "Enter crawl duration in seconds: ";
        std::cin >> seconds;

        // Initialize and start crawler
        WebCrawler crawler(threadCount);
        std::cout << "\nStarting crawler with " << threadCount 
                  << " threads for " << seconds << " seconds...\n\n";
        crawler.start(url);

        // Monitor progress
        auto startTime = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - startTime < std::chrono::seconds(seconds)) {
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - startTime
            ).count();
            
            std::cout << "Pages processed: " << crawler.getPagesProcessed() 
                     << " | Queue size: " << crawler.getQueueSize()
                     << " | Time remaining: " << (seconds - elapsed) << "s\r" << std::flush;
            
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Clean up and show results
        crawler.stop();
        std::cout << "\n\nCrawl completed!" << std::endl;
        std::cout << "Total pages processed: " << crawler.getPagesProcessed() << std::endl;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
