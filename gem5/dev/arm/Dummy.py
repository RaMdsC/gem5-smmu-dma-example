from m5.objects.RealView import AmbaDmaDevice

class Dummy(AmbaDmaDevice):
    type = "Dummy"
    cxx_header = "dev/arm/dummy.hh"

    def generateDeviceTree(self, state):
        node = self.generateBasicPioDeviceNode(
                state, "dummy", self.pio_addr, 0x12, [ int(self.int_num) ])
        node.appendCompatible("arm,dummy")
        self.addIommuProperty(state, node)
        yield node
