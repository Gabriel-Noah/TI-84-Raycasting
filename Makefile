# ----------------------------
# Makefile Options
# ----------------------------

NAME = CRAYCAST
ICON = doom.png
DESCRIPTION = "RAYCASTING PROGRAM"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
