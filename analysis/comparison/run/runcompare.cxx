/* 
 * File:   runcompare.cxx
 * Author: winckler
 *
 * Created on April 3, 2015, 6:15 PM
 */

// std
#include <cstdlib>
#include <string>
#include <vector>

// root
#include "TApplication.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"

// sids/goana lib
#include "SIDSLog.h"
#include "EsrInjData.h"
#include "EsrTree.h"
#include "OscConfig.h"


/*
 * 
 */


int main(int argc, char** argv) 
{
    SET_LOG_LEVEL(DEBUG);
    
    OscConfig* config = new OscConfig();
    config->ParseAll(argc,argv,true);
    
    std::string filename=config->GetPar<std::string>("input.data.file.name");
    std::string treename=config->GetPar<std::string>("input.data.file.tree.name");
    std::string branchname=config->GetPar<std::string>("input.data.file.branch.name");
    
    
    // Get all data from file
    EsrTree *DecayTree = new EsrTree(filename,treename,branchname);
    std::vector<EsrInjData> fDataList = DecayTree->GetEsrData();
    delete DecayTree;
    
    if(fDataList.size()==0)
    {
        SLOG(ERROR)<<"Data could not be obtained. Check file and tree names";
        return 1;
    }
    
    /// Sort and check duplicated analyzed files
    
    // get all file names analyzed (including duplicate analysis)
    std::vector<std::string> inFileList;
    for(auto& file : fDataList)
    {
        string temp=file.GetFileName();
        inFileList.push_back(temp);
    }
    
    // sort file names
    std::sort(inFileList.begin(),inFileList.end());
    
    // create file list without duplicates
    std::vector<std::string> outFileList (inFileList.size()); 
    std::vector<std::string>::iterator it;
    it=std::unique_copy (inFileList.begin(),inFileList.end(),outFileList.begin());
    outFileList.resize( std::distance(outFileList.begin(),it) );
    

    /// count duplicate and create maps
    std::map<std::string,int> FileSummaryList;
    std::map<std::string,int> DuplicatesList;
    
    // for all unique file name
    for(auto& p : outFileList)
    {
        int countdupl=0;
        // for all analysis recorded
        for(auto& q : inFileList)
        {
            // if unique file name match all analysis file name
            if(p==q)
            {
                // count and update maps
                countdupl++;
                FileSummaryList[p]=countdupl;
                if(countdupl>1)
                    DuplicatesList[p]=countdupl;
            }
            
        }
    }
    
    /// plot data
    std::string execute=config->GetPar<std::string>("exec");
    if(execute=="plot")
    {
        double xmin=config->GetPar<double>("obs.xmin");
        double xmax=config->GetPar<double>("obs.xmax");
        int binning=config->GetPar<int>("obs.binning");
        // Create and fill histogram
        TH1D* histox = new TH1D("fx","EC-decay histogram",binning,xmin,xmax);

        for(auto& file : fDataList)
        {
            std::vector<EsrDecayEvent> eventlist = file.GetECData();
            for(auto& event : eventlist)
            {
                double t_ec=(double)event.GetDecayTime();
                histox->Fill(t_ec);
            }
        }
        
        
        // Plot data
        TApplication app("App", &argc, argv);
        TCanvas *Canvas_compare = new  TCanvas("canvas compare","canvas compare",1000,800);
        //Canvas_compare->Divide(4,2);
        //Canvas_compare->cd(1);
        histox->Draw();
        
        
        app.Run();
        
        if(Canvas_compare)
            delete Canvas_compare;
        if(histox)
            delete histox;
    }
    
    if(config)
        delete config;
    
    return 0;
}

