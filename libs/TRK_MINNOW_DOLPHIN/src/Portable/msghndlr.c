#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msghndlr.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubevent.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msgbuf.h"
#include "TRK_MINNOW_DOLPHIN/ppc/Generic/targimpl.h"
#include "TRK_MINNOW_DOLPHIN/Os/dolphin/target_options.h"
#include "TRK_MINNOW_DOLPHIN/Os/dolphin/targcont.h"
#include "trk.h"

BOOL IsTRKConnected;

void __TRK_copy_vectors(void);

BOOL GetTRKConnected(void) {
    return IsTRKConnected;
}

void SetTRKConnected(BOOL isTRKConnected) {
    IsTRKConnected = isTRKConnected;
}

static void TRKMessageIntoReply(TRKBuffer* b, MessageCommandID commandId, DSReplyError replyError) {
    TRKResetBuffer(b, 1);
    TRKAppendBuffer1_ui8(b, commandId);
    TRKAppendBuffer1_ui8(b, replyError);
}

static DSError TRKSendACK(TRKBuffer* b) {
    DSError err;
    int tries = 3;

    do {
        err = TRKMessageSend(b);
        --tries;
    } while (err != DS_NoError && tries > 0);

    return err;
}

DSError TRKStandardACK(TRKBuffer* b, MessageCommandID commandId, DSReplyError replyError) {
    TRKMessageIntoReply(b, commandId, replyError);
    return TRKSendACK(b);
}

DSError TRKDoUnsupported(TRKBuffer* b) {
    return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_UnsupportedCommandError);
}

DSError TRKDoConnect(TRKBuffer* b) {
    IsTRKConnected = TRUE;
    return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_NoError);
}

DSError TRKDoDisconnect(TRKBuffer* b) {
    DSError err;
    TRKEvent event;

    IsTRKConnected = FALSE;
    err = TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_NoError);
    if (err == DS_NoError) {
        TRKConstructEvent(&event, NUBEVENT_Shutdown);
        TRKPostEvent(&event);
    }

    return err;
}

DSError TRKDoReset(TRKBuffer* b) {
    TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_NoError);
    __TRK_copy_vectors();
    return DS_NoError;
}

DSError TRKDoVersions(TRKBuffer* b) {
    DSError err;
    DSVersions versions;

    if (b->length != 1) {
        err = TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_PacketSizeError);
    } else {
        TRKMessageIntoReply(b, DSMSG_ReplyACK, DSREPLY_NoError);
        err = TRKTargetVersions(&versions);

        if (err == DS_NoError)
            err = TRKAppendBuffer1_ui8(b, versions.kernelMajor);
        if (err == DS_NoError)
            err = TRKAppendBuffer1_ui8(b, versions.kernelMinor);
        if (err == DS_NoError)
            err = TRKAppendBuffer1_ui8(b, versions.protocolMajor);
        if (err == DS_NoError)
            err = TRKAppendBuffer1_ui8(b, versions.protocolMinor);

        if (err != DS_NoError)
            err = TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_CWDSError);
        else
            err = TRKSendACK(b);
    }

    return err;
}

DSError TRKDoSupportMask(TRKBuffer* b) {
    DSError err;
    u8 mask[32];

    if (b->length != 1) {
        err = TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_PacketSizeError);
    } else {
        TRKMessageIntoReply(b, DSMSG_ReplyACK, DSREPLY_NoError);
        err = TRKTargetSupportMask(mask);

        if (err == DS_NoError)
            err = TRKAppendBuffer(b, mask, sizeof(mask));
        if (err == DS_NoError)
            err = TRKAppendBuffer1_ui8(b, 2);

        if (err != DS_NoError)
            err = TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_CWDSError);
        else
            err = TRKSendACK(b);
    }

    return err;
}

DSError TRKDoCPUType(TRKBuffer* b) {
    DSError err;
    DSCPUType cputype;

    if (b->length != 1) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_PacketSizeError);
    }

    TRKMessageIntoReply(b, DSMSG_ReplyACK, DSREPLY_NoError);
    err = TRKTargetCPUType(&cputype);

    if (err == DS_NoError)
        err = TRKAppendBuffer1_ui8(b, cputype.cpuMajor);
    if (err == DS_NoError)
        err = TRKAppendBuffer1_ui8(b, cputype.cpuMinor);
    if (err == DS_NoError)
        err = TRKAppendBuffer1_ui8(b, cputype.bigEndian);
    if (err == DS_NoError)
        err = TRKAppendBuffer1_ui8(b, cputype.defaultTypeSize);
    if (err == DS_NoError)
        err = TRKAppendBuffer1_ui8(b, cputype.fpTypeSize);
    if (err == DS_NoError)
        err = TRKAppendBuffer1_ui8(b, cputype.extended1TypeSize);
    if (err == DS_NoError)
        err = TRKAppendBuffer1_ui8(b, cputype.extended2TypeSize);

    if (err != DS_NoError)
        err = TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_CWDSError);
    else
        err = TRKSendACK(b);

    return err;
}

DSError TRKDoReadMemory(TRKBuffer* b) {
    DSError err;
    DSReplyError replyErr;
    u8 buf[0x800];
    u32 start;
    u32 length;
    u8 command;
    u8 options;
    u16 msg_length;

    if (b->length != 8) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_PacketSizeError);
    }

    TRKSetBufferPosition(b, 0);
    err = TRKReadBuffer1_ui8(b, &command);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui8(b, &options);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui16(b, &msg_length);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui32(b, &start);

    if (options & 2) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_UnsupportedOptionError);
    }

    if (msg_length > 0x800) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_ParameterError);
    }

    TRKMessageIntoReply(b, DSMSG_ReplyACK, DSREPLY_NoError);

    if (err == DS_NoError) {
        length = msg_length;
        err = TRKTargetAccessMemory(buf, start, &length,
                                    (options & 8) ? MEMACCESS_UserMemory : MEMACCESS_DebuggerMemory, TRUE);
        msg_length = length;
        if (err == DS_NoError)
            err = TRKAppendBuffer1_ui16(b, msg_length);
        if (err == DS_NoError)
            err = TRKAppendBuffer(b, buf, length);
    }

    if (err != DS_NoError) {
        switch (err) {
        case DS_CWDSException:
            replyErr = DSREPLY_CWDSException;
            break;
        case DS_InvalidMemory:
            replyErr = DSREPLY_InvalidMemoryRange;
            break;
        case DS_InvalidProcessID:
            replyErr = DSREPLY_InvalidProcessID;
            break;
        case DS_InvalidThreadID:
            replyErr = DSREPLY_InvalidThreadID;
            break;
        case DS_OSError:
            replyErr = DSREPLY_OSError;
            break;
        default:
            replyErr = DSREPLY_CWDSError;
            break;
        }
        err = TRKStandardACK(b, DSMSG_ReplyACK, replyErr);
    } else {
        err = TRKSendACK(b);
    }

    return err;
}

DSError TRKDoWriteMemory(TRKBuffer* b) {
    DSError err;
    DSReplyError replyErr;
    u8 buf[0x800];
    u32 start;
    u32 length;
    u8 command;
    u8 options;
    u16 msg_length;

    if (b->length <= 8) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_PacketSizeError);
    }

    TRKSetBufferPosition(b, 0);
    err = TRKReadBuffer1_ui8(b, &command);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui8(b, &options);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui16(b, &msg_length);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui32(b, &start);

    if (options & 2) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_UnsupportedOptionError);
    }

    if (b->length != msg_length + 8 || msg_length > 0x800) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_ParameterError);
    }

    if (err == DS_NoError) {
        length = msg_length;
        err = TRKReadBuffer(b, buf, length);
        if (err == DS_NoError) {
            err = TRKTargetAccessMemory(buf, start, &length,
                                        (options & 8) ? MEMACCESS_UserMemory : MEMACCESS_DebuggerMemory, FALSE);
        }
        msg_length = length;
    }

    if (err == DS_NoError) {
        TRKMessageIntoReply(b, DSMSG_ReplyACK, DSREPLY_NoError);
    }

    if (err == DS_NoError) {
        err = TRKAppendBuffer1_ui16(b, msg_length);
    }

    if (err != DS_NoError) {
        switch (err) {
        case DS_CWDSException:
            replyErr = DSREPLY_CWDSException;
            break;
        case DS_InvalidMemory:
            replyErr = DSREPLY_InvalidMemoryRange;
            break;
        case DS_InvalidProcessID:
            replyErr = DSREPLY_InvalidProcessID;
            break;
        case DS_InvalidThreadID:
            replyErr = DSREPLY_InvalidThreadID;
            break;
        case DS_OSError:
            replyErr = DSREPLY_OSError;
            break;
        default:
            replyErr = DSREPLY_CWDSError;
            break;
        }
        err = TRKStandardACK(b, DSMSG_ReplyACK, replyErr);
    } else {
        err = TRKSendACK(b);
    }

    return err;
}

DSError TRKDoReadRegisters(TRKBuffer* b) {
    DSError err;
    DSReplyError replyErr;
    size_t registersLength;
    u16 firstRegister;
    u16 lastRegister;
    u8 command;
    u8 options;

    if (b->length != 6) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_PacketSizeError);
    }

    TRKSetBufferPosition(b, 0);
    err = TRKReadBuffer1_ui8(b, &command);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui8(b, &options);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui16(b, &firstRegister);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui16(b, &lastRegister);

    if (firstRegister > lastRegister) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_InvalidRegisterRange);
    }

    if (err == DS_NoError)
        TRKMessageIntoReply(b, DSMSG_ReplyACK, DSREPLY_NoError);

    switch (options & 7) {
    case 0:
        err = TRKTargetAccessDefault(firstRegister, lastRegister, b, &registersLength, TRUE);
        break;
    case 1:
        err = TRKTargetAccessFP(firstRegister, lastRegister, b, &registersLength, TRUE);
        break;
    case 2:
        err = TRKTargetAccessExtended1(firstRegister, lastRegister, b, &registersLength, TRUE);
        break;
    case 3:
        err = TRKTargetAccessExtended2(firstRegister, lastRegister, b, &registersLength, TRUE);
        break;
    default:
        err = DS_UnsupportedError;
        break;
    }

    if (err != DS_NoError) {
        switch (err) {
        case DS_UnsupportedError:
            replyErr = DSREPLY_UnsupportedOptionError;
            break;
        case DS_InvalidRegister:
            replyErr = DSREPLY_InvalidRegisterRange;
            break;
        case DS_CWDSException:
            replyErr = DSREPLY_CWDSException;
            break;
        case DS_InvalidProcessID:
            replyErr = DSREPLY_InvalidProcessID;
            break;
        case DS_InvalidThreadID:
            replyErr = DSREPLY_InvalidThreadID;
            break;
        case DS_OSError:
            replyErr = DSREPLY_OSError;
            break;
        default:
            replyErr = DSREPLY_CWDSError;
        }
        err = TRKStandardACK(b, DSMSG_ReplyACK, replyErr);
    } else {
        err = TRKSendACK(b);
    }

    return err;
}

DSError TRKDoWriteRegisters(TRKBuffer* b) {
    DSError err;
    DSReplyError replyErr;
    size_t registersLength;
    u16 firstRegister;
    u16 lastRegister;
    u8 command;
    u8 options;

    if (b->length <= 6) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_PacketSizeError);
    }

    TRKSetBufferPosition(b, 0);
    err = TRKReadBuffer1_ui8(b, &command);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui8(b, &options);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui16(b, &firstRegister);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui16(b, &lastRegister);

    if (firstRegister > lastRegister) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_InvalidRegisterRange);
    }

    switch (options) {
    case 0:
        err = TRKTargetAccessDefault(firstRegister, lastRegister, b, &registersLength, FALSE);
        break;
    case 1:
        err = TRKTargetAccessFP(firstRegister, lastRegister, b, &registersLength, FALSE);
        break;
    case 2:
        err = TRKTargetAccessExtended1(firstRegister, lastRegister, b, &registersLength, FALSE);
        break;
    case 3:
        err = TRKTargetAccessExtended2(firstRegister, lastRegister, b, &registersLength, FALSE);
        break;
    default:
        err = DS_UnsupportedError;
        break;
    }

    if (err == DS_NoError)
        TRKMessageIntoReply(b, DSMSG_ReplyACK, DSREPLY_NoError);

    if (err != DS_NoError) {
        switch (err) {
        case DS_UnsupportedError:
            replyErr = DSREPLY_UnsupportedOptionError;
            break;
        case DS_InvalidRegister:
            replyErr = DSREPLY_InvalidRegisterRange;
            break;
        case DS_MessageBufferReadError:
            replyErr = DSREPLY_PacketSizeError;
            break;
        case DS_CWDSException:
            replyErr = DSREPLY_CWDSException;
            break;
        case DS_InvalidProcessID:
            replyErr = DSREPLY_InvalidProcessID;
            break;
        case DS_InvalidThreadID:
            replyErr = DSREPLY_InvalidThreadID;
            break;
        case DS_OSError:
            replyErr = DSREPLY_OSError;
            break;
        default:
            replyErr = DSREPLY_CWDSError;
        }
        err = TRKStandardACK(b, DSMSG_ReplyACK, replyErr);
    } else {
        err = TRKSendACK(b);
    }

    return err;
}

DSError TRKDoFlushCache(TRKBuffer* b) {
    DSError err;
    DSReplyError replyErr;
    u32 start;
    u32 end;
    u8 command;
    u8 options;

    if (b->length != 10) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_PacketSizeError);
    }

    TRKSetBufferPosition(b, 0);
    err = TRKReadBuffer1_ui8(b, &command);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui8(b, &options);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui32(b, &start);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui32(b, &end);

    if (start > end) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_InvalidMemoryRange);
    }

    if (err == DS_NoError)
        err = TRKTargetFlushCache(options, (void*)start, (void*)end);

    if (err == DS_NoError)
        TRKMessageIntoReply(b, DSMSG_ReplyACK, DSREPLY_NoError);

    if (err != DS_NoError) {
        switch (err) {
        case DS_UnsupportedError:
            replyErr = DSREPLY_UnsupportedOptionError;
            break;
        default:
            replyErr = DSREPLY_CWDSError;
            break;
        }
        err = TRKStandardACK(b, DSMSG_ReplyACK, replyErr);
    } else {
        err = TRKSendACK(b);
    }

    return err;
}

DSError TRKDoContinue(TRKBuffer* b) {
    DSError err;

    if (!TRKTargetStopped()) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_NotStopped);
    }

    err = TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_NoError);
    if (err == DS_NoError)
        err = TRKTargetContinue();

    return err;
}

DSError TRKDoStep(TRKBuffer* b) {
    DSError err;
    u32 pc;
    u32 rangeStart;
    u32 rangeEnd;
    u8 command;
    u8 options;
    u8 count;

    if (b->length < 3) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_PacketSizeError);
    }

    TRKSetBufferPosition(b, 0);
    err = TRKReadBuffer1_ui8(b, &command);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui8(b, &options);

    switch (options) {
    case DSSTEP_IntoCount:
    case DSSTEP_OverCount:
        if (err == DS_NoError)
            err = TRKReadBuffer1_ui8(b, &count);
        if (count < 1) {
            return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_ParameterError);
        }
        break;
    case DSSTEP_IntoRange:
    case DSSTEP_OverRange:
        if (b->length != 10) {
            return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_PacketSizeError);
        }

        if (err == DS_NoError)
            err = TRKReadBuffer1_ui32(b, &rangeStart);
        if (err == DS_NoError)
            err = TRKReadBuffer1_ui32(b, &rangeEnd);

        pc = TRKTargetGetPC();
        if (pc < rangeStart || pc > rangeEnd) {
            return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_ParameterError);
        }
        break;
    default:
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_UnsupportedOptionError);
    }

    if (!TRKTargetStopped()) {
        return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_NotStopped);
    }

    err = TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_NoError);
    if (err == DS_NoError) {
        switch (options) {
        case DSSTEP_IntoCount:
        case DSSTEP_OverCount:
            err = TRKTargetSingleStep(count, (options == DSSTEP_OverCount));
            break;
        case DSSTEP_IntoRange:
        case DSSTEP_OverRange:
            err = TRKTargetStepOutOfRange(rangeStart, rangeEnd, (options == DSSTEP_OverRange));
            break;
        }
    }

    return err;
}

DSError TRKDoStop(TRKBuffer* b) {
    DSReplyError replyError;

    switch (TRKTargetStop()) {
    case DS_NoError:
        replyError = DSREPLY_NoError;
        break;
    case DS_InvalidProcessID:
        replyError = DSREPLY_InvalidProcessID;
        break;
    case DS_InvalidThreadID:
        replyError = DSREPLY_InvalidThreadID;
        break;
    case DS_OSError:
        replyError = DSREPLY_OSError;
        break;
    default:
        replyError = DSREPLY_Error;
        break;
    }

    return TRKStandardACK(b, DSMSG_ReplyACK, replyError);
}

DSError TRKDoSetOption(TRKBuffer* b) {
    DSError err;
    u8 command = 0;
    u8 option = 0;
    u8 value = 0;

    TRKSetBufferPosition(b, 0);
    err = TRKReadBuffer1_ui8(b, &command);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui8(b, &option);
    if (err == DS_NoError)
        err = TRKReadBuffer1_ui8(b, &value);

    if (err != DS_NoError) {
        TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_Error);
    } else if (option == 1) {
        SetUseSerialIO(value);
    }

    return TRKStandardACK(b, DSMSG_ReplyACK, DSREPLY_NoError);
}
