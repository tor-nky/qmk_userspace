BOOTMAGIC_ENABLE = no	# Enable Bootmagic Lite				# 74 bytes (v0.23.4)
MOUSEKEY_ENABLE = yes	# Mouse keys						# 1312 bytes (v0.23.4)
EXTRAKEY_ENABLE = yes	# Audio control and System control	# 488 bytes (v0.23.4)
NKRO_ENABLE = yes		# Enable N-Key Rollover				# 346 bytes (v0.23.4)
RGBLIGHT_ENABLE = yes	# Enable WS2812 RGB underlight.		# 2500 bytes (v0.23.4 & Custom Effects)
RGB_MATRIX_ENABLE = no										# 2784 bytes (v0.23.4 & Custom Effects)
OS_DETECTION_ENABLE = yes									# 264 bytes (v0.23.4)


RING_BUFFERED_6KRO_REPORT_ENABLE = yes					# 398 bytes (v0.23.4)
	# USB 6-Key Rollover - Instead of stopping any new input once 6 keys
	# are pressed, the oldest key is released and the new key is pressed.
DEBOUNCE_TYPE = sym_defer_pk	# debouncing per key.		# 922 bytes (v0.23.4)


#  Squeezing the most out of AVR
#	(from qmk_firmware/docs/squeezing_avr.md)
LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
# MOUSEKEY_ENABLE = no
# EXTRAKEY_ENABLE = no
SPACE_CADET_ENABLE = no			# 310 bytes (v0.23.6)
GRAVE_ESC_ENABLE = no			# 72 bytes (v0.23.6)
MAGIC_ENABLE = no				# 474 bytes (v0.23.6)
AVR_USE_MINIMAL_PRINTF = yes
MUSIC_ENABLE = no
