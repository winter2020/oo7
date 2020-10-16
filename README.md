# oo7: Detecting and Patching Spectre Vulnerabilities on Binary. 
A binary analysis framework to defend against potential vulnerability to Spectre attacks. Our key contribution is to balance the concerns of effectiveness, analysis time and run-time overheads. We employ control flow extraction, taint analysis, and address analysis to detect tainted conditional branches and speculative memory accesses.<oo7>
	
	
## Publication 

>  Guanhua Wang, Sudipta Chattopadhyay, Ivan Gotovchits, Tulika Mitra, and Abhik Roychoudhury. oo7: Low-overhead Defense against Spectre Attacks via Program Analysis. IEEE Transactions on Software Engineering(2020). 

Paper link: [oo7TSE](https://www.comp.nus.edu.sg/~abhik/pdf/TSE20_oo7.pdf)

### Cite:
```
@article{guanhua2020oo7,
  title={{oo7}: Low-overhead Defense against Spectre Attacks via Program Analysis},
  author={Guanhua Wang and Sudipta Chattopadhyay and Ivan Gotovchits and Tulika Mitra and Abhik Roychoudhury},
  journal={IEEE Transactions on Software Engineering},
  year={2020},
  publisher={IEEE}
}
```

## License
NOTE: You should agree with the licensing agreement (LICENSE.pdf) before using the tool. 

## Directory orgnization:
    ./check                # the lisp files for ddtbd (core engine of oo7)
    ./ddtbd                # the source code for spectre detection
    ./toy                  # a toy example from Spectre paper: https://spectreattack.com/spectre.pdf
    ./tool                 # a tool to profile the output (incidents) of the detection 
    ./patch                # patch code for bab
    ./testcases       	   # simple test cases
        - Kocher_tests/    # the examples from Paul Kocher's post: https://www.paulkocher.com/doc/MicrosoftCompilerSpectreMitigation.html
 
 
## How to install and run:

### Install opam and Bap.
    Please follow the instructions on the following page to install opam and bap:

    A. Install opam-1.2.2 or later.
        $ sudo add-apt-repository --yes ppa:avsm/ppa
        $ sudo apt-get update
        $ sudo apt-get --yes install opam

    B. Initialize opam and to install OCaml compiler.
        $ opam init --comp=4.05.0
        $ eval `opam config env`

    C. Install bap and its system dependencies
        $ opam depext --install bap


    *Reference: https://github.com/BinaryAnalysisPlatform/bap/wiki/Build-tips-and-tricks 


### Install and compile the development version of Bap.
```
clone bap project: 
$ git clone https://github.com/BinaryAnalysisPlatform/bap
$ git checkout tags/v1.5.0

pin development version of bap to opam:
$ opam pin add bap to/your/bap/project/path
opam will automatically compile the latest bap.

update your PATH:
$ eval `opam config env`

Make sure bap is the version 1.5.0
$ bap --version 
1.5.0-dev
```

### Copy "check/" directory and patch file to your opam share directory.
```
$ copy check -r ~/.opam/4.05.0/share/bap/
$ copy patch/posix.h ~/.opam/4.05.0/share/bap-api/c/

NOTE: This path may be different according to your opam installation and opam switch
```

### Build and install ddtbd plugin.

```
Build:
$ cd ddtbd/
$ bapbuild -clean 
$ bapbuild ddtbd.plugin -pkgs bap-taint,bap-primus

Install plugin:
$ bapbundle install ddtbd.plugin
```


### Run the toy example. 
```
 $ cd toy/
 $ bap ./test --recipe=check
 ```


### Profile the output of detection.
```
$ objdump -S test > test.asm
$ ./tool/incidents_profile.py incidents test.asm

You can find a profile file with name "incidents_profile.txt" in you directory. 

The content of incidents_profile.txt
	====================================
	@branches: 12                # all condition branches
	@S1: 1 (8.333%)              # tainted branches <CB>
	@S2: 1 (8.333%)              # tainted branches with IM1 <CB, IM1>
	@S2_avg_dis: 10              # average distance between CB and IM1
	@S3: 1 (8.333%)              # tainted branches with IM1, IM2 <CB, IM1, IM2>
	@S3_avg_dis: 7               # average distance between CB and IM2

	S1#4005c8                    # address of CB

	S2#4005c8#4005cf#3           # address of CB and IM1 along with the distance of CB and IM1
	S2#4005c8#4005de#7

	S3#4005c8#4005cf#4005de#7    # address of CB, IM1, IM2 along with the distance of CB and IM2

	taint#400648                 # tainted addresses. 
	taint#4005d9
	taint#4005cf
	taint#4005bd
	taint#400642
	taint#4005d6
	...
	===================================
```


### Testing for Paul Kocher' examples:
```
$ cd Kocher_tests/v01
$ gcc test.c -g -o test
$ bap test/test --recipe=check
$ ../../tool/incidents_profile.py incidents test.asm
```

## Other options
```
use $ bap --ddtbd-help for more options
Note: 
A. Use '--ddtbd-ignore-program-dependencies' or '--ddtbd-ignore-program-dependencies --ddtbd-ignore-control-dependencies' option will give you less detection results, but it may miss some true positives. 
B. You can edit the "recipe.scm" to enable or disable the options. 
```


