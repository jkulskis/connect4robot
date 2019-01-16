def getPhrases(filename):
	#Returns a list containing each line of a text file
	with open(filename) as f:
		phrases = f.readlines()
	for i in range(len(phrases)):
		phrases[i] = phrases[i].rstrip()
	return phrases

def checkPhrases(phrases, filename):
	#Checks if a phrase will work with a 16x2 LCD and returns [[good phrases], [bad phrases]]
	#filename is the prefix to add to the saved phrases
	good = []
	bad = []
	for phrase in phrases:
		firsthalf = ''
		if len(phrase) <= 16:
			good += [phrase]
		else:
			for i in range(16, 0, -1):
				if phrase[i] == ' ':
					firsthalf = phrase[:i]
					secondhalf = phrase[i+1:len(phrase)]
					break
			if firsthalf == '':
				bad.append([phrase, 'first half too long by ' + str(len(phrase) - 16) + ' chars = ' + '"' + phrase[16:] + '"'])
			else:
				print(firsthalf)
				print(secondhalf)
				if len(secondhalf) <= 16:
					good += [phrase]
				else:
					bad.append([phrase, 'second half too long by ' + str(len(secondhalf) - 16) + ' chars = ' + '"' + secondhalf[16:] + '"'])
	with open(filename + 'PhrasesChecked.txt', 'w') as saveFile:
		saveFile.write('const char * ' + filename + 'Phrases[' + str(len(good)) + '] = {')
		text = ''
		for phrase in good:
			text += '"' + phrase + '", '
		text = text[:-2]
		saveFile.write(text)
		saveFile.write('};\n\nBad Phrases:\n')
		for phraseInfo in bad:
			saveFile.write(phraseInfo[0] + ' : ' + phraseInfo[1] + '\n')

	return [good, bad]

phrases = getPhrases('chosePhrases.txt')
print(phrases)
checked = checkPhrases(phrases, 'chose')
print(checked)
