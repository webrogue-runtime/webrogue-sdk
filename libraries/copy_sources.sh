cd $(dirname $0)

TARGET_FILES=$(cat copied_sources.txt)

for TARGET_FILE in $TARGET_FILES; do
    mkdir -p "$(dirname webrogue_gfx/$TARGET_FILE)"
    cmp ../../external/$TARGET_FILE webrogue_gfx/$TARGET_FILE || cp ../../external/$TARGET_FILE webrogue_gfx/$TARGET_FILE || exit 1
done
