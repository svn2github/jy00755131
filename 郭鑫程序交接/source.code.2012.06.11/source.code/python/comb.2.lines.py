if __name__ == '__main__':
	file_handler = open("C:\\Users\\kuoxin\\Desktop\\x6.0.EEPROM")
	w_file_handler = open("C:\\Users\\kuoxin\\Desktop\\x6.1.EEPROM", 'w')
	lines = file_handler.readlines()
	val = ''
	for line in lines:
		list = line.split(':')
		if(len(list)) == 2:
			if(val == ''):
				val = list[1].rstrip()
			else:
				val += list[1]
				w_file_handler.write(val)
				val = ''

	w_file_handler.close()
	file_handler.close()
