#include "WirelessNetwork.h"
#include <cstring>

//#region Constructor

WirelessNetwork::WirelessNetwork() {

}

WirelessNetwork::WirelessNetwork(const char *name, char signalStrength, bool bIsProtected) {
    strcpy(this->name, name);
    this->signalStrength = signalStrength;
    this->isProtected = bIsProtected;
}

WirelessNetwork::WirelessNetwork(WirelessNetwork const &wirelessNetwork) {
    strcpy(this->name, wirelessNetwork.name);
    this->signalStrength = wirelessNetwork.signalStrength;
    this->isProtected = wirelessNetwork.isProtected;
}

//#endregion

//#region Methods

const char *WirelessNetwork::getName() {
    return this->name;
}

char WirelessNetwork::getStrength() const {
    return this->signalStrength;
}

bool WirelessNetwork::getProtected() const {
    return this->isProtected;
}

//#endregion
