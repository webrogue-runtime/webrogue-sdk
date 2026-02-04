import os
import shutil
import filecmp
import sys
import re

if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} <cached_dist> <dist>")
    sys.exit(1)

cached_dist = sys.argv[1]
dist = sys.argv[2]

if dist == cached_dist:
    quit()

def patch_pc(pc_path):
    with open(pc_path, "r") as file:
        original_pc = file.read()
    pc = original_pc

    pc = re.sub(r'prefix=/opt/[A-Za-z0-9\-]+/cached', 'prefix=${pcfiledir}/../..', pc)
    pc = re.sub(r'prefix=//opt/[A-Za-z0-9\-]+/cached', 'prefix=${pcfiledir}/../..', pc)
    pc = re.sub(r'/opt/[A-Za-z0-9\-]+/cached/lib', '${pcfiledir}/../../lib', pc)
    pc = re.sub(r'/opt/[A-Za-z0-9\-]+/cached/include', '${pcfiledir}/../../include', pc)
    pc = pc.replace('prefix=/usr/local', 'prefix=${pcfiledir}/../..')
    pc = pc.replace('/usr/local', '${prefix}')
    pc = pc.replace('/opt/libjpeg-turbo', '${pcfiledir}/../..')
    

    if pc != original_pc:
        with open(pc_path, "w") as file:
            pc = file.write(pc)

if not os.path.exists(cached_dist):
    quit()

for root, dirs, files in os.walk(cached_dist):
    rel_path = os.path.relpath(root, cached_dist)
    if rel_path == ".":
        target_dir = dist
    else:
        target_dir = os.path.join(dist, rel_path)
    
    if not os.path.exists(target_dir):
        os.makedirs(target_dir)
    
    for file in files:
        src_file = os.path.join(root, file)
        if str(src_file)[-3:] == ".pc":
            patch_pc(src_file)
        dst_file = os.path.join(target_dir, file)
        
        if not os.path.exists(dst_file) or not filecmp.cmp(src_file, dst_file, shallow=False):
            shutil.copy2(src_file, dst_file)

for root, dirs, files in os.walk(cached_dist, topdown=False):
    rel_path = os.path.relpath(root, cached_dist)
    if rel_path == ".":
        target_dir = cached_dist
    else:
        target_dir = os.path.join(cached_dist, rel_path)
    for file in files:
        os.remove(os.path.join(root, file))
    for dir in dirs:
        try:
            os.removedirs(os.path.join(root, dir))
        except FileNotFoundError:
            pass
