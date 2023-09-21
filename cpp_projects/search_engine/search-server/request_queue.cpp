#include "request_queue.h"
#include "search_server.h"

#include <deque>
#include <string>
#include <vector>


RequestQueue::RequestQueue(const SearchServer& search_server)
        : search_server_(search_server)
        , no_results_requests_(0)
        , current_time_(0)
        {}


std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    const auto result = search_server_.FindTopDocuments(raw_query, status);
    AddRequest(result.size());
    return result;
}
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    const auto documents_top = search_server_.FindTopDocuments(raw_query);
    AddRequest(documents_top.size());
    return documents_top;
}

int RequestQueue::GetNoResultRequests() const {
    return no_results_requests_;
}

void RequestQueue::AddRequest(int results_num) {
    // новый запрос - новая секунда
    ++current_time_;
    // удаляем все результаты поиска, которые устарели
    while (!requests_.empty() && min_in_day_ <= current_time_ - requests_.front().timestamp) {
        if (0 == requests_.front().results) {
            --no_results_requests_;
        }
        requests_.pop_front();
    }
    // сохраняем новый результат поиска
    requests_.push_back({current_time_, results_num});
    if (0 == results_num) {
        ++no_results_requests_;
    }
}