import sys

operator = ''

if __name__ == '__main__':
	in_file = open("d:\\in.txt", 'r')
	out_file = open("d:\\out.txt", 'w')

	for line in in_file.readlines():
		line = line.strip()
		ls = line.split(' ')
		val = ls[1]
		val = val.strip()
		val1 = val[0:-1]
		val = val[0:-2]
		if ls[0] == 'SEGD_UINT':
			operator = 'assign_uint'
		elif ls[0] == 'SEGD_UCHAR':
			operator = 'assign_uchar'
		elif ls[0] == 'SEGD_USHORT':
			operator = 'assign_ushort'
		elif ls[0] == 'SEGD_FLOAT':
			operator = 'assign_float'
		elif ls[0] == 'SEGD_DOUBLE':
			operator = 'assign_double'
		elif ls[0] == 'SEGD_ULONGLONG':
			operator = 'assign_ulonglong'

		if operator != '':
			new_line =  "{\"" + val + "\"" + ", &__segd_file.trace_header_.trace_header_extended_1_." + val1 + ", " + operator + "}, \n"
			out_file.write(new_line)
		operator = ''

	in_file.close()
	out_file.close()

		
