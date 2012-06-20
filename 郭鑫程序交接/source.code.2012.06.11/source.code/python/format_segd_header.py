import string, sys

start_str = """	int i = 0x0;
	SEGD_TRACE_HEADER_EXTENSION_x_PRIVATE_PTR ptr_p = trace_header_extended_private;
	SEGD_TRACE_HEADER_EXTENSION_x_PTR ptr = trace_header_extended;

	if(be_reverted_private){
"""

else_str = """	}
	else{
		
"""

end_str = """	}"""

assign_str_1 = "ptr_p->{0[1]} = ptr->{0[1]};\n" 
assign_str_2 = "ptr->{0[1]} = ptr_p->{0[1]};\n"
memcpy_str_1 = "memcpy(ptr_p->{0[1]}, ptr->{0[1]}, {0[2]} * sizeof({0[0]}));\n"
memcpy_str_2 = "memcpy(ptr->{0[1]}, ptr_p->{0[1]}, {0[2]} * sizeof({0[0]}));\n"

ushort_str_1 = "convert_ushort_littlend_to_bigend( &(ptr->{0[1]}), &(ptr_p->{0[1]}));\n"
ushort_str_2 = "convert_ushort_bigend_to_littlend( &(ptr_p->{0[1]}), &(ptr->{0[1]}));\n"
ushort_for_str_1 = """for(i = 0; i < {0[2]}; ++i) convert_ushort_littlend_to_bigend( &(ptr->{0[1]})+i, &(ptr_p->{0[1]})+i);\n"""
ushort_for_str_2 = """for(i = 0; i < {0[2]}; ++i) convert_ushort_bigend_to_littlend( &(ptr_p->{0[1]})+i, &(ptr->{0[1]})+i);\n"""

uint_str_1 = "convert_ulong_littlend_to_bigend( &(ptr->{0[1]}), &(ptr_p->{0[1]}));\n"
uint_str_2 = "convert_ulong_bigend_to_littlend( &(ptr_p->{0[1]}), &(ptr->{0[1]}));\n"
uint_for_str_1 = """for(i = 0; i < {0[2]}; ++i) convert_ulong_littlend_to_bigend( &(ptr->{0[1]})+i, &(ptr_p->{0[1]})+i);\n"""
uint_for_str_2 = """for(i = 0; i < {0[2]}; ++i) convert_ulong_bigend_to_littlend( &(ptr_p->{0[1]})+i, &(ptr->{0[1]})+i);\n"""

float_str_1 = "convert_float_littlend_to_bigend( &(ptr->{0[1]}), &(ptr_p->{0[1]}));\n"
float_str_2 = "convert_float_bigend_to_littlend( &(ptr_p->{0[1]}), &(ptr->{0[1]}));\n"
float_for_str_1 = """for(i = 0; i < {0[2]}; ++i) convert_float_littlend_to_bigend( &(ptr->{0[1]})+i, &(ptr_p->{0[1]})+i);\n"""
float_for_str_2 = """for(i = 0; i < {0[2]}; ++i) convert_float_bigend_to_littlend( &(ptr_p->{0[1]})+i, &(ptr->{0[1]})+i);\n"""

double_str_1 = "convert_double_littlend_to_bigend( &(ptr->{0[1]}), &(ptr_p->{0[1]}));\n"
double_str_2 = "convert_double_bigend_to_littlend( &(ptr_p->{0[1]}), &(ptr->{0[1]}));\n"
double_for_str_1 = """for(i = 0; i < {0[2]}; ++i) convert_double_littlend_to_bigend( &(ptr->{0[1]})+i, &(ptr_p->{0[1]})+i);\n"""
double_for_str_2 = """for(i = 0; i < {0[2]}; ++i) convert_double_bigend_to_littlend( &(ptr_p->{0[1]})+i, &(ptr->{0[1]})+i);\n"""

def format_code():
	new_file = open('d:\\new_file.txt', 'w')
	new_file.write(start_str)
	file_handle = open('d:\\conv.txt', 'r')
	all_lines = file_handle.readlines()
	for line in all_lines:
		token = []
		index = line.find(';')
		if index != -1:
			str = line[:index]
		else:
			str = line
		str = str.strip()
		list = str.split(' ')
		if len(list) < 2:
			continue
		token.append(list[0])
		index = list[1].find('[')
		if index != -1:
			str = list[1].strip()
			list = str.split('[')
			token.append(list[0])
			index = list[1].find(']')
			if index != -1:
				str = list[1]
				list = str.split(']')
				token.append(list[0])
			else:
				print("have a error about ']'")
		else:
			token.append(list[1])			
		if token[0] == 'SEGD_UCHAR':
			if len(token) == 3:
				str1 = memcpy_str_1.format(token)
			else:
				str1 = assign_str_1.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_USHORT':
			if len(token) == 3:
				str1 = ushort_for_str_1.format(token)
			else:
				str1 = ushort_str_1.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_UINT':
			if len(token) == 3:
				str1 = uint_for_str_1.format(token)
			else:
				str1 = uint_str_1.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_FLOAT':
			if len(token) == 3:
				str1 = float_for_str_1.format(token)
			else:
				str1 = float_str_1.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_DOUBLE':
			if len(token) == 3:
				str1 = double_for_str_1.format(token)
			else:
				str1 = double_str_1.format(token)
			new_file.write(str1)
	new_file.write(else_str)
	
	for line in all_lines:
		token=[]
		index = line.find(';')
		if index != -1:
			str = line[:index]
		else:
			str = line
		str = str.strip()
		list = str.split(' ')
		if len(list) < 2:
			continue
		token.append(list[0])
		index = list[1].find('[')
		if index != -1:
			str = list[1].strip()
			list = str.split('[')
			token.append(list[0])
			index = list[1].find(']')
			if index != -1:
				str = list[1]
				list = str.split(']')
				token.append(list[0])
			else:
				print("have a error about ']'")
		else:
			token.append(list[1])
		if token[0] == 'SEGD_UCHAR':
			if len(token) == 3:
				str1 = memcpy_str_2.format(token)
			else:
				str1 = assign_str_2.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_USHORT':
			if len(token) == 3:
				str1 = ushort_for_str_2.format(token)
			else:
				str1 = ushort_str_2.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_UINT':
			if len(token) == 3:
				str1 = uint_for_str_2.format(token)
			else:
				str1 = uint_str_2.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_FLOAT':
			if len(token) == 3:
				str1 = float_for_str_2.format(token)
			else:
				str1 = float_str_2.format(token)
			new_file.write(str1)
		elif token[0] == 'SEGD_DOUBLE':
			if len(token) == 3:
				str1 = double_for_str_2.format(token)
			else:
				str1 = double_str_2.format(token)
			new_file.write(str1)	
	new_file.write(end_str)
	new_file.close()
	file_handle.close()
if __name__ == '__main__':
    format_code()

