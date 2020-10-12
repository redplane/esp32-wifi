//
// Created by Redplane on 2020-10-05.
//

#ifndef INDOORWEATHERSTATION_WIRELESSNETWORKMANAGER_H
#define INDOORWEATHERSTATION_WIRELESSNETWORKMANAGER_H

#include "WirelessNetwork.h"
#include <vector>

class WirelessNetworkManager {

private:

    // Available networks that have been searched.
    WirelessNetwork* _availableNetworks;

    // The last time when networks are scanned.
    float _lastScanTime;

    // Id of network which item will be retrieved previously.
    int _previousPageId;

    // Id of network from which item will be retrieved
    int _pageId;

public:

    WirelessNetworkManager();

    // Get list of available networks.
    void scan();

    WirelessNetwork *getAvailableNetworks();

    // Get list of wireless networks by using page id & maximum records per page.
    const WirelessNetwork *getAvailableNetworks(int pageId, int maxRecords, int &length);

    // Get the id of page.
    int getPageId() const;

    // Disconnect to from the connect network (if any)
    static void disconnect() ;

    // Mark page to be loaded.
    void markPageLoaded();

    // Whether network records must be updated.
    bool shouldRecordsRefreshed() const;

    // Go to next network page
    void goForward();

    // Go to previous network page
    void goBackward();

    int getTotalAvailableNetworks();
};

#endif //INDOORWEATHERSTATION_WIRELESSNETWORKMANAGER_H
