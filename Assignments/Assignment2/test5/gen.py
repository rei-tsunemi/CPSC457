#!/bin/env python3

import os

def pad(n, max):
  maxl = len(str(max))
  n = str(n)
  while len(n) < maxl:
    n = "0" + n
  return n

def make_group( tfname, gsize, contents, fsize):
  assert len(contents) > 0
  fname1 = tfname.replace("{}", pad(1, gsize))
  print("fname1=", fname1)
  rem = fsize
  with open(fname1, "w") as fp:
    while rem > 0:
      s = rem if rem < len(contents) else len(contents)
      fp.write(contents[:s])
      rem -= s
  for i in range(2, gsize+1):
    fnamei = tfname.replace("{}", pad(i, gsize))
    os.link(fname1, fnamei)


make_group( "sub/a{}.dat", 100, "fileOne ", 1024)
make_group( "sub/b{}.dat", 200, "file two\n", 1024)
make_group( "sub/c{}.dat", 300, " filethree  ", 1024)
make_group( "sub/d{}.dat", 400, " FILEFOUR \n", 1024)

