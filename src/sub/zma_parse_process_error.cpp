// --------------------------------------------------------------------
//	Z80 Macro Assembler parse
// ====================================================================
//	2019/05/04	t.hara
// --------------------------------------------------------------------

#include "zma_parse.hpp"
#include "zma_text.hpp"
#include "zma_parse_process.hpp"
#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "zma_parse_process_error.hpp"

// --------------------------------------------------------------------
bool CZMA_PARSE_ERROR::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {

	set_code_size( &info, 0 );
	update_flags( &info, p_last_line );
	put_error( CZMA_ERROR::get( CZMA_ERROR_CODE::INVALID_COMMAND ) );
	return false;
}
