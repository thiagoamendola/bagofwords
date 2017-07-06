import numpy
import sys
import os
import re
from sklearn.model_selection import train_test_split
from sklearn.model_selection import ShuffleSplit
from sklearn.model_selection import StratifiedKFold

def split_filename(file):
	filename = os.path.basename(file)
	filename, ext = os.path.splitext(filename)
	label, idx = filename.split('_')
	return int(label), int(idx)

def id(file):
	return split_filename(file)[1]

def label(file):
	return split_filename(file)[0]

def main():

	if len(sys.argv) != 5:
		print('Usage  <input files path [.txt]> <seed [int] > <number of samples [int]> <testSize [0.0 - 1.0 (float)]>')
		exit(1)


	currentDir = os.getcwd()
	symbolBetweenFolder = '/'
	strAux = sys.argv[1]

	occorruences = numpy.array([])
	for m in re.finditer(symbolBetweenFolder, strAux):
		occorruences = numpy.append(occorruences,m.start())




	if len(occorruences) == 1 :
		start = 0
		if strAux[0] == '.':
			start += 1
		databaseName = strAux[start:int(occorruences[0])]
	elif len(occorruences) > 1:
		lastIndex = int(len(occorruences)-1)
		secondLastIndex = int(len(occorruences)-2)
		start = int(occorruences[secondLastIndex])
		end = int(occorruences[lastIndex])
		databaseName = strAux[start:end]
	else:
		databaseName = ''



	path2files = currentDir + symbolBetweenFolder + databaseName
	print(path2files)
	seed = int(sys.argv[2])
	n = int(sys.argv[3])
	testSize = float(sys.argv[4])
	ntrain = int(n*(1-testSize))
	ntest = n-ntrain;


	inpath  = numpy.loadtxt(sys.argv[1], dtype='str')
	vectorizedFunction = numpy.vectorize(split_filename);
	arrayTuplas = vectorizedFunction(inpath)
	sampleLabels = arrayTuplas[:][0]
	sampleIds = arrayTuplas[:][1]
	#print str(arrayTuplas[0][0]) + ' ' + str(arrayTuplas[0][1]) ;


	classes =  numpy.unique(sampleLabels)
	nclasses = len(classes)
	samplesPerClassTrain = [0]*(nclasses)
	samplesPerClassTest = [0]*(nclasses)

	for i in range(nclasses):
		samplesPerClassTrain[i] = ntrain/nclasses
		samplesPerClassTest[i] = ntest/nclasses

	selectedTrain = []
	selectedTest = []
	numpy.random.seed(seed)
	#import pdb; pdb.set_trace()
	for i, c in enumerate(classes):
		samples = numpy.where(sampleLabels == c)[0]
		numpy.random.shuffle(samples)
		classTrain = int(samplesPerClassTrain[i] * len(samples))
		classTest = int((samplesPerClassTrain[i]+samplesPerClassTest[i]) * len(samples))
		selectedTrain.extend(samples[0: classTrain ])
		selectedTest.extend(samples[ classTrain : classTest ])

	pathTrain =  numpy.chararray(inpath[selectedTrain].shape,itemsize=100)
	pathTest =  numpy.chararray(inpath[selectedTest].shape,itemsize=100)

	#inpath.astype('U13')
	inpath = inpath.tolist()
	inpath = [x[2:len(x)-1] for x in inpath]

	k = 0
	for i in selectedTrain:
		pathTrain[k] = path2files + symbolBetweenFolder + inpath[i]
		k += 1

	k = 0
	for i in selectedTest:
		pathTest[k] = path2files +  symbolBetweenFolder + inpath[i]
		k += 1

	strTrain = [str(x.decode("utf-8")) for x in pathTrain]
	strTest = [str(x.decode("utf-8")) for x in pathTest]

	numpy.savetxt(path2files+"/train_paths.txt", strTrain, fmt='%s')
	numpy.savetxt(path2files+"/test_paths.txt", strTest, fmt='%s')


if __name__=='__main__':
	main()
