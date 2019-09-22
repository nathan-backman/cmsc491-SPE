import random

# load in 5
mylist = []

for i in range(0,5):
    mylist.append(int(input()))

print(sum(mylist)/len(mylist))

for i in range(0,47):
    mylist.pop(0);
    mylist.pop(0);
    mylist.append(int(input()));
    mylist.append(int(input()));
    print(sum(mylist)/len(mylist))
