/**
 * msg.c
 * Description:
 */

#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msg.h"
#include "TRK_MINNOW_DOLPHIN/Os/dolphin/dolphin_trk_glue.h"
#include "trk.h"

DSError TRKMessageSend(TRKBuffer* msg) {
    DSError err;
    u8 fcs;
    int i;

    fcs = 0;
    for (i = 0; i < msg->length; i++) {
        fcs = fcs + msg->data[i];
    }
    fcs = fcs ^ 0xFF;

    err = WriteUART1(0x7E);
    if (err == DS_NoError) {
        for (i = 0; i < msg->length; i++) {
            u8 c = msg->data[i];
            if (c == 0x7E || c == 0x7D) {
                err = WriteUART1(0x7D);
                c ^= 0x20;
                if (err != DS_NoError) {
                    break;
                }
            }
            err = WriteUART1(c);
            if (err != DS_NoError) {
                break;
            }
        }
    }

    if (err == DS_NoError) {
        u8 c = fcs;
        for (i = 0; i < 1; i++) {
            if (c == 0x7E || c == 0x7D) {
                err = WriteUART1(0x7D);
                c ^= 0x20;
                if (err != DS_NoError) {
                    break;
                }
            }
            err = WriteUART1(c);
            if (err != DS_NoError) {
                break;
            }
        }
    }

    if (err == DS_NoError) {
        err = WriteUART1(0x7E);
    }

    if (err == DS_NoError) {
        err = WriteUARTFlush();
    }

    return err;
}
