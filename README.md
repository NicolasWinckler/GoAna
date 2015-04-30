#GoAna

##The framework
Statistical data analysis framework of the orbital electron capture oscillation GSI experiment

## Installation

### Pre-requesite
1. Install FairSoft
2. Make sure that the $SIMPATH environment variable is set to FairSoft install path before starting the GoAna installation

### Optionnal
1. download [BAT](https://www.mppmu.mpg.de/bat/) and [Cuba](http://www.feynarts.de/cuba/) framework 
2. Follow standard cuba installation instruction
3. Follow BAT installation instruction and set the BAT installation path prefix to the FairSoft installation path ($SIMPATH)


###  Install GoAna
1. Go to your cloned GoAna repository
2. Do 
```bash
    mdkir build
    cd build
    cmake ..
    make  
    #(or make -jn, for n cores)
    . config.sh
```

### Git workflow
1. Follow the Git workflow given [here](https://github.com/AnarManafov/GitWorkflow/blob/master/GitWorkflow.markdown)

### more docs...
- input/output/config paths are in the workflow directory
- root input data files should be placed in worspace/input. 
To change the path of the input file change the value of the key "file.name" in [input.data] section. 
Or alternatively in command line add --input.data.file.name myfilename
Do the same for the tree and branch names in case of root files.


### Todo:
- Fix LinkDef error messages
- Import and refactor Bayesian and simulation packages
- Refactor File manager
- More docs
