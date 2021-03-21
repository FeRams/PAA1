import matplotlib.pyplot as plt

def main():
	reader = open('output.txt', 'r')
	size = reader.read()
	while size!='':
		time = reader.read()
		plt.plot(size, time)
		size = reader.read()
	reader.close()
	plt.xlabel('size') 
	plt.ylabel('time') 
	plt.title('execution time') 
	plt.show() 

if __name__ == "__main__":
    main()