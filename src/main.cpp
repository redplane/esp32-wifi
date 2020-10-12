#include <Wire.h> //Including wire library
#include "WiFi.h"
#include "WirelessNetwork.h"
#include <HTTPClient.h>
#include <WirelessNetworkManager.h>
#include <cstring>

#define NETWORK_VIEW_ID 1


#define NETWORK_STATUS_LOADED 1
#define NETWORK_STATUS_LOADING  2


// Instance for managing wireless connection.
WirelessNetworkManager *m_WirelessNetworkManager = new WirelessNetworkManager();

int m_iMaxNetworkPageItems = 4;
bool m_bMustLoadWifiNetworks = true;
int m_iActivatedScreen = NETWORK_VIEW_ID;
int m_NetworkProcessStatus = NETWORK_STATUS_LOADING;

void setup() {

    // Open serial connection.
    Serial.begin(115000);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
}

void loop() {

    String message;

    if (m_iActivatedScreen != NETWORK_VIEW_ID) {
        return;
    }

    // Networks must be loaded.
    if (m_bMustLoadWifiNetworks) {

        m_bMustLoadWifiNetworks = false;
        m_NetworkProcessStatus = NETWORK_STATUS_LOADING;

        // Scan for available networks.
        m_WirelessNetworkManager->scan();
        delay(50);

        // No network is discovered.
        if (m_WirelessNetworkManager->getTotalAvailableNetworks() < 1) {
            return;
        }

        WirelessNetwork *availableNetworks = m_WirelessNetworkManager->getAvailableNetworks();
        for (int i = 0; i < m_WirelessNetworkManager->getTotalAvailableNetworks(); i++) {
            char szItemTitle[128] = "";
            char szValue[10] = "";
            char szNetworkName[128] = "";
            char* name = new char[128];
            bool bIsProtected = (m_WirelessNetworkManager + i);

            strcat(szItemTitle, " ");

            strcat(szNetworkName, reinterpret_cast<const char *>(*(availableNetworks + i)->getName()));
            strcat(szItemTitle, szNetworkName);
            strcat(szItemTitle, !bIsProtected ? " " : "*");

            // Log the networks onto terminal
            Serial.println(szItemTitle);
        }

        m_NetworkProcessStatus = NETWORK_STATUS_LOADED;
    }

    // Wifi already loaded.
    if (m_NetworkProcessStatus == NETWORK_STATUS_LOADED && m_WirelessNetworkManager->shouldRecordsRefreshed()) {

        int pageId = m_WirelessNetworkManager->getPageId();

        // Get the available networks which have been paginated.
        int length = 0;
        const WirelessNetwork *paginatedNetworks = m_WirelessNetworkManager->getAvailableNetworks(pageId,
                                                                                            m_iMaxNetworkPageItems,
                                                                                            length);

        Serial.print("Length = ");
        Serial.println(length);
        for (int iNetworkIndex = 0; iNetworkIndex < length; iNetworkIndex++) {

            // Get the available network.
            WirelessNetwork availableNetwork = *(paginatedNetworks + iNetworkIndex);

            char szItemTitle[128] = "";
            char szValue[10] = "";
            char szNetworkName[128] = "";
            const bool bIsProtected = availableNetwork.getProtected();

            strcat(szItemTitle, " ");
            strcat(szNetworkName, availableNetwork.getName());
            strcat(szItemTitle, szNetworkName);
            strcat(szItemTitle, !bIsProtected ? " " : "*");

            delay(20);
        }

        m_WirelessNetworkManager->markPageLoaded();
    }
}