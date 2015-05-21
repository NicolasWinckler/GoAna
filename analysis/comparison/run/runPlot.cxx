/* 
 * File:   runPlot.cxx
 * Author: winckler
 *
 * Created on May 20, 2015, 5:47 PM
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
#include "SimpleConfig.h"
#include "SIDSFileManager.h"

/*
 * 
 */

int main(int argc, char** argv) 
{
    SET_LOG_LEVEL(DEBUG);
    
    SimpleConfig* config = nullptr; 
    SIDSFileManager* man = nullptr;
    
    std::vector<EsrInjData> DataContainer;
    
    try
    {
        
        config = new SimpleConfig();
        config->UseConfigFile();
        po::options_description decayNbDesc("requested decay number");
        decayNbDesc.add_options() 
            ("MaxNbDecayPerFile", po::value<int>()->default_value(20), "requested maximum number of decay per file");
        config->AddToCmdLineOptions(decayNbDesc);
        
        if (config->ParseAll(argc,argv,true))
            return 0;
        
        
        std::string filename=config->GetPar<std::string>("input.data.file.name");
        std::string treename=config->GetPar<std::string>("input.data.file.tree.name");
        std::string branchname=config->GetPar<std::string>("input.data.file.branch.name");

        man = new SIDSFileManager(filename,treename,branchname);
        
        
        DataContainer=man->GetAllData(true);
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
        int requested_decaynumber_perfile=config->GetPar<int>("MaxNbDecayPerFile");
        // Create and fill histogram
        TH1D* histox = new TH1D("fx","EC-decay histogram",binning,xmin,xmax);

        for(auto& file : DataContainer)
        {
            std::vector<EsrDecayEvent> eventlist = file.GetECData();
            if(eventlist.size() <= requested_decaynumber_perfile)
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
    
    if(man)
        delete man;
    
    return 0;
}


