; -----------------------------------------------------------------------------
;	test program
; -----------------------------------------------------------------------------

hoge macro
	include "sub.asm"
	endm

	hoge
	hoge


	include	"sub_macro1.asm"
	moge
