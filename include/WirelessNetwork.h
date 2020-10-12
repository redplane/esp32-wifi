#ifndef INDOORWEATHERSTATION_WIRELESSNETWORK_H
#define INDOORWEATHERSTATION_WIRELESSNETWORK_H


class WirelessNetwork {
private:

    // Name of network.
    char name[128];

    // Signal strength.
    char signalStrength;

    // Whether wifi is protected or not.
    bool isProtected;

public:

    WirelessNetwork();

    // Constructor
    WirelessNetwork(const char *name, char signalStrength, bool isProtected);

    WirelessNetwork(WirelessNetwork const &wirelessNetwork);

    // Get network name.
    const char *getName();

    // Get network strength.
    char getStrength() const;

    // Whether wifi is protected or not.
    bool getProtected() const;
};

#endif