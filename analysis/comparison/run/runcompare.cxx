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
#include "TCanvas.h"

// sids/goana lib
#include "SIDSLog.h"
#include "EsrInjData.h"
#include "EsrTree.h"
#include "OscConfig.h"
#include "SIDSFileManager.h"

/*
 * 
 */


int main(int argc, char** argv) 
{
    SET_LOG_LEVEL(DEBUG);
    
    OscConfig* config = nullptr; 
    SIDSFileManager* man_visual = nullptr;
    SIDSFileManager* man_auto = nullptr;
    
    std::vector<EsrInjData> DataContainer;
    
    try
    {
        config= new OscConfig();
        if (config->ParseAll(argc,argv,true))
            return 0;
        
        
        std::string filename=config->GetPar<std::string>("input.data.file.name");
        std::string treename=config->GetPar<std::string>("input.data.file.tree.name");
        std::string branchname=config->GetPar<std::string>("input.data.file.branch.name");

        man_visual = new SIDSFileManager(filename,treename,branchname);
        //man_auto =  new SIDSFileManager(filename,treename,branchname);
        
        
        DataContainer=man_visual->GetAllData(true);
        // true = pre-sorted 
        // false = direct from tree
        
    }
    catch (std::exception& e)
    {
        SLOG(ERROR) << e.what();
        return 1;
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

        for(auto& file : DataContainer)
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
    
    if(man_visual)
        delete man_visual;
    
    return 0;
}

