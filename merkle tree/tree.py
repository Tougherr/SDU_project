import hashlib,random
import argparse

alphabet=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']

class TreeNode:
    def __init__(self,value):
        self.left = None
        self.right = None
        self.value = value
        self.hashValue = hashlib.sha256(value.encode('utf-8')).hexdigest()
    
def buildTree(leaves,f):
    nodes = []
    for i in leaves:
        nodes.append(TreeNode(i))

    while len(nodes)!=1:
        temp = []
        for i in range(0,len(nodes),2):
            node1 = nodes[i]
            if i+1 < len(nodes):
                node2 = nodes[i+1]
            else:
                temp.append(nodes[i])
                break
            f.write("Left child : "+ node1.value + " | Hash : " + node1.hashValue +" \n")
            f.write("Right child : "+ node2.value + " | Hash : " + node2.hashValue +" \n")
            concatenatedHash = node1.hashValue + node2.hashValue
            parent = TreeNode(concatenatedHash)
            parent.left = node1
            parent.right = node2
            f.write("Parent(concatenation of "+ node1.value + " and " + node2.value + ") : " +parent.value + " | Hash : " + parent.hashValue +" \n")
            temp.append(parent)
        nodes = temp 
    return nodes[0]



def parseFile():
    f = open("merkle.tree","r")
    tree ={}
    for line in f:
        lineArray = line.split(" ")
        if lineArray[0] == 'Parent(concatenation':
            tree[lineArray[6]] = lineArray[10]
        else:
            tree[lineArray[3]] = lineArray[7]
    return tree

def checkInclusion(inputString,tree):
    op = []
    for key,value in tree.items():
        if inputString in key:
            op.append(value)
            inputString = value
    return op


def main(args):
    if args.method=='build':
        leaves=[]
        for i in range(args.size):
            tmp=''
            for j in range(10):
                k=random.randint(0,25)
                tmp=tmp+alphabet[int(k)]
            leaves.append(tmp) 
        f = open("merkle.tree", "w")
        root = buildTree(leaves,f)
        f.close()
    elif args.method=='check':
        inputString = args.char
        tree = parseFile()
        op = checkInclusion(inputString,tree)
        if(len(op)> 0):
            print("yes",op)
        else:
            print("no inclusion")





if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='The Merkle Hash Tree')

    # common arguments
    parser.add_argument('--method', '-m', type=str, default='none')
    parser.add_argument('--size', '-s', type=int, default=0)
    parser.add_argument('--char', '-c', type=str, default='none')

    arguments = parser.parse_args()
    main(arguments)
