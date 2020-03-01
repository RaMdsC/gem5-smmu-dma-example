from argparse import ArgumentParser
from m5.objects.Dummy import Dummy
from m5.util import addToPath
addToPath("../../../gem5/configs/")
addToPath("../../../gem5/configs/example/arm/")
from fs_bigLITTLE import addOptions, build, instantiate, run

def main():
    parser = ArgumentParser()
    addOptions(parser)
    options = parser.parse_args()
    root = build(options)
    root.system.realview.dummy = Dummy(pio_addr=0x2e000000, int_num=0x32,
                                       amba_id=0xbeef, sid=0xa, ssid=0xa)
    root.system.realview.attachSmmu([root.system.realview.dummy],
                                    root.system.membus)
    instantiate(options)
    run()

if __name__ == "__m5_main__":
    main()
