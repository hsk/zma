; -----------------------------------------------------------------------------
;	test program
; -----------------------------------------------------------------------------

	ORG			0

	if LABEL1
		defb	1
	else
		defb	2
	endif

LABEL1:

	if !LABEL2
		defb	3
	else
		defb	4
	endif

LABEL2: