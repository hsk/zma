; -----------------------------------------------------------------------------
;	test program
; -----------------------------------------------------------------------------

	ORG			0

	defb		1

	if FILE_ADDRESS % 16384
		defs	" " * ( 16384 - (FILE_ADDRESS % 16384) )
	endif