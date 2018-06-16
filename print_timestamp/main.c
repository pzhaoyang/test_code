#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "timestamp.h"

typedef struct CCT_HANDLE_WIDE_ {
    unsigned char       slot;
    unsigned char       port;
    unsigned short      channel;
    unsigned int        circuit_id;
} CCT_HANDLE_WIDE;

#define CCT_PSEUDO_SLOT                         255
#define CCT_PSEUDO_PORT_PW                      25
#define CCT_PSEUDO_PORT_ACCT_LS_ETHER           6
#define CCT_PSEUDO_PORT_ACCT_LS_MP              7
#define CCT_PSEUDO_PORT_ACCT_LS_MFR             8
#define CCT_PSEUDO_PORT_ACCT_LS_HDLC            9
#define CCT_PSEUDO_PORT_ACCT_LS_PPP             10
#define CCT_PSEUDO_PORT_ACCT_LS_DOT1Q           11
#define CCT_PSEUDO_PORT_ACCESS                  22
#define CCT_PSEUDO_PORT_LAG                     36

#define CCT_ID_CHAN_LVL3_MASK                   0x7C00
#define CCT_ID_CHAN_LVL2_MASK                   0x7FC0
#define CCT_ID_CHAN_FMT_MASK                    0x8000

#define CCT_ID_SUBCHAN_LVL3_MASK                0x03E0
#define CCT_ID_SUBCHAN_LVL2_MASK                0x003F

#define CCT_ID_SUBSUBCHAN_LVL3_MASK             0x001F

#define CCT_ID_CHAN_LVL2_LSB                    6
#define CCT_ID_CHAN_LVL3_LSB                    10

#define CCT_ID_SUBCHAN_LVL3_LSB                 5
#define CCT_ID_SUBCHAN_LVL2_LSB                 0

#define CCT_ID_AUTH_MASK                        0xF0000000
#define CCT_ID_LEVEL_MASK                       0x0C000000
#define CCT_ID_INDEX_MASK                       0x00ffffff

#define CCT_ID_AUTH_LSB                         28
#define CCT_ID_LEVEL_LSB                        26

#define CCT_MAX_CHAN_LVL2                       (CCT_ID_CHAN_LVL2_MASK >> CCT_ID_CHAN_LVL2_LSB)
#define CCT_CHAN_ID_INVALID_VAL                 CCT_MAX_CHAN_LVL2

#define CCT_MAX_SUBCHAN_LVL2                    CCT_ID_SUBCHAN_LVL2_MASK
#define CCT_SUBCHAN_ID_INVALID_VAL              CCT_MAX_SUBCHAN_LVL2

#define CCT_MAX_SUBSUBCHAN_LVL3                 CCT_ID_SUBSUBCHAN_LVL3_MASK
#define CCT_SUBSUBCHAN_ID_INVALID_VAL           CCT_MAX_SUBSUBCHAN_LVL3

#define CCT_GET_SLOT(cct_handle) ((cct_handle).slot)
#define CCT_SET_SLOT(cct_handle, slotnum) ((cct_handle).slot = (unsigned char)(slotnum))

#define CCT_SET_PORT(cct_handle, portnum) ((cct_handle).port = (unsigned char)(portnum))
#define CCT_GET_PORT(cct_handle) ((cct_handle).port)

#define CCT_SET_CHANNEL(cct_handle, chnl) ((cct_handle).channel = (unsigned char)(chnl))
#define CCT_GET_CHANNEL(cct_handle) ((cct_handle).channel)

#define _BIT_MATCH(x,mask) ((x & mask) == mask)
#define _BIT_VALUE_GET(x, mask, shift) ((x & mask) >> shift)
#define _BIT_VALUE_SET(x, v, mask, shift) ((x & ~mask) | ((v << shift) & mask))

#define CHANNEL_IS_3LEVEL(channel_id) ((channel_id) & CCT_ID_CHAN_FMT_MASK)

#define CHANNEL_GET_CHAN_ID(channel_id) \
            (CHANNEL_IS_3LEVEL(channel_id) ? \
                (_BIT_MATCH(channel_id,CCT_ID_CHAN_LVL3_MASK) ? CCT_CHAN_ID_INVALID_VAL:_BIT_VALUE_GET(channel_id, CCT_ID_CHAN_LVL3_MASK, CCT_ID_CHAN_LVL3_LSB)): \
                (_BIT_MATCH(channel_id,CCT_ID_CHAN_LVL2_MASK) ? CCT_CHAN_ID_INVALID_VAL:_BIT_VALUE_GET(channel_id, CCT_ID_CHAN_LVL2_MASK, CCT_ID_CHAN_LVL2_LSB)))

#define CHANNEL_GET_SUBCHAN_ID(channel_id) \
            (CHANNEL_IS_3LEVEL(channel_id) ? \
                    (_BIT_MATCH(channel_id,CCT_ID_SUBCHAN_LVL3_MASK) ? CCT_SUBCHAN_ID_INVALID_VAL:_BIT_VALUE_GET(channel_id, CCT_ID_SUBCHAN_LVL3_MASK, CCT_ID_SUBCHAN_LVL3_LSB)): \
                    (_BIT_MATCH(channel_id,CCT_ID_SUBCHAN_LVL2_MASK) ? CCT_SUBCHAN_ID_INVALID_VAL:_BIT_VALUE_GET(channel_id, CCT_ID_SUBCHAN_LVL2_MASK, CCT_ID_SUBCHAN_LVL2_LSB)))

#define CHANNEL_GET_SUBSUBCHAN_ID(channel_id) \
            (CHANNEL_IS_3LEVEL(channel_id) ? \
                (_BIT_MATCH(channel_id,CCT_ID_SUBSUBCHAN_LVL3_MASK) ? CCT_SUBSUBCHAN_ID_INVALID_VAL:_BIT_VALUE_GET(channel_id, CCT_ID_SUBSUBCHAN_LVL3_MASK, 0)): \
                CCT_SUBSUBCHAN_ID_INVALID_VAL)

#define CCT_ID_GET_AUTH(cct_handle) (((cct_handle).circuit_id & CCT_ID_AUTH_MASK) >> CCT_ID_AUTH_LSB)
#define CCT_ID_GET_LEVEL(cct_handle) (((cct_handle).circuit_id & CCT_ID_LEVEL_MASK) >> CCT_ID_LEVEL_LSB)
#define CCT_ID_GET_INDEX(cct_handle) ((cct_handle).circuit_id & CCT_ID_INDEX_MASK)
#define CCT_GEN_CIRCUIT_ID(id_auth, id_level, id_index) ((((id_auth) & 0x0F) << CCT_ID_AUTH_LSB) | (((id_level) & 0x03) << CCT_ID_LEVEL_LSB) | ((id_index) & CCT_ID_INDEX_MASK))
#define CCT_SET_ID(cct_handle, circuit)  ((cct_handle).circuit_id = (unsigned int)(circuit))



#define CCT_IS_3LEVEL(cct_handle) CHANNEL_IS_3LEVEL((cct_handle).channel)
#define SLOT_PORT_IS_ACCT_LS(slot, port) ((slot == CCT_PSEUDO_SLOT) && ((port == CCT_PSEUDO_PORT_ACCT_LS_ETHER) || (port == CCT_PSEUDO_PORT_ACCT_LS_MP) || \
    (port == CCT_PSEUDO_PORT_ACCT_LS_MFR) || (port == CCT_PSEUDO_PORT_ACCT_LS_HDLC) || (port == CCT_PSEUDO_PORT_ACCT_LS_DOT1Q) || (port == CCT_PSEUDO_PORT_ACCESS) || \
    (port == CCT_PSEUDO_PORT_LAG) || (port == CCT_PSEUDO_PORT_ACCT_LS_PPP)))

#define SLOT_PORT_IS_PW(slot, port) ((slot == CCT_PSEUDO_SLOT) && (port == CCT_PSEUDO_PORT_PW))
#define SLOT_PORT_IS_ACCT_LS_OR_PW(slot, port) (SLOT_PORT_IS_PW(slot, port) || SLOT_PORT_IS_ACCT_LS(slot, port))


/* Generate a 2-level encoded channel value */
unsigned short CHANNEL_GEN_2_LEVEL(unsigned short chan, unsigned short subchan){
    unsigned short channel_id = 0;

    if(chan == 0xFFFF || chan == 0x1FF) {
        channel_id = 0xFFFF; //no channel
    } else {
        channel_id = _BIT_VALUE_SET(channel_id, chan, 0x7FC0, 6);
        channel_id = _BIT_VALUE_SET(channel_id, subchan, 0x003F, 0);
    }

    return channel_id;
}

int cct_handle_is_invalid (CCT_HANDLE_WIDE cct_handle){
    unsigned int *ptr;

    ptr = (unsigned int *)&cct_handle;

    if(((ptr[0] == 0) && (ptr[1] == 0)) || ((ptr[0] == 0xffffffff) && (ptr[1] == 0xffffffff))){
        return 1;
    }

    return 0;
}

void print_cct_handle (CCT_HANDLE_WIDE cct_handle){
    unsigned char    slot, port;
    unsigned short   channel, chan, subchan, subsubchan;
    unsigned int   index, auth, level;

    /* check if cct_handle is invalid */
    if (cct_handle_is_invalid(cct_handle)) {
        ANTS_ERROR("cct invalid");
        return;
    }

    slot = CCT_GET_SLOT(cct_handle);
    port = CCT_GET_PORT(cct_handle);
    channel = CCT_GET_CHANNEL(cct_handle);
    chan = CHANNEL_GET_CHAN_ID(channel);
    subchan = CHANNEL_GET_SUBCHAN_ID(channel);
    subsubchan = CHANNEL_GET_SUBSUBCHAN_ID(channel);

    auth = CCT_ID_GET_AUTH(cct_handle);
    level = CCT_ID_GET_LEVEL(cct_handle);
    index = CCT_ID_GET_INDEX(cct_handle);

    /* for pseudo slots, do not increment slot/port */
    if(slot != CCT_PSEUDO_SLOT) {
        slot += 1;

        if (port != 0xFF) {
            port += 1;
        }
    }

    if(chan != CCT_CHAN_ID_INVALID_VAL) {
        chan += 1;
    }

    /* EV-183635: allow 64th subchan or 255th subsubchan for Pseudowire and LG (except APS) CCTs */
    if ((subchan != CCT_SUBCHAN_ID_INVALID_VAL) || SLOT_PORT_IS_ACCT_LS_OR_PW(slot, port)) {
        subchan += 1;
    }

    if ((subsubchan != CCT_SUBSUBCHAN_ID_INVALID_VAL) || SLOT_PORT_IS_ACCT_LS_OR_PW(slot, port)) {
        subsubchan += 1;
    }

    /* normal cct display or pseudo port aps cct display */
    if (CCT_IS_3LEVEL(cct_handle)){
        ANTS_INFO("\n");
        ANTS_INFO("CCT Handle ==> %u/%u:%u:%u:%u/%u/%u/%u", slot, port, chan, subchan, subsubchan, auth, level, index);
    }else{
        ANTS_INFO("\n");
        ANTS_INFO("CCT Handle ==> %u/%u:%u:%u/%u/%u/%u", slot, port, chan, subchan, auth, level, index);
    }

    return;
}

#define SLASH '/'
#define COLON ':'

// TODO: 255/36:1:2/1/1/2
void cct_string_to_cct_handle(char *cct ){
    CCT_HANDLE_WIDE cct_handle;
    char *p, *n_p;
    int slot, port, chan, subchan, auth, level, idx;
    char cct_arry[256];

    memcpy(cct_arry, cct, strlen(cct));
    p = n_p = cct_arry;
    slot = port = chan = subchan = auth = level = idx = 0;

    n_p = index(p, SLASH);
    if (n_p) {
        *n_p = 0;
        slot = (atoi(p) - 1);
        p = ++n_p;
    }
    n_p = index(p, COLON);
    if (n_p) {
        *n_p = 0;
        port = (atoi(p) -1);
        p = ++n_p;
    }
    n_p = index(p, COLON);
    if (n_p) {
        *n_p = 0;
        chan = (atoi(p) -1);
        p = ++n_p;
    }
    n_p = index(p, SLASH);
    if (n_p) {
        *n_p = 0;
        subchan = atoi(p);
        p = ++n_p;
    }
    n_p = index(p, SLASH);
    if (n_p) {
        *n_p = 0;
        auth = atoi(p);
        p = ++n_p;
    }
    n_p = index(p, SLASH);
    if (n_p) {
        *n_p = 0;
        level = atoi(p);
        p = ++n_p;
    }
    idx = atoi(p);
    CCT_SET_SLOT(cct_handle, slot);
    CCT_SET_PORT(cct_handle, port);

    CCT_SET_CHANNEL(cct_handle, CHANNEL_GEN_2_LEVEL(chan, subchan));
    CCT_SET_ID(cct_handle, CCT_GEN_CIRCUIT_ID(auth, level, idx));

    print_cct_handle(cct_handle);
}


void main(){
    ANTS_MEMDUMP("hello222333444555666", 21, "show");
    cct_string_to_cct_handle("255/36:1:2/1/1/2");
}
