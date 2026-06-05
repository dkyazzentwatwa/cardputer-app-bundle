#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <M5Cardputer.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <vector>

#include "CypherSplash.h"

#if __has_include("secrets.h")
#include "secrets.h"
#endif

#ifndef NEWS_WIFI_SSID
#define NEWS_WIFI_SSID ""
#endif

#ifndef NEWS_WIFI_PASSWORD
#define NEWS_WIFI_PASSWORD ""
#endif

#ifndef GUARDIAN_API_KEY
#define GUARDIAN_API_KEY ""
#endif

namespace {

constexpr int kScreenW = 240;
constexpr int kScreenH = 135;
constexpr int kHeaderH = 18;
constexpr int kFooterH = 16;
constexpr int kBodyY = kHeaderH + 4;
constexpr int kBodyH = kScreenH - kHeaderH - kFooterH - 6;
constexpr int kMaxArticles = 10;
constexpr int kLineChars = 36;
constexpr char kKeyUp = ';';
constexpr char kKeyDown = '.';
constexpr const char* kGuardianBase = "https://content.guardianapis.com";
constexpr uint16_t kCypherBg = 0xffdf;
constexpr uint16_t kCypherSurface = 0xef3f;
constexpr uint16_t kCypherSurfaceStrong = 0xde7f;
constexpr uint16_t kCypherPrimary = 0x723c;
constexpr uint16_t kCypherAccent = 0x05bb;
constexpr uint16_t kCypherText = 0x18c6;
constexpr uint16_t kCypherMuted = 0x6aef;
constexpr uint16_t kCypherOnPrimary = TFT_WHITE;

enum class Screen {
    Query,
    Loading,
    Results,
    Reading,
    Error,
};

struct Article {
    String title;
    String section;
    String date;
    String apiUrl;
    String webUrl;
    String trail;
    String body;
};

std::vector<Article> articles;
Screen screen = Screen::Query;
String query = "technology";
String errorText;
int selected = 0;
int listTop = 0;
int readLineTop = 0;
std::vector<String> readLines;

String stripHtml(String text) {
    text.replace("&amp;", "&");
    text.replace("&quot;", "\"");
    text.replace("&#39;", "'");
    text.replace("&apos;", "'");
    text.replace("&lt;", "<");
    text.replace("&gt;", ">");
    bool inside = false;
    String out;
    out.reserve(text.length());
    for (size_t i = 0; i < text.length(); ++i) {
        const char c = text[i];
        if (c == '<') {
            inside = true;
            continue;
        }
        if (c == '>') {
            inside = false;
            continue;
        }
        if (!inside) {
            out += c;
        }
    }
    out.trim();
    return out;
}

String urlEncode(const String& raw) {
    const char* hex = "0123456789ABCDEF";
    String out;
    for (size_t i = 0; i < raw.length(); ++i) {
        const char c = raw[i];
        if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
            out += c;
        } else if (c == ' ') {
            out += '+';
        } else {
            out += '%';
            out += hex[(c >> 4) & 0x0F];
            out += hex[c & 0x0F];
        }
    }
    return out;
}

String shortenDate(const char* date) {
    if (!date) {
        return "";
    }
    String d(date);
    if (d.length() >= 10) {
        return d.substring(0, 10);
    }
    return d;
}

void drawHeader(const String& title) {
    M5.Lcd.fillRect(0, 0, kScreenW, kHeaderH, kCypherPrimary);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(kCypherOnPrimary, kCypherPrimary);
    M5.Lcd.setCursor(4, 5);
    M5.Lcd.print(title.substring(0, 34));
}

void drawFooter(const String& hint) {
    M5.Lcd.fillRect(0, kScreenH - kFooterH, kScreenW, kFooterH, kCypherSurfaceStrong);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(kCypherText, kCypherSurfaceStrong);
    M5.Lcd.setCursor(4, kScreenH - 12);
    M5.Lcd.print(hint.substring(0, 38));
}

void drawWrappedLine(String text, int x, int y, int widthChars, uint16_t color) {
    M5.Lcd.setTextColor(color, kCypherBg);
    M5.Lcd.setCursor(x, y);
    if (text.length() > static_cast<size_t>(widthChars)) {
        text = text.substring(0, widthChars - 3) + "...";
    }
    M5.Lcd.print(text);
}

void redrawQuery() {
    M5.Lcd.fillScreen(kCypherBg);
    drawHeader("News Search");
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(kCypherMuted, kCypherBg);
    M5.Lcd.setCursor(8, 34);
    M5.Lcd.print("Query:");
    M5.Lcd.fillRect(8, 50, 224, 24, kCypherSurface);
    M5.Lcd.drawRect(8, 50, 224, 24, kCypherPrimary);
    M5.Lcd.setTextColor(kCypherText, kCypherSurface);
    M5.Lcd.setCursor(12, 58);
    M5.Lcd.print(query.substring(0, 32));

    if (strlen(GUARDIAN_API_KEY) == 0) {
        M5.Lcd.setTextColor(TFT_ORANGE, kCypherBg);
        M5.Lcd.setCursor(8, 88);
        M5.Lcd.print("Set src/secrets.h first");
    } else if (WiFi.status() != WL_CONNECTED) {
        M5.Lcd.setTextColor(TFT_ORANGE, kCypherBg);
        M5.Lcd.setCursor(8, 88);
        M5.Lcd.print("WiFi not connected");
    }
    drawFooter("type query  Enter:search  Del:erase");
}

void redrawLoading(const String& message) {
    M5.Lcd.fillScreen(kCypherBg);
    drawHeader("News Reader");
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(kCypherText, kCypherBg);
    M5.Lcd.setCursor(16, 58);
    M5.Lcd.print(message);
    drawFooter("please wait");
}

void redrawError() {
    M5.Lcd.fillScreen(kCypherBg);
    drawHeader("Error");
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_ORANGE, kCypherBg);
    M5.Lcd.setCursor(8, 32);
    M5.Lcd.print(errorText.substring(0, 180));
    drawFooter("Backspace:query");
}

void redrawResults() {
    M5.Lcd.fillScreen(kCypherBg);
    drawHeader("Results: " + query);

    if (articles.empty()) {
        M5.Lcd.setTextColor(kCypherMuted, kCypherBg);
        M5.Lcd.setCursor(12, 58);
        M5.Lcd.print("No articles found");
        drawFooter("s:new search");
        return;
    }

    const int visibleRows = 5;
    if (selected < listTop) {
        listTop = selected;
    }
    if (selected >= listTop + visibleRows) {
        listTop = selected - visibleRows + 1;
    }

    for (int row = 0; row < visibleRows; ++row) {
        const int idx = listTop + row;
        if (idx >= static_cast<int>(articles.size())) {
            break;
        }
        const int y = kBodyY + row * 19;
        const bool isSelected = idx == selected;
        M5.Lcd.fillRect(0, y - 2, kScreenW, 17, isSelected ? kCypherPrimary : kCypherBg);
        M5.Lcd.setTextColor(isSelected ? kCypherOnPrimary : kCypherText, isSelected ? kCypherPrimary : kCypherBg);
        M5.Lcd.setCursor(4, y);
        M5.Lcd.print(isSelected ? ">" : " ");
        M5.Lcd.print(idx + 1);
        M5.Lcd.print(" ");
        String title = articles[idx].title;
        if (title.length() > 30) {
            title = title.substring(0, 27) + "...";
        }
        M5.Lcd.print(title);
        M5.Lcd.setTextColor(isSelected ? kCypherSurfaceStrong : kCypherMuted, isSelected ? kCypherPrimary : kCypherBg);
        M5.Lcd.setCursor(18, y + 9);
        M5.Lcd.print((articles[idx].section + " " + articles[idx].date).substring(0, 34));
    }

    drawFooter("Up/Down:move  Enter:read  s:search");
}

void buildReadLines(const Article& article) {
    readLines.clear();
    auto addWrapped = [](const String& src) {
        String text = src;
        text.replace("\r", " ");
        text.replace("\n", " ");
        text.trim();

        while (text.length() > 0) {
            int take = min(static_cast<int>(text.length()), kLineChars);
            if (take == kLineChars) {
                int lastSpace = -1;
                for (int i = 0; i < take; ++i) {
                    if (text[i] == ' ') {
                        lastSpace = i;
                    }
                }
                if (lastSpace > 14) {
                    take = lastSpace;
                }
            }
            String line = text.substring(0, take);
            line.trim();
            if (line.length() > 0) {
                readLines.push_back(line);
            }
            text = text.substring(take);
            text.trim();
        }
    };

    addWrapped(article.title);
    readLines.push_back("");
    addWrapped(article.section + "  " + article.date);
    readLines.push_back("");
    if (article.body.length() > 0) {
        addWrapped(article.body.substring(0, 2400));
    } else if (article.trail.length() > 0) {
        addWrapped(article.trail);
    } else {
        addWrapped("No article body returned by the API.");
    }
    readLines.push_back("");
    addWrapped(article.webUrl);
}

void redrawReading() {
    M5.Lcd.fillScreen(kCypherBg);
    drawHeader("Read " + String(selected + 1) + "/" + String(articles.size()));

    const int visible = 10;
    if (readLineTop < 0) {
        readLineTop = 0;
    }
    if (readLineTop > max(0, static_cast<int>(readLines.size()) - visible)) {
        readLineTop = max(0, static_cast<int>(readLines.size()) - visible);
    }

    M5.Lcd.setTextSize(1);
    for (int row = 0; row < visible; ++row) {
        const int idx = readLineTop + row;
        if (idx >= static_cast<int>(readLines.size())) {
            break;
        }
        const uint16_t color = idx == 0 ? kCypherPrimary : kCypherText;
        drawWrappedLine(readLines[idx], 4, kBodyY + row * 10, kLineChars, color);
    }

    drawFooter("Up/Down:scroll  Backspace:list");
}

bool connectWifi() {
    if (strlen(NEWS_WIFI_SSID) == 0) {
        errorText = "Missing NEWS_WIFI_SSID in src/secrets.h";
        return false;
    }
    WiFi.mode(WIFI_STA);
    WiFi.begin(NEWS_WIFI_SSID, NEWS_WIFI_PASSWORD);
    redrawLoading("Connecting WiFi...");

    const unsigned long started = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - started < 15000) {
        delay(250);
        M5Cardputer.update();
    }

    if (WiFi.status() != WL_CONNECTED) {
        errorText = "WiFi failed. Check SSID/password.";
        return false;
    }
    return true;
}

bool httpGetJson(const String& url, JsonDocument& doc) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.setTimeout(12000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    if (!http.begin(client, url)) {
        errorText = "HTTP begin failed.";
        return false;
    }

    const int code = http.GET();
    if (code != HTTP_CODE_OK) {
        errorText = "HTTP " + String(code) + " from Guardian.";
        http.end();
        return false;
    }

    const String payload = http.getString();
    http.end();

    DeserializationError err = deserializeJson(doc, payload);
    if (err) {
        errorText = "JSON parse failed: " + String(err.c_str());
        return false;
    }
    return true;
}

bool fetchSearch() {
    if (strlen(GUARDIAN_API_KEY) == 0) {
        errorText = "Missing GUARDIAN_API_KEY in src/secrets.h";
        return false;
    }
    if (WiFi.status() != WL_CONNECTED && !connectWifi()) {
        return false;
    }

    articles.clear();
    selected = 0;
    listTop = 0;
    redrawLoading("Fetching headlines...");

    String url = String(kGuardianBase) +
                 "/search?q=" + urlEncode(query) +
                 "&page-size=" + String(kMaxArticles) +
                 "&order-by=newest&show-fields=trailText,standfirst,byline" +
                 "&api-key=" + urlEncode(GUARDIAN_API_KEY);

    JsonDocument doc;
    if (!httpGetJson(url, doc)) {
        return false;
    }

    JsonArray results = doc["response"]["results"].as<JsonArray>();
    for (JsonObject item : results) {
        Article article;
        article.title = item["webTitle"].as<String>();
        article.section = item["sectionName"].as<String>();
        article.date = shortenDate(item["webPublicationDate"]);
        article.apiUrl = item["apiUrl"].as<String>();
        article.webUrl = item["webUrl"].as<String>();
        article.trail = stripHtml(item["fields"]["trailText"].as<String>());
        if (article.trail.length() == 0) {
            article.trail = stripHtml(item["fields"]["standfirst"].as<String>());
        }
        articles.push_back(article);
    }

    return true;
}

bool fetchArticleBody(Article& article) {
    if (article.body.length() > 0) {
        return true;
    }
    if (article.apiUrl.length() == 0) {
        article.body = article.trail;
        return true;
    }

    redrawLoading("Fetching article...");

    String url = article.apiUrl +
                 "?show-fields=bodyText,trailText,standfirst,byline" +
                 "&api-key=" + urlEncode(GUARDIAN_API_KEY);
    JsonDocument doc;
    if (!httpGetJson(url, doc)) {
        return false;
    }

    JsonObject content = doc["response"]["content"];
    article.body = stripHtml(content["fields"]["bodyText"].as<String>());
    if (article.body.length() == 0) {
        article.body = stripHtml(content["fields"]["trailText"].as<String>());
    }
    if (article.body.length() == 0) {
        article.body = article.trail;
    }
    return true;
}

void readSelectedArticle() {
    if (articles.empty()) {
        return;
    }
    if (!fetchArticleBody(articles[selected])) {
        screen = Screen::Error;
        redrawError();
        return;
    }
    readLineTop = 0;
    buildReadLines(articles[selected]);
    screen = Screen::Reading;
    redrawReading();
}

void runSearch() {
    query.trim();
    if (query.length() == 0) {
        query = "technology";
    }
    screen = Screen::Loading;
    if (fetchSearch()) {
        screen = Screen::Results;
        redrawResults();
    } else {
        screen = Screen::Error;
        redrawError();
    }
}

void handleKeyboard() {
    M5Cardputer.update();
    if (!M5Cardputer.Keyboard.isChange() || !M5Cardputer.Keyboard.isPressed()) {
        return;
    }

    Keyboard_Class::KeysState keys = M5Cardputer.Keyboard.keysState();
    const bool up = M5Cardputer.Keyboard.isKeyPressed(kKeyUp);
    const bool down = M5Cardputer.Keyboard.isKeyPressed(kKeyDown);

    if (screen == Screen::Query) {
        for (char c : keys.word) {
            if (isPrintable(c) && query.length() < 64) {
                query += c;
            }
        }
        if (keys.del && query.length() > 0) {
            query.remove(query.length() - 1);
        }
        if (keys.enter) {
            runSearch();
            return;
        }
        redrawQuery();
        return;
    }

    if (screen == Screen::Results) {
        if (up && selected > 0) {
            selected--;
            redrawResults();
        } else if (down && selected < static_cast<int>(articles.size()) - 1) {
            selected++;
            redrawResults();
        } else if (keys.enter) {
            readSelectedArticle();
        } else if (keys.del) {
            screen = Screen::Query;
            redrawQuery();
        } else {
            for (char c : keys.word) {
                if (c == 's' || c == 'S') {
                    screen = Screen::Query;
                    redrawQuery();
                    return;
                }
            }
        }
        return;
    }

    if (screen == Screen::Reading) {
        if (up && readLineTop > 0) {
            readLineTop--;
            redrawReading();
        } else if (down && readLineTop < static_cast<int>(readLines.size()) - 1) {
            readLineTop++;
            redrawReading();
        } else if (keys.del) {
            screen = Screen::Results;
            redrawResults();
        }
        return;
    }

    if (screen == Screen::Error && keys.del) {
        screen = Screen::Query;
        redrawQuery();
    }
}

}  // namespace

void setup() {
    M5Cardputer.begin();
    M5.Lcd.setRotation(1);
    M5.Lcd.setBrightness(160);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(kCypherText, kCypherBg);
    showCypherSplash();

    if (strlen(NEWS_WIFI_SSID) > 0) {
        connectWifi();
    }
    redrawQuery();
}

void loop() {
    handleKeyboard();
    delay(25);
}
