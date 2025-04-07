#!/bin/bash

VERSION=$(sed -e 's/^.*_VERSION_ \([0-9]\+\.[0-9]\+\.[0-9]\+\)/\1/' include/version.h)

if [ -z "$VERSION" ]; then
    echo "Error: __VERSION__ not found in include/version.h"
    exit 1
fi

MAJOR_VERSION=$(echo "$VERSION" | cut -d. -f1)
MINOR_VERSION=$(echo "$VERSION" | cut -d. -f2)
PATCH_VERSION=$(echo "$VERSION" | cut -d. -f3)

NEW_PATCH_VERSION=$(echo "$PATCH_VERSION + 1" | bc -lq)

NEW_VERSION="$MAJOR_VERSION.$MINOR_VERSION.$NEW_PATCH_VERSION"

echo "#define _VERSION_ $NEW_VERSION" > include/version.h
