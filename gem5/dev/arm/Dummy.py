from m5.objects.RealView import AmbaDmaDevice, VExpress_GEM5_V2_Base

class Dummy(AmbaDmaDevice):
    type = "Dummy"
    cxx_header = "dev/arm/dummy.hh"

    def generateDeviceTree(self, state):
        node = self.generateBasicPioDeviceNode(
                state, "dummy", self.pio_addr, 0x12, [ int(self.int_num) ])
        node.appendCompatible("arm,dummy")
        self.addIommuProperty(state, node)
        yield node

class DummyPlatform(VExpress_GEM5_V2_Base):
    dummy = Dummy(pio_addr=0x2e000000, int_num=0x32,
                  amba_id=0xbeef, sid=0xa, ssid=0xa)

    def attachOnChipIO(self, bus, bridge=None, dma_ports=None, mem_ports=None):
        super(DummyPlatform, self).attachOnChipIO(bus, bridge, dma_ports,
                                                  mem_ports)
        self.attachSmmu([self.dummy], bus)
