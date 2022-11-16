Steps to install VLC module into NS3:

Prerequisites:

1-Having the NS3 environment installed and running in your linux environment

Installation:

download .zip file of the vlc module

open .zip file and you should see a simple folder named vlcnew

enter that folder and you should see folders that mimic the ones in the repository

Now, go into the terminal and build a skeleton module by typing "./create-module.py vlcnew" (please follow ns3 guidline on how to build new module)

Find your NS3 file system and in the "src" folder there should be a "vlcnew" folder, open that and you should see similar folder names to that of the repository.

you should delete those files as they will be replaced by the files downloaded from the repository

Look back to where you unziped the downloaded repository files and copy and paste those folders and files into this newly made folder (do not copy the scratch folder)

Now, start up the terminal and go to the ns-3-dev directory

While in this directory build all the NS3 modules (if unsure use the installation guide to help, I use ubuntu and my command is ./waf build)

It is optional to enable tests and examples but it should build regardless ( build will take sometimes! have your coffee ready!)

now you can use the vlcnew-module in ns3 simulations

as far as the error model and propagation model are concerned, they can be also put into the model folder of the module but propagation can also be added onto the propagation module that is native to ns3, just be sure to add everything to its respective wscript

//Running a script

create a file a .cc file and follow the format that the example scripts use (in terms of laying out the script)

move the file into the /scratch directory (must run any test from the scratch directory)

to compile and run use the command "./waf --run scratch/your_file_name_without_.cc
