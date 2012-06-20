import sys

__THE_FIRST_PAGE_SIZE = 256

if __name__ == '__main__':

	file_handler = open(sys.argv[1])
	w_file_handler = open(sys.argv[2], 'w')
	lines = file_handler.readlines()
	val = ''
	idx = 0

	for i in range(__THE_FIRST_PAGE_SIZE):
		val = '{0:04X}'.format(i)
		val += ":FF\r\n"
		w_file_handler.write(val)

	for line in lines:
		val2 = line[1:3]
		val3 = line[3:5]
		i += 1;
		val = '{0:04X}'.format(i)
		val = val + ':' + val2 + "\r\n"
		w_file_handler.write(val)

		i += 1;
		val = '{0:04X}'.format(i)
		val = val + ':' + val3 + "\r\n"
		w_file_handler.write(val)

	w_file_handler.close()
	file_handler.close()

