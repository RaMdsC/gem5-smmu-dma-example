#include "dev/arm/dummy.hh"
#include "params/Dummy.hh"

Dummy::Dummy(const DummyParams *params)
    : AmbaDmaDevice(params, pioSize),
      dmaDoneEvent([this]{ regBlock.status = 1; }, name()) {
}

AddrRangeList
Dummy::getAddrRanges() const {
    return { RangeSize(pioAddr, pioSize) };
}

Tick
Dummy::read(PacketPtr pkt) {
    const Addr offset { pkt->getAddr() - pioAddr };
    uint64_t resp { 0 };
    switch (offset) {
        case STATUS:
            resp = regBlock.status;
    }
    pkt->setUintX(resp, LittleEndianByteOrder);
    pkt->makeResponse();
    return pioDelay;
}

Tick
Dummy::write(PacketPtr pkt) {
    const Addr offset { pkt->getAddr() - pioAddr };
    const uint64_t data { pkt->getUintX(LittleEndianByteOrder) };
    switch (offset) {
        case BASE:
            regBlock.base = data;
            break;
        case RANGE:
            regBlock.range = data;
            break;
        case CONTROL:
            regBlock.control = data;
            if (!dmaPending())
                dmaWrite(regBlock.base, regBlock.range,
                         &dmaDoneEvent, nullptr);
            break;
        case STATUS:
            regBlock.status = 0;
    }
    pkt->makeResponse();
    return pioDelay;
}

Dummy *
DummyParams::create() {
    return new Dummy(this);
}
