cd $(dirname $0)

TARGET_FILES=$(cat copied_sources.txt)

for TARGET_FILE in $TARGET_FILES; do
    mkdir -p "$(dirname webroguegfx/$TARGET_FILE)"
    cmp ../../external/$TARGET_FILE webroguegfx/$TARGET_FILE || cp ../../external/$TARGET_FILE webroguegfx/$TARGET_FILE || exit 1
done
