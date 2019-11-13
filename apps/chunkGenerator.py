from random import uniform, randint

with open('data.data', 'w') as fin:
    #player position in x, z, y format
    fin.write('42 32 15\n')

    for i in range(0, 3):
        #global chunk position
        fin.write('{} {} 0\n'.format(randint(0, 100), randint(0, 100)))

        for j in range(0, 65536):
            #chunk gloabl pos in xzy format
            #block id
            fin.write('{}\n'.format( randint(0, 65) ))
