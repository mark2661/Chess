import os
import shutil

if(os.path.isdir(os.path.join(os.getcwd(), "build"))):
    shutil.rmtree(os.path.join(os.getcwd(), "build"))
    
os.mkdir("build")
os.chdir(os.path.join(os.getcwd(), "build"))
os.system("cmake ..")
