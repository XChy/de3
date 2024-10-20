#!python3
import os
import sys
import atexit
import multiprocessing
from tqdm import tqdm
from multiprocessing import Pool


def reduce(dir_to_reduce):
    reducer = os.path.join(os.path.split(__file__)[0], "reduce.py")
    return os.system(f"python3 {reducer} -d {dir_to_reduce} >/dev/null 2>&1")


def cleanup():
    for p in multiprocessing.active_children():
        p.terminate()


if __name__ == "__main__":
    atexit.register(cleanup)
    dir_to_reduce = sys.argv[1]
    dirs_to_reduce = [os.path.join(dir_to_reduce, dirname)
                      for dirname in os.listdir(dir_to_reduce)]
    with Pool() as p:
        result = list(tqdm(p.imap_unordered(
            reduce, dirs_to_reduce), total=len(dirs_to_reduce)))
