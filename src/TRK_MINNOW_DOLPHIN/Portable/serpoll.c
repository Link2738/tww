#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/serpoll.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubevent.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msgbuf.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msghndlr.h"
#include "trk.h"

TRKFramingState gTRKFramingState;

void* gTRKInputPendingPtr;

void TRKProcessInput(int bufferIdx);

static inline void TRKDiscardFrame(void) {
    if (gTRKFramingState.msgBufID != -1) {
        TRKReleaseBuffer(gTRKFramingState.msgBufID);
        gTRKFramingState.msgBufID = -1;
    }

    gTRKFramingState.buffer = NULL;
    gTRKFramingState.receiveState = DSRECV_Wait;
}

static inline BOOL TRKFrameDone(void) {
    TRKBuffer* b = gTRKFramingState.buffer;

    if (b->length < 2) {
        TRKStandardACK(b, DSMSG_ReplyNAK, DSREPLY_PacketSizeError);
        TRKDiscardFrame();
        return FALSE;
    }

    b->position = 0;
    b->length--;
    return TRUE;
}

MessageBufferID TRKTestForPacket(void) {
    DSError err;
    UARTError readErr;
    char c;

    err = DS_NoError;
    readErr = TRKReadUARTPoll(&c);

    while (readErr == UART_NoError && err == DS_NoError) {
        if (gTRKFramingState.receiveState != DSRECV_InFrame) {
            gTRKFramingState.isEscape = FALSE;
        }

        switch (gTRKFramingState.receiveState) {
        case DSRECV_Wait:
            if (c == 0x7E) {
                err = TRKGetFreeBuffer(&gTRKFramingState.msgBufID, &gTRKFramingState.buffer);
                gTRKFramingState.fcsType = 0;
                gTRKFramingState.receiveState = DSRECV_Found;
            }
            break;
        case DSRECV_Found:
            if (c == 0x7E) {
                break;
            }
            gTRKFramingState.receiveState = DSRECV_InFrame;
            // fallthrough
        case DSRECV_InFrame:
            if (c == 0x7E) {
                if (gTRKFramingState.isEscape) {
                    TRKStandardACK(gTRKFramingState.buffer, DSMSG_ReplyNAK, DSREPLY_EscapeError);
                    TRKDiscardFrame();
                    break;
                }

                if (TRKFrameDone()) {
                    MessageBufferID id = gTRKFramingState.msgBufID;
                    gTRKFramingState.msgBufID = -1;
                    gTRKFramingState.buffer = NULL;
                    gTRKFramingState.receiveState = DSRECV_Wait;
                    return id;
                }

                gTRKFramingState.receiveState = DSRECV_Wait;
            } else {
                if (gTRKFramingState.isEscape) {
                    c ^= 0x20;
                    gTRKFramingState.isEscape = FALSE;
                } else if (c == 0x7D) {
                    gTRKFramingState.isEscape = TRUE;
                    break;
                }

                err = TRKAppendBuffer1_ui8(gTRKFramingState.buffer, c);
                gTRKFramingState.fcsType += c;
            }
            break;
        case DSRECV_FrameOverflow:
            if (c == 0x7E) {
                TRKDiscardFrame();
            }
            break;
        }

        readErr = TRKReadUARTPoll(&c);
    }

    return -1;
}

void TRKGetInput(void) {
    TRKBuffer* msgBuffer;
    MessageBufferID id;
    u8 command;

    id = TRKTestForPacket();
    if (id == -1) {
        return;
    }

    msgBuffer = TRKGetBuffer(id);
    TRKSetBufferPosition(msgBuffer, 0);
    TRKReadBuffer1_ui8(msgBuffer, &command);

    if (command < DSMSG_ReplyACK) {
        TRKProcessInput(id);
    } else {
        TRKReleaseBuffer(id);
    }
}

void TRKProcessInput(int bufferIdx) {
    TRKEvent event;

    TRKConstructEvent(&event, NUBEVENT_Request);
    event.msgBufID = bufferIdx;
    gTRKFramingState.msgBufID = -1;
    TRKPostEvent(&event);
}

DSError TRKInitializeSerialHandler() {
    gTRKFramingState.msgBufID = -1;
    gTRKFramingState.receiveState = DSRECV_Wait;
    gTRKFramingState.isEscape = FALSE;

    return DS_NoError;
}

DSError TRKTerminateSerialHandler(void) {
    return DS_NoError;
}
