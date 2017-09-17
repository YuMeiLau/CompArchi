
output_list = []
hex_list = []
valid_testcase = [1,2,3]
for i in valid_testcase:
	output_list.append('testcaseoutput'+str(i))
	hex_list.append('testcase'+str(i) + '.hex')

for outter_o, outter_h in zip(output_list, hex_list):
	with open(outter_o, 'r') as o, open(outter_h, 'r') as h:
		content_o = o.readlines()
		content_h = h.readlines()
		if content_o == content_h:
			print('Pass')
		else:
			print('Error')
			print(content_o)
			print(content_h)
	