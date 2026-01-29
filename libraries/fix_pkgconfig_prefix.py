import re
import sys

pc_paths = sys.argv[1:]

for pc_path in pc_paths:
    with open(pc_path, "r") as file:
        original_pc = file.read()
    pc = original_pc

    pc = re.sub(r'prefix=/opt/[A-Za-z0-9\-]+', 'prefix=${pcfiledir}/../..', pc)
    pc = re.sub(r'prefix=//opt/[A-Za-z0-9\-]+', 'prefix=${pcfiledir}/../..', pc)
    pc = re.sub(r'/opt/[A-Za-z0-9\-]+/lib', '${pcfiledir}/../../lib', pc)
    pc = re.sub(r'/opt/[A-Za-z0-9\-]+/include', '${pcfiledir}/../../include', pc)
    pc = pc.replace('prefix=/usr/local', 'prefix=${pcfiledir}/../..')
    pc = pc.replace('/usr/local', '${prefix}')

    if pc != original_pc:
        with open(pc_path, "w") as file:
            pc = file.write(pc)
