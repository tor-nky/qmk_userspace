#BOOTMAGIC_ENABLE = yes	# Enable Bootmagic Lite				# 56 bytes (v0.25.9)
#MOUSEKEY_ENABLE = yes	# Mouse keys						# 1292 bytes (v0.25.9)
#EXTRAKEY_ENABLE = yes	# Audio control and System control	# 564 bytes (v0.25.9)
NKRO_ENABLE = no		# Enable N-Key Rollover				# 342 bytes (v0.25.9)
RGBLIGHT_ENABLE = no	# Enable WS2812 RGB underlight.		# 2410 bytes (v0.25.9 & Custom Effects)
#RGB_MATRIX_ENABLE = no										# 2504 bytes (v0.25.9 & Custom Effects)
#OLED_ENABLE		= yes									# 3782 bytes (v0.25.9)
OLED_DRIVER		= ssd1306
# OS_DETECTION_ENABLE = yes									# 466 bytes (v0.25.9)


# RING_BUFFERED_6KRO_REPORT_ENABLE = yes					# 406 bytes (v0.25.9)
	# USB 6-Key Rollover - Instead of stopping any new input once 6 keys
	# are pressed, the oldest key is released and the new key is pressed.
DEBOUNCE_TYPE = sym_defer_pk	# debouncing per key.		# 882 bytes (v0.25.9)


#  Squeezing the most out of AVR
#	(from qmk_firmware/docs/squeezing_avr.md)
LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
# MOUSEKEY_ENABLE = no
# EXTRAKEY_ENABLE = no
SPACE_CADET_ENABLE = no			# 270 bytes (v0.25.9)
GRAVE_ESC_ENABLE = no			# 104 bytes (v0.25.9)
MAGIC_ENABLE = no				# 464 bytes (v0.25.9)
AVR_USE_MINIMAL_PRINTF = yes
MUSIC_ENABLE = no
