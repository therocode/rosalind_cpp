with open('textout.txt', 'r') as inFile:

  print("reading data file")
  data = inFile.read()

  aAmount = 0
  cAmount = 0
  gAmount = 0
  tAmount = 0

  print("counting")
  for nucleotide in data:
    if nucleotide == "A":
      aAmount += 1
    elif nucleotide == "C":
      cAmount += 1
    elif nucleotide == "G":
      gAmount += 1
    elif nucleotide == "T":
      tAmount += 1

  print(aAmount, " ", cAmount, " ", gAmount, " ", tAmount)
