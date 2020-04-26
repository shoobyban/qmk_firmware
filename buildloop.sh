#!/bin/bash



make $1 && while true;
do
    if ls /dev/cu.* | grep -v Bluet | grep -v Sams | grep -v Monolith ; then
        export APRT=`ls /dev/cu.* | grep -v Bluet | grep -v Sams | grep -v Monolith`
        leonardoUploader/leonardoUploader $APRT && \
        /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude \
            -C/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf \
            -pm32u4 -cavr109 -P $APRT -b115200 -v -D \
            -Uflash:w:$1:i
        echo "press enter"
        read a
    fi
done
