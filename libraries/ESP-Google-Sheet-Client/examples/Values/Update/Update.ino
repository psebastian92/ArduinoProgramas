
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: suwatchai@outlook.com
 *
 * Github: https://github.com/mobizt
 *
 * Copyright (c) 2023 mobizt
 *
 */

// This example shows how to update the spreadsheet's values.

#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif

#include <ESP_Google_Sheet_Client.h>

// For SD/SD_MMC mounting helper
#include <GS_SDHelper.h>

#define WIFI_SSID "WIFI_AP"
#define WIFI_PASSWORD "WIFI_PASSWORD"

// For how to create Service Account and how to use the library, go to https://github.com/mobizt/ESP-Google-Sheet-Client

#define PROJECT_ID "PROJECT_ID"

// Service Account's client email
#define CLIENT_EMAIL "CLIENT_EMAIL"

// Service Account's private key
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----XXXXXXXXXXXX-----END PRIVATE KEY-----\n";

/**
const char rootCACert[] PROGMEM = "-----BEGIN CERTIFICATE-----\n"
                                  "MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n"
                                  "CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n"
                                  "MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n"
                                  "MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n"
                                  "Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n"
                                  "A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n"
                                  "27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n"
                                  "Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n"
                                  "TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n"
                                  "qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n"
                                  "szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n"
                                  "Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n"
                                  "MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n"
                                  "wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n"
                                  "aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n"
                                  "VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n"
                                  "AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n"
                                  "FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n"
                                  "C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n"
                                  "QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n"
                                  "h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n"
                                  "7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n"
                                  "ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n"
                                  "MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n"
                                  "Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n"
                                  "6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n"
                                  "0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n"
                                  "2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n"
                                  "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n"
                                  "-----END CERTIFICATE-----\n";
*/

bool taskComplete = false;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

void tokenStatusCallback(TokenInfo info);

void setup()
{

    Serial.begin(115200);
    Serial.println();
    Serial.println();

    GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

#if defined(ESP32) || defined(ESP8266)
    WiFi.setAutoReconnect(true);
#endif

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    multi.addAP(WIFI_SSID, WIFI_PASSWORD);
    multi.run();
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        if (millis() - ms > 10000)
            break;
#endif
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    // In case SD/SD_MMC storage file access, mount the SD/SD_MMC card.
    // SD_Card_Mounting(); // See src/GS_SDHelper.h

    // GSheet.setCert(rootCACert); // or GSheet.setCertFile("path/to/certificate/file.pem", esp_google_sheet_file_storage_type_flash /* or esp_google_sheet_file_storage_type_sd */);

    // Set the callback for Google API access token generation status (for debug only)
    GSheet.setTokenCallback(tokenStatusCallback);

    // The WiFi credentials are required for Pico W
    // due to it does not have reconnect feature.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    GSheet.clearAP();
    GSheet.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

    // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
    GSheet.setPrerefreshSeconds(10 * 60);

    // Begin the access token generation for Google API authentication
    GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);

    // Or begin with the Service Account JSON file
    // GSheet.begin("path/to/serviceaccount/json/file", esp_google_sheet_file_storage_type_flash /* or esp_google_sheet_file_storage_type_sd */);
}

void loop()
{
    // Call ready() repeatedly in loop for authentication checking and processing
    bool ready = GSheet.ready();

    if (ready && !taskComplete)
    {
        // For basic FirebaseJson usage example, see examples/FirebaseJson/Create_Edit_Parse/Create_Edit_Parse.ino

        // If you assign the spreadsheet id from your own spreadsheet,
        // you need to set share access to the Service Account's CLIENT_EMAIL

        Serial.println("\nUpdate spreadsheet values...");
        Serial.println("----------------------------------------------------------------");

        FirebaseJson response;
        // Instead of using FirebaseJson for response, you can use String for response to the functions
        // especially in low memory device that deserializing large JSON response may be failed as in ESP8266

        FirebaseJson valueRange;

        valueRange.add("range", "Sheet1!A1:B5");
        valueRange.add("majorDimension", "ROWS");
        valueRange.set("values/[0]/[0]", "1"); // row 1/ column 1
        valueRange.set("values/[1]/[0]", "2"); // row 2/ column 1
        valueRange.set("values/[2]/[0]", "3"); // row 3/ column 1
        valueRange.set("values/[3]/[0]", "4"); // row 4/ column 1
        valueRange.set("values/[4]/[0]", "5"); // row 5/ column 1

        // With formula
        valueRange.set("values/[0]/[1]", "=SUM(A1:A5)");     // row 1/ column 2
        valueRange.set("values/[1]/[1]", "=COUNT(A1:A5)");   // row 2/ column 2
        valueRange.set("values/[2]/[1]", "=AVERAGE(A1:A5)"); // row 3/ column 2

        // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/update

        bool success = GSheet.values.update(&response /* returned response */, "<spreadsheetId>" /* spreadsheet Id to update */, "Sheet1!A1:B5" /* range to update */, &valueRange /* data to update */);
        response.toString(Serial, true);
        Serial.println();

        Serial.println("\nUpdate spreadsheet values in multiple ranges...");
        Serial.println("----------------------------------------------------------------");

        FirebaseJson valueRange1;

        valueRange1.add("range", "Sheet1!C1:D5");
        valueRange1.add("majorDimension", "ROWS");
        valueRange1.set("values/[0]/[0]", "A"); // row 1/ column 3
        valueRange1.set("values/[1]/[0]", "B"); // row 2/ column 3
        valueRange1.set("values/[2]/[0]", "C"); // row 3/ column 3
        valueRange1.set("values/[3]/[0]", "D"); // row 4/ column 3
        valueRange1.set("values/[4]/[0]", "E"); // row 5/ column 3

        valueRange1.set("values/[0]/[1]", "F"); // row 1/ column 4
        valueRange1.set("values/[1]/[1]", "G"); // row 2/ column 4
        valueRange1.set("values/[2]/[1]", "H"); // row 3/ column 4
        valueRange1.set("values/[3]/[1]", "I"); // row 4/ column 4
        valueRange1.set("values/[4]/[1]", "J"); // row 5/ column 4

        FirebaseJson valueRange2;

        valueRange2.add("range", "Sheet1!C6:D10");
        valueRange2.add("majorDimension", "ROWS");
        valueRange2.set("values/[0]/[0]", "1"); // row 6/ column 3
        valueRange2.set("values/[1]/[0]", "2"); // row 7/ column 3
        valueRange2.set("values/[2]/[0]", "3"); // row 8/ column 3
        valueRange2.set("values/[3]/[0]", "4"); // row 9/ column 3
        valueRange2.set("values/[4]/[0]", "5"); // row 10/ column 3

        valueRange2.set("values/[0]/[1]", "6");  // row 6/ column 4
        valueRange2.set("values/[1]/[1]", "7");  // row 7/ column 4
        valueRange2.set("values/[2]/[1]", "8");  // row 8/ column 4
        valueRange2.set("values/[3]/[1]", "9");  // row 9/ column 4
        valueRange2.set("values/[4]/[1]", "10"); // row 10/ column 4

        FirebaseJsonArray valueRangeArr;
        valueRangeArr.add(valueRange1);
        valueRangeArr.add(valueRange2);

        // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/batchUpdate

        success = GSheet.values.batchUpdate(&response /* returned response */, "<spreadsheetId>" /* spreadsheet Id to update */, &valueRangeArr /* array of data range to update */);
        response.toString(Serial, true);
        Serial.println();

        Serial.println("\nUpdate spreadsheet values in multiple ranges by data filter...");
        Serial.println("----------------------------------------------------------------");

        FirebaseJsonArray dataFilterValueRangeArr;

        FirebaseJson dataFilterValueRange1;
        FirebaseJson dataFilterValueRange2;

        dataFilterValueRange1.set("dataFilter/a1Range", "Sheet1!E1:F2");
        dataFilterValueRange1.set("majorDimension", "COLUMNS");
        dataFilterValueRange1.set("values/[0]/[0]", 1); // column 5/ row 1
        dataFilterValueRange1.set("values/[0]/[1]", 2); // column 5/ row 2

        dataFilterValueRange1.set("values/[1]/[0]", 3); // column 6/ row 1
        dataFilterValueRange1.set("values/[1]/[1]", 4); // column 6/ row 2

        dataFilterValueRange2.set("dataFilter/a1Range", "Sheet1!G1:G3");
        dataFilterValueRange2.set("majorDimension", "ROWS");
        dataFilterValueRange2.set("values/[0]/[0]", "one");   // row 1/ column 7
        dataFilterValueRange2.set("values/[1]/[0]", "two");   // row 2/ column 7
        dataFilterValueRange2.set("values/[2]/[0]", "three"); // row 3/ column 7

        dataFilterValueRangeArr.add(dataFilterValueRange1);
        dataFilterValueRangeArr.add(dataFilterValueRange2);

        // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/batchUpdateByDataFilter

        success = GSheet.values.batchUpdateByDataFilter(&response /* returned response */, "<spreadsheetId>" /* spreadsheet Id to update */, &dataFilterValueRangeArr /* array of data range to update by filter */);
        if (success)
            response.toString(Serial, true);
        else
            Serial.println(GSheet.errorReason());
        Serial.println();

#if defined(ESP32) || defined(ESP8266)
        Serial.println(ESP.getFreeHeap());
#elif defined(PICO_RP2040)
        Serial.println(rp2040.getFreeHeap());
#endif

        taskComplete = true;
    }
}

void tokenStatusCallback(TokenInfo info)
{
    if (info.status == token_status_error)
    {
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
        GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
    }
    else
    {
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    }
}