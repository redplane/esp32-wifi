#include <WirelessNetworkManager.h>
#include <WiFi.h>

//#region Constructor

WirelessNetworkManager::WirelessNetworkManager() {
    this->_lastScanTime = -1.0f;
    this->_availableNetworks = nullptr;
    this->_pageId = 0;
    this->_previousPageId = -1;
}

//#endregion

//#region Methods

void WirelessNetworkManager::scan() {

    // Clear the previous loaded network.
    if (this->_availableNetworks != nullptr) {
        delete this->_availableNetworks;
    }

    this->_previousPageId = -1;
    this->_pageId = 0;

    // WiFi.scanNetworks will return the number of networks found
    int iDiscoveredNetworks = WiFi.scanNetworks();
    delay(20);

    // No network is discovered.
    if (iDiscoveredNetworks == 0) {
        return;
    }

    WirelessNetwork availableNetworks[iDiscoveredNetworks];

    for (int iNetworkIndex = 0; iNetworkIndex < iDiscoveredNetworks; iNetworkIndex++) {
        String ssid(WiFi.SSID(iNetworkIndex));
        char iSignalStrength = WiFi.RSSI(iNetworkIndex);
        bool bIsProtected = WiFi.encryptionType(iNetworkIndex) != WIFI_AUTH_OPEN;

        // Enlist the available networks.
        WirelessNetwork availableNetwork = WirelessNetwork(ssid.c_str(), iSignalStrength, bIsProtected);
        availableNetworks[iNetworkIndex] = availableNetwork;
    }

    this->_availableNetworks = &availableNetworks[0];
}

const WirelessNetwork *WirelessNetworkManager::getAvailableNetworks(int pageId, int maxRecords, int& length) {

    int totalAvailableNetworks = this->getTotalAvailableNetworks();
    if (totalAvailableNetworks < 1) {
        length = 0;
        return new WirelessNetwork[0];
    }

    // Update the page id.
    this->_pageId = pageId;

    // Get the records which must be skipped.
    int skippedRecords = this->_pageId * maxRecords;
    int endRecordId = skippedRecords + maxRecords;
    if (endRecordId > totalAvailableNetworks) {
        endRecordId = totalAvailableNetworks;
    }

    length = endRecordId - skippedRecords;
    WirelessNetwork* availableNetworks = new WirelessNetwork[length];

    Serial.print("Items = ");
    Serial.println(totalAvailableNetworks);

    Serial.print("Skipped records = ");
    Serial.println(skippedRecords);

    Serial.print("End records = ");
    Serial.println(endRecordId);

    int index = 0;
    for (int j = skippedRecords; j < endRecordId; j++) {

        Serial.print("Id ");
        Serial.println(j);

        char szItemTitle[128] = "";
        char szNetworkName[128] = "";
        const bool bIsProtected = (this->_availableNetworks + j)->getProtected();

        strcat(szItemTitle, " ");
        strcat(szNetworkName, (this->_availableNetworks + j)->getName());
        strcat(szItemTitle, szNetworkName);
        strcat(szItemTitle, !bIsProtected ? " " : "*");
        Serial.println(szItemTitle);

        WirelessNetwork availableNetwork(szNetworkName,
                                         (this->_availableNetworks + j)->getStrength(), (this->_availableNetworks + j)->getProtected());
        *(availableNetworks + index) = availableNetwork;
//        *(availableNetworks + index) = availableNetwork;
        index++;
    }

    length = 0;
    return new WirelessNetwork[0];
}

int WirelessNetworkManager::getPageId() const {
    return this->_pageId;
}

void WirelessNetworkManager::disconnect() {
    WiFi.disconnect();
}

void WirelessNetworkManager::markPageLoaded() {
    this->_previousPageId = this->_pageId;
}

bool WirelessNetworkManager::shouldRecordsRefreshed() const {
    return this->_pageId != this->_previousPageId;
}

void WirelessNetworkManager::goForward() {
    // TODO: Check if page can be gone forward or not.
    this->_pageId++;
}

void WirelessNetworkManager::goBackward() {
    // TODO: Check if page can be gone backward or not.
    if (this->_pageId < 2) {
        return;
    }

    this->_pageId--;
}

int WirelessNetworkManager::getTotalAvailableNetworks() {

    if (this->_availableNetworks == nullptr) {
        return 0;
    }

    size_t totalAvailableNetworks = (&this->_availableNetworks)[1] - this->_availableNetworks;
    return totalAvailableNetworks;
}

WirelessNetwork *WirelessNetworkManager::getAvailableNetworks() {
    return this->_availableNetworks;
}


//#endregion
