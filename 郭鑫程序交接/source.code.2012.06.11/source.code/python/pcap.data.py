import sys, string

dest_ip = '192.168.0.17'
dest_port = '38656'
net_protocol = 'UDP'

prev_count = 0.0
cur_count = 0.0

if __name__ == '__main__':
	in_file = open("d:\\test.csv", 'r')
	out_file = open("d:\\output.log", 'w')

	for line in in_file.readlines():
		line = line.strip()
		ls = line.split(',')
		str = ls[5]
		ls_5 = str.split()
		if ls_5[5] == dest_port and ls[3] == dest_ip and ls[4] == net_protocol:
			cur_count = ls[1]
			if prev_count == 0x0:
				prev_count = cur_count
			else:
				if (cur_count * 100 - prev_count * 100) > 77:
					out_file.write(line)
					out_file.flush()
	
	in_file.close()
	out_file.close()

