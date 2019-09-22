from random import randint

fNames = "Grace Hunter Glenn Aaron Marco Aubrey Kayla Garrett Emily Tina Billy Mel Sarah Ryan Cole Natalie Corgan Dalton John Anakin Han Ben Jacob Miguel"
lNames = "Meyers Schmitt Hackbarth Pluym Uribe Anderson Sweet Beebe Brewer Wenzel Gerard Graf Mackey Jensen Peterson Walker Snow Skywalker Solo Kenobi Wertz Corsale Kobold Zuniga"
emails = "gmail.com bvu.edu hotmail.com yahoo.com"
status = "H Z M C"

fNames = fNames.split()
lNames = lNames.split()
emails = emails.split()
status = status.split()

out = []

with open("input.data", "w") as fin:
    for i in range (0, 1000):
        f = fNames[randint(0, len(fNames)-1)]
        l = lNames[randint(0, len(lNames)-1)]
        e = emails[randint(0, len(emails)-1)]
        s = status[randint(0, len(status)-1)]
        gamesPlayed = randint(0, 7)

        email = "{}{}@{}".format(f.lower(), l.lower(), e)

        line = "{} {} {}\n".format(s, gamesPlayed, email)
        fin.write(line)
        out.append(line)
with open("output.data", "w") as fout:
    for line in out:
        l = line.strip().split()
        email = l[2].split("@")
        if email[1]=="bvu.edu" and l[1]=="7":
            l[1] = "8"
            fout.write("{} {} {}\n".format(l[0], l[1], email[0]))
