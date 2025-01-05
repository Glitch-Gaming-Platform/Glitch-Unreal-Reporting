#include <curl/curl.h>
#include <string>
#include <iostream>

std::string CreateInstallRecord(const std::string& authToken, const std::string& titleId, const std::string& userInstallId, const std::string& platform)
{
    CURL* curl = curl_easy_init();
    if (!curl) return "Failed to init curl";

    std::string url = "https://api.glitch.fun/api/titles/" + titleId + "/installs";

    // JSON payload
    std::string jsonBody = R"({"user_install_id":")" + userInstallId + R"(","platform":")" + platform + R"("})";

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + authToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());

    // Response
    std::string responseString;
    auto WriteCallback = [](char* ptr, size_t size, size_t nmemb, void* userdata) {
        ((std::string*)userdata)->append((char*)ptr, size * nmemb);
        return size * nmemb;
    };
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        responseString = "CURL error: " + std::string(curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return responseString;
}

