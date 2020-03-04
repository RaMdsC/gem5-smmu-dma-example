from argparse import ArgumentParser
from m5.util import addToPath
addToPath("../../../gem5/configs/")
addToPath("../../../gem5/configs/example/arm/")
from fs_bigLITTLE import addOptions, build, instantiate, run

def main():
    parser = ArgumentParser()
    addOptions(parser)
    options = parser.parse_args()
    root = build(options)
    instantiate(options)
    run()

if __name__ == "__m5_main__":
    main()
