#include "dev/arm/amba_device.hh"

class DummyParams;
class Dummy : public AmbaDmaDevice {
    public:
        Dummy(const DummyParams *params);
    protected:
        AddrRangeList getAddrRanges() const override;
        Tick read(PacketPtr pkt) override;
        Tick write(PacketPtr pkt) override;
    private:
        struct RegisterBlock {
            Addr base;
            Addr range;
            uint8_t control;
            uint8_t status;
        } regBlock;
        enum Offset : Addr {
            BASE    = 0x00,
            RANGE   = 0x08,
            CONTROL = 0x10,
            STATUS  = 0x11
        };
        static constexpr Addr pioSize { 0x12 };
        EventFunctionWrapper dmaDoneEvent;
};
