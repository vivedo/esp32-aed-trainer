#include <Arduino.h>
#include <unity.h>

#include <map>

// Include the WebServer header
#include "WebServer.h"

// Test ParseQuery function with empty query string
void test_parse_query_empty() {
    std::map<String, String> result = WebServer::ParseQuery("");
    TEST_ASSERT_EQUAL(0, result.size());
}

// Test ParseQuery function with null pointer
void test_parse_query_null() {
    std::map<String, String> result = WebServer::ParseQuery(nullptr);
    TEST_ASSERT_EQUAL(0, result.size());
}

// Test ParseQuery function with single parameter
void test_parse_query_single_param() {
    std::map<String, String> result = WebServer::ParseQuery("foo=bar");
    TEST_ASSERT_EQUAL(1, result.size());
    TEST_ASSERT_EQUAL_STRING("bar", result["foo"].c_str());
}

// Test ParseQuery function with multiple parameters
void test_parse_query_multiple_params() {
    std::map<String, String> result = WebServer::ParseQuery("foo=1&bar=2&baz=3");
    TEST_ASSERT_EQUAL(3, result.size());
    TEST_ASSERT_EQUAL_STRING("1", result["foo"].c_str());
    TEST_ASSERT_EQUAL_STRING("2", result["bar"].c_str());
    TEST_ASSERT_EQUAL_STRING("3", result["baz"].c_str());
}

// Test ParseQuery function with parameter without value
void test_parse_query_param_without_equals() {
    std::map<String, String> result = WebServer::ParseQuery("foo&bar=2");
    TEST_ASSERT_EQUAL(1, result.size());
    TEST_ASSERT_EQUAL_STRING("2", result["bar"].c_str());
}

// Test ParseQuery function with empty value
void test_parse_query_empty_value() {
    std::map<String, String> result = WebServer::ParseQuery("foo=&bar=value");
    TEST_ASSERT_EQUAL(2, result.size());
    TEST_ASSERT_EQUAL_STRING("", result["foo"].c_str());
    TEST_ASSERT_EQUAL_STRING("value", result["bar"].c_str());
}

// Test ParseQuery function with special characters in values
void test_parse_query_special_chars() {
    std::map<String, String> result = WebServer::ParseQuery("name=test_value&count=123");
    TEST_ASSERT_EQUAL(2, result.size());
    TEST_ASSERT_EQUAL_STRING("test_value", result["name"].c_str());
    TEST_ASSERT_EQUAL_STRING("123", result["count"].c_str());
}

// Test ParseRequest function with simple GET request
void test_parse_request_simple_get() {
    String request = "GET /index.html HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("GET", req.method);
    TEST_ASSERT_EQUAL_STRING("/index.html", req.path);
    TEST_ASSERT_EQUAL(0, req.queryParams.size());
}

// Test ParseRequest function with POST request
void test_parse_request_post() {
    String request = "POST /api/data HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("POST", req.method);
    TEST_ASSERT_EQUAL_STRING("/api/data", req.path);
    TEST_ASSERT_EQUAL(0, req.queryParams.size());
}

// Test ParseRequest function with query parameters
void test_parse_request_with_query() {
    String request = "GET /api?foo=1&bar=2 HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("GET", req.method);
    TEST_ASSERT_EQUAL_STRING("/api", req.path);
    TEST_ASSERT_EQUAL(2, req.queryParams.size());
    TEST_ASSERT_EQUAL_STRING("1", req.queryParams["foo"].c_str());
    TEST_ASSERT_EQUAL_STRING("2", req.queryParams["bar"].c_str());
}

// Test ParseRequest function with root path
void test_parse_request_root_path() {
    String request = "GET / HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("GET", req.method);
    TEST_ASSERT_EQUAL_STRING("/", req.path);
    TEST_ASSERT_EQUAL(0, req.queryParams.size());
}

// Test ParseRequest function with path and single query param
void test_parse_request_single_query() {
    String request = "GET /search?q=test HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("GET", req.method);
    TEST_ASSERT_EQUAL_STRING("/search", req.path);
    TEST_ASSERT_EQUAL(1, req.queryParams.size());
    TEST_ASSERT_EQUAL_STRING("test", req.queryParams["q"].c_str());
}

// Test ParseRequest function with complex path
void test_parse_request_complex_path() {
    String request = "GET /api/v1/users/123 HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("GET", req.method);
    TEST_ASSERT_EQUAL_STRING("/api/v1/users/123", req.path);
    TEST_ASSERT_EQUAL(0, req.queryParams.size());
}

// Test ParseRequest function with query but no path
void test_parse_request_root_with_query() {
    String request = "GET /?id=123&mode=test HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("GET", req.method);
    TEST_ASSERT_EQUAL_STRING("/", req.path);
    TEST_ASSERT_EQUAL(2, req.queryParams.size());
    TEST_ASSERT_EQUAL_STRING("123", req.queryParams["id"].c_str());
    TEST_ASSERT_EQUAL_STRING("test", req.queryParams["mode"].c_str());
}

// Test ParseRequest with empty query parameter values
void test_parse_request_empty_query_values() {
    String request = "GET /test?enabled=&disabled= HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("GET", req.method);
    TEST_ASSERT_EQUAL_STRING("/test", req.path);
    TEST_ASSERT_EQUAL(2, req.queryParams.size());
    TEST_ASSERT_EQUAL_STRING("", req.queryParams["enabled"].c_str());
    TEST_ASSERT_EQUAL_STRING("", req.queryParams["disabled"].c_str());
}

// Test ParseRequest with PUT method
void test_parse_request_put_method() {
    String request = "PUT /api/update HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("PUT", req.method);
    TEST_ASSERT_EQUAL_STRING("/api/update", req.path);
    TEST_ASSERT_EQUAL(0, req.queryParams.size());
}

// Test ParseRequest with DELETE method
void test_parse_request_delete_method() {
    String request = "DELETE /api/item?id=5 HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("DELETE", req.method);
    TEST_ASSERT_EQUAL_STRING("/api/item", req.path);
    TEST_ASSERT_EQUAL(1, req.queryParams.size());
    TEST_ASSERT_EQUAL_STRING("5", req.queryParams["id"].c_str());
}

// Test ParseRequest with multiple query params and values with numbers
void test_parse_request_numeric_values() {
    String request = "GET /data?x=100&y=200&z=300 HTTP/1.1";
    WebServer::HttpRequest req = WebServer::ParseRequest(request);

    TEST_ASSERT_EQUAL_STRING("GET", req.method);
    TEST_ASSERT_EQUAL_STRING("/data", req.path);
    TEST_ASSERT_EQUAL(3, req.queryParams.size());
    TEST_ASSERT_EQUAL_STRING("100", req.queryParams["x"].c_str());
    TEST_ASSERT_EQUAL_STRING("200", req.queryParams["y"].c_str());
    TEST_ASSERT_EQUAL_STRING("300", req.queryParams["z"].c_str());
}

void setup() {
    delay(2000);  // Wait for serial monitor

    UNITY_BEGIN();

    // ParseQuery tests
    RUN_TEST(test_parse_query_empty);
    RUN_TEST(test_parse_query_null);
    RUN_TEST(test_parse_query_single_param);
    RUN_TEST(test_parse_query_multiple_params);
    RUN_TEST(test_parse_query_param_without_equals);
    RUN_TEST(test_parse_query_empty_value);
    RUN_TEST(test_parse_query_special_chars);

    // ParseRequest tests
    RUN_TEST(test_parse_request_simple_get);
    RUN_TEST(test_parse_request_post);
    RUN_TEST(test_parse_request_with_query);
    RUN_TEST(test_parse_request_root_path);
    RUN_TEST(test_parse_request_single_query);
    RUN_TEST(test_parse_request_complex_path);
    RUN_TEST(test_parse_request_root_with_query);
    RUN_TEST(test_parse_request_empty_query_values);
    RUN_TEST(test_parse_request_put_method);
    RUN_TEST(test_parse_request_delete_method);
    RUN_TEST(test_parse_request_numeric_values);

    UNITY_END();
}

void loop() {}
