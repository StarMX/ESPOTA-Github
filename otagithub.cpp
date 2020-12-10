#include "otagithub.h"

#ifdef ESP32
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#endif
#ifdef ESP8266
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecureBearSSL.h>
#include <BearSSLHelpers.h>
#endif
#include <ArduinoJson.h>
namespace espota
{
    const char *githubCACertificate =
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEsTCCA5mgAwIBAgIQBOHnpNxc8vNtwCtCuF0VnzANBgkqhkiG9w0BAQsFADBs\n"
        "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
        "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n"
        "ZSBFViBSb290IENBMB4XDTEzMTAyMjEyMDAwMFoXDTI4MTAyMjEyMDAwMFowcDEL\n"
        "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n"
        "LmRpZ2ljZXJ0LmNvbTEvMC0GA1UEAxMmRGlnaUNlcnQgU0hBMiBIaWdoIEFzc3Vy\n"
        "YW5jZSBTZXJ2ZXIgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC2\n"
        "4C/CJAbIbQRf1+8KZAayfSImZRauQkCbztyfn3YHPsMwVYcZuU+UDlqUH1VWtMIC\n"
        "Kq/QmO4LQNfE0DtyyBSe75CxEamu0si4QzrZCwvV1ZX1QK/IHe1NnF9Xt4ZQaJn1\n"
        "itrSxwUfqJfJ3KSxgoQtxq2lnMcZgqaFD15EWCo3j/018QsIJzJa9buLnqS9UdAn\n"
        "4t07QjOjBSjEuyjMmqwrIw14xnvmXnG3Sj4I+4G3FhahnSMSTeXXkgisdaScus0X\n"
        "sh5ENWV/UyU50RwKmmMbGZJ0aAo3wsJSSMs5WqK24V3B3aAguCGikyZvFEohQcft\n"
        "bZvySC/zA/WiaJJTL17jAgMBAAGjggFJMIIBRTASBgNVHRMBAf8ECDAGAQH/AgEA\n"
        "MA4GA1UdDwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIw\n"
        "NAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2Vy\n"
        "dC5jb20wSwYDVR0fBEQwQjBAoD6gPIY6aHR0cDovL2NybDQuZGlnaWNlcnQuY29t\n"
        "L0RpZ2lDZXJ0SGlnaEFzc3VyYW5jZUVWUm9vdENBLmNybDA9BgNVHSAENjA0MDIG\n"
        "BFUdIAAwKjAoBggrBgEFBQcCARYcaHR0cHM6Ly93d3cuZGlnaWNlcnQuY29tL0NQ\n"
        "UzAdBgNVHQ4EFgQUUWj/kK8CB3U8zNllZGKiErhZcjswHwYDVR0jBBgwFoAUsT7D\n"
        "aQP4v0cB1JgmGggC72NkK8MwDQYJKoZIhvcNAQELBQADggEBABiKlYkD5m3fXPwd\n"
        "aOpKj4PWUS+Na0QWnqxj9dJubISZi6qBcYRb7TROsLd5kinMLYBq8I4g4Xmk/gNH\n"
        "E+r1hspZcX30BJZr01lYPf7TMSVcGDiEo+afgv2MW5gxTs14nhr9hctJqvIni5ly\n"
        "/D6q1UEL2tU2ob8cbkdJf17ZSHwD2f2LSaCYJkJA69aSEaRkCldUxPUd1gJea6zu\n"
        "xICaEnL6VpPX/78whQYwvwt/Tv9XBZ0k7YXDK/umdaisLRbvfXknsuvCnQsH6qqF\n"
        "0wGjIChBWUMo0oHjqvbsezt3tkBigAVBRQHvFwY+3sAzm2fTYS5yh+Rp/BIAV0Ae\n"
        "cPUeybQ=\n"
        "-----END CERTIFICATE-----\n";
    const char *amazonawsCACertificate =
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEYzCCA0ugAwIBAgIQAYL4CY6i5ia5GjsnhB+5rzANBgkqhkiG9w0BAQsFADBa\n"
        "MQswCQYDVQQGEwJJRTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJl\n"
        "clRydXN0MSIwIAYDVQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTE1\n"
        "MTIwODEyMDUwN1oXDTI1MDUxMDEyMDAwMFowZDELMAkGA1UEBhMCVVMxFTATBgNV\n"
        "BAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3LmRpZ2ljZXJ0LmNvbTEjMCEG\n"
        "A1UEAxMaRGlnaUNlcnQgQmFsdGltb3JlIENBLTIgRzIwggEiMA0GCSqGSIb3DQEB\n"
        "AQUAA4IBDwAwggEKAoIBAQC75wD+AAFz75uI8FwIdfBccHMf/7V6H40II/3HwRM/\n"
        "sSEGvU3M2y24hxkx3tprDcFd0lHVsF5y1PBm1ITykRhBtQkmsgOWBGmVU/oHTz6+\n"
        "hjpDK7JZtavRuvRZQHJaZ7bN5lX8CSukmLK/zKkf1L+Hj4Il/UWAqeydjPl0kM8c\n"
        "+GVQr834RavIL42ONh3e6onNslLZ5QnNNnEr2sbQm8b2pFtbObYfAB8ZpPvTvgzm\n"
        "+4/dDoDmpOdaxMAvcu6R84Nnyc3KzkqwIIH95HKvCRjnT0LsTSdCTQeg3dUNdfc2\n"
        "YMwmVJihiDfwg/etKVkgz7sl4dWe5vOuwQHrtQaJ4gqPAgMBAAGjggEZMIIBFTAd\n"
        "BgNVHQ4EFgQUwBKyKHRoRmfpcCV0GgBFWwZ9XEQwHwYDVR0jBBgwFoAU5Z1ZMIJH\n"
        "WMys+ghUNoZ7OrUETfAwEgYDVR0TAQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMC\n"
        "AYYwNAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdp\n"
        "Y2VydC5jb20wOgYDVR0fBDMwMTAvoC2gK4YpaHR0cDovL2NybDMuZGlnaWNlcnQu\n"
        "Y29tL09tbmlyb290MjAyNS5jcmwwPQYDVR0gBDYwNDAyBgRVHSAAMCowKAYIKwYB\n"
        "BQUHAgEWHGh0dHBzOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMwDQYJKoZIhvcNAQEL\n"
        "BQADggEBAC/iN2bDGs+RVe4pFPpQEL6ZjeIo8XQWB2k7RDA99blJ9Wg2/rcwjang\n"
        "B0lCY0ZStWnGm0nyGg9Xxva3vqt1jQ2iqzPkYoVDVKtjlAyjU6DqHeSmpqyVDmV4\n"
        "7DOMvpQ+2HCr6sfheM4zlbv7LFjgikCmbUHY2Nmz+S8CxRtwa+I6hXsdGLDRS5rB\n"
        "bxcQKegOw+FUllSlkZUIII1pLJ4vP1C0LuVXH6+kc9KhJLsNkP5FEx2noSnYZgvD\n"
        "0WyzT7QrhExHkOyL4kGJE7YHRndC/bseF/r/JUuOUFfrjsxOFT+xJd1BDKCcYm1v\n"
        "upcHi9nzBhDFKdT3uhaQqNBU4UtJx5g=\n"
        "-----END CERTIFICATE-----\n";
#ifdef ESP32
    // struct SpiRamAllocator
    // {
    //     void *allocate(size_t size)
    //     {
    //         return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
    //     }

    //     void deallocate(void *pointer)
    //     {
    //         heap_caps_free(pointer);
    //     }

    //     void *reallocate(void *ptr, size_t new_size)
    //     {
    //         return heap_caps_realloc(ptr, new_size, MALLOC_CAP_SPIRAM);
    //     }
    // };

    // using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;
#endif

    bool Github::Update()
    {
#ifdef ESP32
        WiFiClientSecure *client = new WiFiClientSecure;
        client->setCACert(githubCACertificate);
#endif
#ifdef ESP8266
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setTrustAnchors(new BearSSL::X509List(githubCACertificate));
#endif
        HTTPClient http;
        http.begin(*client, "https://api.github.com/repos/" REPOSITORY "/releases/latest");
        http.setUserAgent("ESP32-ota");
        auto httpCode = http.GET();
        Serial.println(httpCode);
        if (httpCode == HTTP_CODE_OK)
        {
            const size_t capacity = JSON_ARRAY_SIZE(3) + 3 * JSON_OBJECT_SIZE(13) + 5 * JSON_OBJECT_SIZE(18) + 5560;
            String json = http.getString();
// #ifdef ESP32
//             SpiRamJsonDocument doc(capacity);
// #else
            DynamicJsonDocument doc(capacity);
// #endif
            DeserializationError error = deserializeJson(doc, json);
            if (error)
            {
                // switch (error.code())
                // {
                // case DeserializationError::Ok:
                //     Serial.print(F("Deserialization succeeded"));
                //     break;
                // case DeserializationError::InvalidInput:
                //     Serial.print(F("Invalid input!"));
                //     break;
                // case DeserializationError::NoMemory:
                //     Serial.print(F("Not enough memory"));
                //     break;
                // default:
                //     Serial.print(F("Deserialization failed"));
                //     break;
                // }
                ESP_LOGE("espota", "deserializeJson() failed: %s", error.c_str());
                return false;
            }
            if (!doc.isNull())
            {
                JsonObject root = doc.as<JsonObject>();
                strcpy(version.tag, root["tag_name"].as<String>().c_str());
                strcpy(version.downloadUrl, root["assets"][0]["browser_download_url"].as<String>().c_str());
            }
        }
        http.end();
        return true;
    }
    const char *Github::RealDownloadUrl(const char *filename)
    {
        char downloadUrl[200];
        if (strcasecmp(filename, ""))
            snprintf(downloadUrl, sizeof(downloadUrl), "%s", version.downloadUrl);
        else
            snprintf(downloadUrl, sizeof(downloadUrl), "%s%s%s%s%s", "https://github.com/", REPOSITORY, "/releases/download/", version.tag, filename);

#ifdef ESP32
        WiFiClientSecure *client = new WiFiClientSecure;
        client->setCACert(githubCACertificate);
#endif
#ifdef ESP8266
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setTrustAnchors(new BearSSL::X509List(githubCACertificate));
#endif
        HTTPClient http;
        http.begin(*client, downloadUrl);
        const char *headerKeys[] = {"Location", "Set-Cookie"};
        http.collectHeaders(headerKeys, (size_t)2);
        auto httpCode = http.GET();
        const char *url;
        if (httpCode == 302)
            url = http.header("Location").c_str();
        else
            url = "";
        http.end();
        return url;
    }
    void Github::Upgrade(const char *filename)
    {
        auto url = RealDownloadUrl(filename);

#ifdef ESP32
        WiFiClientSecure *client = new WiFiClientSecure;
        client->setCACert(amazonawsCACertificate);
#endif
#ifdef ESP8266
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setTrustAnchors(new BearSSL::X509List(amazonawsCACertificate));
#endif
#ifdef ESP32
        httpUpdate.update(*client, url);
#endif
#ifdef ESP8266
        ESPhttpUpdate.update(*client, url);
#endif
    }
    Github github;
} // namespace espota
