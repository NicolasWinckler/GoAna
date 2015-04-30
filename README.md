#GoAna

##The Project
Statistical data analysis of the time modulated orbital electron capture decay experiments.

## Installation

### Prerequisite
1. Install FairSoft
2. Make sure that the $SIMPATH environment variable is set to FairSoft install path before starting the GoAna installation

### Optional
1. download [BAT](https://www.mppmu.mpg.de/bat/) and [Cuba](http://www.feynarts.de/cuba/) framework 
2. Follow standard cuba installation instruction
3. Follow BAT installation instruction and set the BAT installation path prefix to the FairSoft installation path ($SIMPATH)


###  Install GoAna
1. Open a terminal and go to the cloned GoAna repository
2. Then 
```bash
    mdkir build
    cd build
    cmake ..
    make  
    #(or make -jn, for n cores)
    . config.sh
```


#### Git workflow for participants
1. See [here](https://github.com/AnarManafov/GitWorkflow/blob/master/GitWorkflow.markdown)


#### Todo:
- Fix LinkDef error messages
- Import and refactor Bayesian and simulation packages
- Refactor File manager : clean, and adapt for multi data sets input and output
- Refactor base libs CMakeLists
- More docs

