#  Jh (A distribted version control system)

This is a distribted version control system similar to git.

## Motivation

My motivation behind it to learn more about internals of Git, and also advance my knowledge and practice writing code in C/C++ language.

## Architectre

The architecture i followed to bild this version control system is very much simailar to git. In this architecture every file is treated as `BLOB`
and every directory is treated as a `TREE` a tree may conatin either one or more blobs and it may also contain another tree.
First when the command `<executable> init` is being run it will create a folder named `.jh` this folder contains 
different information about the repository. When the programmer runs `<executable> commit "<commit-message>"` it first compresses the blob data and creates a unique
hash id after which the compressed data is stored in `.jh/objects` folder, similarly tree is also compreesed conationg information
like blob name, blob hash is stored in `.jh/objects`.
For further inqiry please visit `git internals`.

## Pre-Install

In Debian/Linux
bash
```
sudo apt-get update
sudo apt-get install libssl-dev zlib1g-dev
```

## How to Use:

1. First create the exectable by rnning the command `make`

2. After running `make` command rn command `make clean` to delete `*.o(object files)` files

3. After exectable has been created, run command `<executable> init` this command Initializes the repository

4. After Initializing the repository run command `<executable> commit "<commit-message>"` to commit changes to the repository.

5. Run command `<executable> log` to see all the commits with its hashes

6. To restore your working directory to different commit run command `<executable> restore <hash>`. 



## Other useful commands:

1. `<executable> hash-object <filename>` this command compresses the files using zlib compression and then
    creates `hash` of that compressed data

2. `<executable> cat-file <hash>` to see contains of file which is stored in `.jh/objects`

3. `<executable> ls-tree <folderpath> -w` to create a tree object


## WARNING 😞

This program is written in C/C++ and tested in Debian/Linux before running it in windows or any other operating
you may have to change some code :wrench:. Even though i tried my best to write croos-platform code
Thanks

