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

#include "zma_parse_process_global_symbol.hpp"

// --------------------------------------------------------------------
bool CZMA_PARSE_GLOBAL_SYMBOL::process( CZMA_INFORMATION& info, CZMA_PARSE* p_last_line ) {
	std::string label;
	CVALUE v;

	this->set_code_size( &info, 0 );
	update_flags( &info, p_last_line );
	//	log
	label = words[0];
	if( !this->is_analyze_phase ) {
		std::stringstream s;
		log.write_line_infomation( this->line_no, -1, -1, get_line() );
		if( !info.dict.count( label ) ){
			s << "global symbol [" << label << "] = UNKNOWN";
		}
		else {
			v = info.dict[label];
			if( v.is_integer() ) {
				if( v.is_unknown() ){
					s << "Global integer symbol [" << label << "] = UNKNOWN";
				}
				else {
					s << "Global integer symbol [" << label << "] = " << v.i << " (0x" << std::hex << v.i << ")";
				}
			}
			else {	//if( v.is_string() ) {
				if( v.is_unknown() ){
					s << "Global string symbol [" << label << "] = UNKNOWN";
				}
				else{
					s << "Global string symbol [" << label << "] = \"" << v.s << "\"";
				}
			}
		}
		log.write_message( s.str() );
		log.write_separator();
	}
	if( this->is_data_fixed ) {
		if( check_all_fixed() && info.dict.count( label ) ){
			return true;
		}
	}
	if( words[0][0] == '\"' ) {
		put_error( CZMA_ERROR::get( CZMA_ERROR_CODE::INVALID_COMMAND ) );
		return false;
	}
	if( this->expression( info, 2, v ) ) {
		if( info.dict.count( label ) && !info.dict[ label ].is_unknown() ) {
			put_error( CZMA_ERROR::get( CZMA_ERROR_CODE::MULTIPLE_DEFINITION ) );
			return false;
		}
		else {
			if( !v.is_unknown() ){
				this->is_data_fixed = true;
			}
			if( !info.dict.count( label ) || info.dict[ label ].value_type != v.value_type ){
				info.is_updated = true;
			}
			info.dict[label] = v;
		}
	}
	else {
		put_error( CZMA_ERROR::get( CZMA_ERROR_CODE::LABEL_IS_INDETERMINATE ), info.all_error_enable, info.error_disable );
		info.error_disable = true;
		return false;
	}
	return check_all_fixed();
}
