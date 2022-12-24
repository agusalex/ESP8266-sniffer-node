#include "sdk_structs.h"
#include "ieee80211_structs.h"

void mac2str(const uint8_t *ptr, char *string) {
#ifdef MASKED
    sprintf(string, "XX:XX:XX:%02x:%02x:XX", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
#else
    sprintf(string, "%02x:%02x:%02x:%02x:%02x:%02x", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
#endif
    return;
}

wifi_promiscuous_pkt_type_t packet_type_parser(uint16_t len) {
    switch (len) {
        // If only rx_ctrl is returned, this is an unsupported packet
        case sizeof(wifi_pkt_rx_ctrl_t):
            return WIFI_PKT_MISC;

            // Management packet
        case sizeof(wifi_pkt_mgmt_t):
            return WIFI_PKT_MGMT;

            // Data packet
        default:
            return WIFI_PKT_DATA;
    }
}

const char *wifi_pkt_type2str(wifi_promiscuous_pkt_type_t type, wifi_mgmt_subtypes_t subtype) {
    switch (type) {
        case WIFI_PKT_MGMT:
            switch (subtype) {
                case ASSOCIATION_REQ:
                    return "Mgmt: Association request";
                case ASSOCIATION_RES:
                    return "Mgmt: Association response";
                case REASSOCIATION_REQ:
                    return "Mgmt: Reassociation request";
                case REASSOCIATION_RES:
                    return "Mgmt: Reassociation response";
                case PROBE_REQ:
                    return "Mgmt: Probe request";
                case PROBE_RES:
                    return "Mgmt: Probe response";
                case BEACON:
                    return "Mgmt: Beacon frame";
                case ATIM:
                    return "Mgmt: ATIM";
                case DISASSOCIATION:
                    return "Mgmt: Dissasociation";
                case AUTHENTICATION:
                    return "Mgmt: Authentication";
                case DEAUTHENTICATION:
                    return "Mgmt: Deauthentication";
                case ACTION:
                    return "Mgmt: Action";
                case ACTION_NACK:
                    return "Mgmt: Action no ack";
                default:
                    return "Mgmt: Unsupported/error";
            }

        case WIFI_PKT_CTRL:
            return "Control";

        case WIFI_PKT_DATA:
            return "Data";

        default:
            return "Unsupported/error";
    }
}

