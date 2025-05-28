Import("env")
env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.elf",
    "arm-none-eabi-objcopy -O ihex $BUILD_DIR/${PROGNAME}.elf $BUILD_DIR/${PROGNAME}.hex"
)
