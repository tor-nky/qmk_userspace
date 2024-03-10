# SRC += users/naginata_v15m/twpair_on_jis.c
# VPATH += users/naginata_v15m

#BOOTMAGIC_ENABLE = yes		# Enable Bootmagic Lite
#MOUSEKEY_ENABLE = yes		# Mouse keys
#EXTRAKEY_ENABLE = yes		# Audio control and System control
#NKRO_ENABLE = yes			# Enable N-Key Rollover
#RGBLIGHT_ENABLE = yes		# Enable keyboard RGB underglow

RING_BUFFERED_6KRO_REPORT_ENABLE = yes
	# USB 6-Key Rollover - Instead of stopping any new input once 6 keys
	# are pressed, the oldest key is released and the new key is pressed.
DEBOUNCE_TYPE = sym_defer_pk	# debouncing per key.
