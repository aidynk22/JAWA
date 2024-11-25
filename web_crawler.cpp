#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <curl/curl.h>

class WebCrawler {
public:
    WebCrawler(int numThreads);
    void startCrawling(const std::string& startUrl);

private:
    void worker();
    std::string fetchPage(const std::string& url);
    std::vector<std::string> parseUrls(const std::string& html);

    int numThreads;
    std::queue<std::string> urlQueue;
    std::set<std::string> visitedUrls;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    bool stop;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch (std::bad_alloc& e) {
        // Handle memory problem
        return 0;
    }
    return newLength;
}

std::string WebCrawler::fetchPage(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

std::vector<std::string> WebCrawler::parseUrls(const std::string& html) {
    std::vector<std::string> urls;
    // Simple regex or string parsing to extract URLs from HTML
    // This is a placeholder, you should use a proper HTML parser
    size_t pos = 0;
    while ((pos = html.find("href=\"", pos)) != std::string::npos) {
        pos += 6;
        size_t end = html.find("\"", pos);
        if (end != std::string::npos) {
            urls.push_back(html.substr(pos, end - pos));
            pos = end;
        }
    }
    return urls;
}

void WebCrawler::worker() {
    while (true) {
        std::string url;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCV.wait(lock, [this] { return !urlQueue.empty() || stop; });
            if (stop && urlQueue.empty()) return;
            url = urlQueue.front();
            urlQueue.pop();
        }

        std::string html = fetchPage(url);
        std::vector<std::string> urls = parseUrls(html);

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            for (const auto& newUrl : urls) {
                if (visitedUrls.find(newUrl) == visitedUrls.end()) {
                    visitedUrls.insert(newUrl);
                    urlQueue.push(newUrl);
                }
            }
        }
        queueCV.notify_all();
    }
}

WebCrawler::WebCrawler(int numThreads) : numThreads(numThreads), stop(false) {}

void WebCrawler::startCrawling(const std::string& startUrl) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        urlQueue.push(startUrl);
        visitedUrls.insert(startUrl);
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(&WebCrawler::worker, this);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

int main() {
    WebCrawler crawler(4); // Number of threads
    crawler.startCrawling("http://example.com");
    return 0;
}