; -----------------------------------------------------------------------------
;	test program
; -----------------------------------------------------------------------------

	ld		a, 1
	call	0x180
	ld		a, 2
	call	0x180
	ld		a, 3
	call	0x180
	ld		a, 1
	call	0x180
	ld		a, 2
	call	0x180
	ld		a, 3
	call	0x180
	ret
