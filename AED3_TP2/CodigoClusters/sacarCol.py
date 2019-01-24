
with open("in/zahn  .txt", "r") as fin:
    with open("zahn.txt", "w") as fout:
        for line in fin:
            line = line.split('\t')
            print((line))
            if (len(line) == 3):
                del line[2]
                fout.write(line[0] + ' ' + line[1] + "\n")
            elif(len(line)==1):
                fout.write(line[0])
            else:
                fout.write("\n")
