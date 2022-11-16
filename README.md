Steps to install VLC module into NS3:

Prerequisites:

1-Having the NS3 environment installed and running in your linux environment

Installation:

1.download .zip file of the vlc module

2.open .zip file and you should see a simple folder named vlcnew

3.enter that folder and you should see folders that mimic the ones in the repository

4.Now, go into the terminal and build a skeleton module by typing "./create-module.py vlcnew" (please follow ns3 guidline on how to build new module)

5.Find your NS3 file system and in the "src" folder there should be a "vlcnew" folder, open that and you should see similar folder names to that of the repository.

6.you should delete those files as they will be replaced by the files downloaded from the repository

7.Look back to where you unziped the downloaded repository files and copy and paste those folders and files into this newly made folder

8.Now, start up the terminal and go to the ns-3-dev directory

9.While in this directory build all the NS3 modules

10.It is optional to enable tests and examples but it should build regardless 

now you can use the vlcnew-module in ns3 simulations

as far as the error model and propagation model are concerned, they can be also put into the model folder of the module but propagation can also be added onto the propagation module that is native to ns3, just be sure to add everything to its respective wscript

//Running a script

create a file a .cc file and follow the format that the example scripts use (in terms of laying out the script)

move the file into the /scratch directory (must run any test from the scratch directory)

to compile and run use the command "./ns3 --run scratch/your_file_name_without_.cc
