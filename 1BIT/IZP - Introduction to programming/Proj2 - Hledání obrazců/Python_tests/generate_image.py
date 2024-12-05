from random import randint

height = int(input("Enter the image's height: "))
width = int(input("Enter the image's width: "))

file = open("output.txt", "w")
file.write(f"{height} {width}\n{"\n".join([" ".join([str(randint(0, 1)) for _ in range(0, width)]) for _ in range(0, height)])}")