; -----------------------------------------------------------------------------
;	test program
; -----------------------------------------------------------------------------

PAGE_ALIGN	macro
				if FILE_ADDRESS % 256
					repeat I, 256 - (FILE_ADDRESS % 256)
						defb	0
					endr
				endif
			endm