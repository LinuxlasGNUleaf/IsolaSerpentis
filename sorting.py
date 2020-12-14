with open("rooms.txt","r") as readfile:
    with open("rooms_edited.txt","w") as writefile:
        lines = sorted(readfile.readlines(), key=lambda x: int(x[:2]))
        print(lines)
        writefile.writelines(lines)